#include "honesconfig.h"

#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <directfb/directfb.h>
#include <upnp/ixml.h>
#include "util/Utils.h"
#include "ui/PlayerHandler.h"

using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace ui {

                        PlayerHandler* PlayerHandler::instance = NULL;

                        PlayerHandler::PlayerHandler() {

                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.util.playerhandler");
                            LoggerUtil_info(logger, "Entrando no construtor PlayerHandler()!");

                            avProvider = NULL;
                            imgProvider = NULL;
                            win = NULL;
                            font = NULL;
                            surface = NULL;
                            buffer = NULL;
                            streamingThread = NULL;
                            uri = "";
                            prevURI = "";
                            tmpFile = "";
                            prevFile = "";
                            metadata = "";
                            width = 0;
                            height = 0;
                            running = false;
                            mediaReady = false;
                            hasVisualContent = false;

                        }

                        PlayerHandler::~PlayerHandler() {
                            stop();
                            if (prevFile.size() != 0)
                                remove(prevFile.c_str());
                        }

                        PlayerHandler* PlayerHandler::PlayerHandlerFactory() {
                            if (instance == NULL) {
                                instance = new PlayerHandler();
                            }
                            return instance;
                        }

                        void PlayerHandler::PlayerHandlerDestructor(){
                            if (instance != NULL) {
                                delete instance;
                                instance = NULL;
                            }
                        }

                        void PlayerHandler::setRunning(bool running) {
                            this->running = running;
                        }

                        void PlayerHandler::initializeWindow() {
                            int s_width = GraphicsHandler::getScreenWidth() * 20 / 100;
                            int s_height = GraphicsHandler::getScreenHeight() * 15 / 100;
                            int s_posx = GraphicsHandler::getScreenWidth() - s_width - 10;
                            int s_posy = 10;
                            int borderSize = 5;

                            if (hasVisualContent == true) {
                                s_width = width + (borderSize * 2);
                                s_height = height + (borderSize * 2);

                                if (s_width > GraphicsHandler::getScreenWidth()) {
                                    s_width = GraphicsHandler::getScreenWidth();
                                    s_height = (((GraphicsHandler::getScreenWidth() * 100) / width) * height) / 100;
                                }
                                if (s_height > GraphicsHandler::getScreenHeight()) {
                                    s_height = GraphicsHandler::getScreenHeight();
                                    s_width = (((GraphicsHandler::getScreenHeight() * 100) / height) * width) / 100;
                                }

                                s_posx = (GraphicsHandler::getScreenWidth() - s_width) / 2;
                                s_posy = (GraphicsHandler::getScreenHeight() - s_height) / 2;
                            }

                            DFBWindowDescription desc;
                            desc.flags = (DFBWindowDescriptionFlags) (DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS);
                            desc.width = s_width;
                            desc.height = s_height;
                            desc.posx = s_posx;
                            desc.posy = s_posy;
                            desc.caps = DWCAPS_ALPHACHANNEL;

                            DFBRectangle surfaceRect;
                            surfaceRect.x = borderSize;
                            surfaceRect.y = borderSize;
                            surfaceRect.w = s_width - (borderSize * 2);
                            surfaceRect.h = s_height - (borderSize * 2);

                            win = GraphicsHandler::createFrame(&desc);

                            IDirectFBWindow* winPtr = (IDirectFBWindow*) win;

                            winPtr->RaiseToTop(winPtr);

                            IDirectFBSurface* tmp;

                            winPtr->GetSurface(winPtr, &tmp);

                            winPtr->SetOpacity(winPtr, 0xFF);

                            DFBWindowOptions opt;
                            winPtr->GetOptions(winPtr, &opt);
                            // Adiciona as opções ghost e alphachannel.
                            winPtr->SetOptions(winPtr, (DFBWindowOptions) (opt | DWOP_ALPHACHANNEL | DWOP_GHOST));
                            // Coloca a janela sobre todas as outras (modal).
                            winPtr->SetStackingClass(winPtr, DWSC_UPPER);

                            tmp->SetBlittingFlags(tmp, DSBLIT_BLEND_ALPHACHANNEL);

                            tmp->Clear(tmp, 0, 0, 0, 0xFF);

                            tmp->SetColor(tmp, 150, 150, 150, 0.5 * 0xFF);

                            tmp->FillRectangle(tmp, 0, 0, s_width, s_height);

                            tmp->Flip(tmp, NULL, DSFLIP_WAITFORSYNC);

                            tmp->GetSubSurface(tmp, &surfaceRect, (IDirectFBSurface**) &surface);

                            IDirectFBSurface* surfacePtr = (IDirectFBSurface*) surface;

                            surfacePtr->SetColor(surfacePtr, 0, 0, 0, 0xFF);

                            surfacePtr->FillRectangle(surfacePtr, 0, 0, surfaceRect.w, surfaceRect.h);

                            surfacePtr->Flip(surfacePtr, NULL, DSFLIP_WAITFORSYNC);
                        }

                        bool PlayerHandler::isRunning() {
                            return running;
                        }

                        void PlayerHandler::setURI(string uri) {
                            LoggerUtil_info(logger, "Entrando no método setURI(uri)!");
                            LoggerUtil_debug(logger, "uri: " << uri);

                            this->prevURI = this->uri;
                            this->uri = uri;
                        }

                        void PlayerHandler::setMetadata(string metadata) {
                            LoggerUtil_info(logger, "Entrando no método setMetadata(metadata)!");
                            LoggerUtil_debug(logger, "metadata: " << metadata);

                            this->metadata = metadata;
                        }

                        void PlayerHandler::setPlaybackSurface(void* surface) {
                            this->surface = surface;
                        }

                        struct streamingData {
                            const char* uri;
                            IDirectFBDataBuffer* playerBuffer;
                        };

                        void* bufferFeeder(void* param){
                            streamingData* streamingPtr = (streamingData*) param;
                            string uri = streamingPtr->uri;
                            IDirectFBDataBuffer* buffer = streamingPtr->playerBuffer;

                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.util.playerhandler");
                            LoggerUtil_debug(logger, "Buffer");
                            LoggerUtil_debug(logger, "Host: " << Utils::getHostURL(uri.c_str()) << " Port: " << Utils::getPortURL(uri.c_str()));

                            int sock = -1;
                            if ((sock = Utils::socketConnect(Utils::getHostURL(uri.c_str()), Utils::getPortURL(uri.c_str()))) >= 0) {

                                LoggerUtil_debug(logger, "socket connected!");

                                timeval tmvl;
                                tmvl.tv_sec = 2;
                                tmvl.tv_usec = 500;
                                const int flag = 1;

                                setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof (flag));
                                setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tmvl, sizeof (tmvl));

                                string cmd("GET ");
                                cmd.append(uri.substr(uri.substr(uri.find("://") + 3).find('/') + uri.find("://") + 3));
                                cmd.append(" HTTP/1.1\r\n");

                                cmd.append("User-Agent: PlayerHandler\r\n");

                                cmd.append("Host: ");
                                cmd.append(uri.substr(uri.find("://") + 3).substr(0, uri.substr(uri.find("://") + 3).find('/')));
                                cmd.append("\r\n");

                                cmd.append("Connection: Close\r\n");
                                cmd.append("\r\n");

                                if (Utils::socketSend(sock, cmd.c_str(), cmd.length()) == cmd.length()) {
                                    char bufferChar[8192];
                                    ssize_t responseLength = 0;
                                    unsigned int bufferLength = 0;
                                    string header;
                                    bool hasFound = false;

                                    do {
                                        pthread_testcancel();

                                        buffer->GetLength(buffer, &bufferLength);
                                        
                                        if (bufferLength <= MAX_BUFFER) {
                                            responseLength = recv(sock, bufferChar, 8192, MSG_NOSIGNAL);

                                            if (responseLength == -1)
                                                break;

                                            if (hasFound == true) {
                                                buffer->PutData(buffer, bufferChar, responseLength);
                                            } else {
                                                header.append(bufferChar);
                                                size_t headerPos = header.find("\r\n\r\n");
                                                if (headerPos != string::npos) {
                                                    hasFound = true;
                                                    buffer->PutData(buffer, &bufferChar[headerPos + 4], responseLength - headerPos - 4);
                                                }
                                            }
                                        } else {
                                            Utils::wait(100);
                                        }
                                    } while (responseLength > 0);

                                    buffer->Finish(buffer);
                                }

                                shutdown(sock, SHUT_RDWR);
                                close(sock);
                            } else {
                                LoggerUtil_debug(logger, "socket not connected!");
                            }

                            return NULL;
                        }

                        struct progressBarData {
                            off_t* totalReceived;
                            off_t total;
                        };
                        
                        void* progressBarThread(void* param){
                            progressBarData* data = (progressBarData*) param;
                            off_t* totalReceived = data->totalReceived;
                            off_t total = data->total;
                            off_t received = *totalReceived;

                            DFBWindowDescription desc;
                            desc.flags = (DFBWindowDescriptionFlags) (DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS);
                            desc.width = GraphicsHandler::getScreenWidth();
                            desc.height = 7;
                            desc.posx = 0;
                            desc.posy = GraphicsHandler::getScreenHeight() - 12;
                            desc.caps = DWCAPS_ALPHACHANNEL;

                            IDirectFBWindow* winPtr = (IDirectFBWindow*) GraphicsHandler::createFrame(&desc);
                            winPtr->RaiseToTop(winPtr);
                            winPtr->SetOpacity(winPtr, 0.5 * 0xFF);
                            IDirectFBSurface* progress;
                            winPtr->GetSurface(winPtr, &progress);
                            DFBWindowOptions opt;
                            winPtr->GetOptions(winPtr, &opt);
                            winPtr->SetOptions(winPtr, (DFBWindowOptions) (opt | DWOP_ALPHACHANNEL | DWOP_GHOST));
                            winPtr->SetStackingClass(winPtr, DWSC_UPPER);
                            progress->SetBlittingFlags(progress, DSBLIT_BLEND_ALPHACHANNEL);
                            progress->SetColor(progress, 0, 0xFF, 0, 0.8 * 0xFF);

                            while (received < total){
                                pthread_testcancel();

                                off_t aux = received;

                                received = *totalReceived;

                                if (received <= aux)
                                    break;

                                progress->FillRectangle(progress, 0, 1, received * desc.width / total, 5);
                                progress->Flip(progress, NULL, (DFBSurfaceFlipFlags) DSFLIP_WAITFORSYNC);

                                Utils::wait(1000);
                            }

                            winPtr->Destroy(winPtr);
                            winPtr->Release(winPtr);
                            winPtr = NULL;

                            return NULL;
                        }

                        void PlayerHandler::prepareMedia() {
                            LoggerUtil_info(logger, "Entrando no método prepareMedia()!");
                            LoggerUtil_debug(logger, "uri: " << uri);

                            if (((avProvider == NULL) && (imgProvider == NULL)) || stop()) {
                                mediaReady = false;
                                hasVisualContent = false;
                                avProvider = NULL;
                                imgProvider = NULL;
                                win = NULL;
                                font = NULL;
                                width = 0;
                                height = 0;
                                string type;

                                /* janela informativa: "Carregando..." */
                                if (surface == NULL) {
                                    initializeWindow();
                                    DFBFontDescription fdsc;
                                    fdsc.flags = (DFBFontDescriptionFlags) (DFDESC_HEIGHT | DFDESC_ATTRIBUTES);
                                    fdsc.attributes = DFFA_NONE;
                                    fdsc.height = 24;
                                    IDirectFB* dfb = (IDirectFB*) GraphicsHandler::getGraphicsRoot();
                                    if (dfb->CreateFont(dfb, NULL, &fdsc, (IDirectFBFont**) & font) == DFB_OK) {
                                        IDirectFBSurface* surfacePtr = (IDirectFBSurface*) surface;
                                        surfacePtr->SetColor(surfacePtr, 0xdd, 0xdd, 0xdd, 0xff);
                                        surfacePtr->SetFont(surfacePtr, (IDirectFBFont*) font);

                                        string text = "Carregando...";
                                        int textWidth = 0;
                                        ((IDirectFBFont*)font)->GetStringWidth((IDirectFBFont*)font, text.c_str(), -1, &textWidth);

                                        surfacePtr->DrawString(surfacePtr, text.c_str(), -1, ((GraphicsHandler::getScreenWidth() * 20 / 100) - textWidth)/2, ((GraphicsHandler::getScreenHeight() * 15 / 100) - (0.6 * fdsc.height))/2, DSTF_TOPLEFT);
                                        surfacePtr->Flip(surfacePtr, NULL, (DFBSurfaceFlipFlags) DSFLIP_WAITFORSYNC);
                                    }
                                }

                                if (metadata.length() > 0) {
                                    //Utils::parseXMLChars(metadata);

                                    IXML_Document* xml = ixmlParseBuffer(metadata.c_str());
                                    if (xml != NULL) {
                                        IXML_NodeList* resList = ixmlDocument_getElementsByTagName(xml, "res");
                                        if ( (resList != NULL) && (ixmlNodeList_length(resList) > 0) ) {
                                            IXML_Node* resNode = ixmlNodeList_item(resList, 0);
                                            if (resNode != NULL) {
                                                string tmp = Utils::getAttributeValue(resNode, "protocolInfo");
                                                // <protocol>’:’<network>’:’<contentFormat>’:’<additionalInfo>
                                                tmp = tmp.substr(tmp.find(":") + 1);
                                                tmp = tmp.substr(tmp.find(":") + 1);
                                                type = tmp.substr(0, tmp.find(":"));
                                            }
                                        }
                                    }
                                } else if (Utils::isAbsoluteURL(uri)) {
                                    int sock = -1;
                                    if ((sock = Utils::socketConnect(Utils::getHostURL(uri.c_str()), Utils::getPortURL(uri.c_str()))) >= 0) {

                                        LoggerUtil_debug(logger, "socket connected!");

                                        timeval tmvl;
                                        tmvl.tv_sec = 2;
                                        tmvl.tv_usec = 500;
                                        const int flag = 1;

                                        setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof (flag));
                                        setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tmvl, sizeof (tmvl));

                                        string cmd("HEAD ");
                                        cmd.append(uri.substr(uri.substr(uri.find("://") + 3).find('/') + uri.find("://") + 3));
                                        cmd.append(" HTTP/1.1\r\n");

                                        cmd.append("User-Agent: PlayerHandler\r\n");

                                        cmd.append("Host: ");
                                        cmd.append(uri.substr(uri.find("://") + 3).substr(0, uri.substr(uri.find("://") + 3).find('/')));
                                        cmd.append("\r\n");

                                        cmd.append("Connection: Close\r\n");
                                        cmd.append("\r\n");

                                        if (Utils::socketSend(sock, cmd.c_str(), cmd.length()) == cmd.length()) {
                                            char bufferChar[8192];
                                            string header;
                                            bool hasFound = false;

                                            while ((hasFound == false) && (recv(sock, bufferChar, 8192, MSG_NOSIGNAL) > 0)) {
                                                header.append(bufferChar);
                                                size_t headerPos = header.find("\r\n\r\n");
                                                if (headerPos != string::npos) {
                                                    hasFound = true;
                                                    Utils::stringToLowerCase(header);
                                                    LoggerUtil_debug(logger, "header:\n" << header.substr(0, headerPos).c_str());
                                                    headerPos = header.find("content-type:");
                                                    if (headerPos != string::npos) {
                                                        string contentType = header.substr(headerPos + 13);
                                                        type = contentType.substr(0, contentType.find("\r\n")).c_str();
                                                    }
                                                }
                                            }
                                        }

                                        shutdown(sock, SHUT_RDWR);
                                        close(sock);
                                    } else {
                                        LoggerUtil_debug(logger, "socket not connected!");
                                    }
                                }

                                LoggerUtil_debug(logger, "content type: " << type.c_str());

                                // streaming
                                if (type.find("image") != string::npos) {
                                    imgProvider = GraphicsHandler::createImageProvider((char*) (uri.c_str()));
                                } else if ((type.find("audio") != string::npos) || (type.find("video") != string::npos)) {
                                    avProvider = GraphicsHandler::createAVProvider((char*) (uri.c_str()));
                                } else {
                                    if ((imgProvider = GraphicsHandler::createImageProvider((char*) (uri.c_str()))) == NULL)
                                        avProvider = GraphicsHandler::createAVProvider((char*) (uri.c_str()));
                                }

                                // cache
                                if ((avProvider == NULL) && (imgProvider == NULL)) {
                                    if (uri.compare(prevURI.c_str()) == 0) {
                                        LoggerUtil_debug(logger, "Cache");
                                        LoggerUtil_debug(logger, "Host: " << Utils::getHostURL(uri.c_str()) << " Port: " << Utils::getPortURL(uri.c_str()));
                                        
                                        if ( ((imgProvider = GraphicsHandler::createImageProvider((char*) (prevFile.c_str()))) != NULL) ||
                                            ((avProvider = GraphicsHandler::createAVProvider((char*) (prevFile.c_str()))) != NULL) ) {
                                            tmpFile = prevFile;
                                            prevFile = "";
                                        }
                                    }
                                }

                                if (Utils::isAbsoluteURL(uri)) {

                                    // buffer
                                    if ((avProvider == NULL) && (imgProvider == NULL)) {
                                        IDirectFB* dfb = (IDirectFB*) GraphicsHandler::getGraphicsRoot();

                                        if (dfb->CreateDataBuffer(dfb, NULL, (IDirectFBDataBuffer**) & buffer) == DFB_OK) {
                                            streamingThread = new pthread_t;
                                            streamingData streamingPtr;
                                            streamingPtr.uri = uri.c_str();
                                            streamingPtr.playerBuffer = (IDirectFBDataBuffer*) buffer;
                                            pthread_create(streamingThread, NULL, bufferFeeder, &streamingPtr);
                                            pthread_detach(*streamingThread);

                                            if (((IDirectFBDataBuffer*) buffer)->CreateImageProvider((IDirectFBDataBuffer*) buffer, (IDirectFBImageProvider**) & imgProvider) != DFB_OK)
                                                ((IDirectFBDataBuffer*) buffer)->CreateVideoProvider((IDirectFBDataBuffer*) buffer, (IDirectFBVideoProvider**) & avProvider);
                                        }
                                    }

                                    // local
                                    if ((avProvider == NULL) && (imgProvider == NULL)) {
                                        LoggerUtil_debug(logger, "Local");
                                        LoggerUtil_debug(logger, "Host: " << Utils::getHostURL(uri.c_str()) << " Port: " << Utils::getPortURL(uri.c_str()));

                                        int sock = -1;
                                        if ((sock = Utils::socketConnect(Utils::getHostURL(uri.c_str()), Utils::getPortURL(uri.c_str()))) >= 0) {

                                            LoggerUtil_debug(logger, "socket connected!");

                                            timeval tmvl;
                                            tmvl.tv_sec = 2;
                                            tmvl.tv_usec = 500;
                                            const int flag = 1;

                                            setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof (flag));
                                            setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tmvl, sizeof (tmvl));

                                            string cmd("GET ");
                                            cmd.append(uri.substr(uri.substr(uri.find("://") + 3).find('/') + uri.find("://") + 3));
                                            cmd.append(" HTTP/1.1\r\n");

                                            cmd.append("User-Agent: PlayerHandler\r\n");

                                            cmd.append("Host: ");
                                            cmd.append(uri.substr(uri.find("://") + 3).substr(0, uri.substr(uri.find("://") + 3).find('/')));
                                            cmd.append("\r\n");

                                            cmd.append("Connection: Close\r\n");
                                            cmd.append("\r\n");

                                            if (Utils::socketSend(sock, cmd.c_str(), cmd.length()) == cmd.length()) {
                                                char bufferChar[8192];
                                                ssize_t responseLength = 0;
                                                off_t totalReceived = 0;
                                                off_t total = 0;
                                                char* filePtr = tmpnam(NULL);
                                                string header;
                                                bool hasFound = false;

                                                if (filePtr != NULL) {
                                                    tmpFile = filePtr;
                                                    int fd = open(tmpFile.c_str(), O_CREAT | O_RDWR);
                                                    if (fd != -1) {
                                                        while ((responseLength = recv(sock, bufferChar, 8192, MSG_NOSIGNAL)) > 0) {
                                                            if (hasFound == true) {
                                                                if (write(fd, bufferChar, responseLength) == -1)
                                                                    break;

                                                                totalReceived += responseLength;
                                                            } else {
                                                                header.append(bufferChar);
                                                                size_t headerPos = header.find("\r\n\r\n");
                                                                if (headerPos != string::npos) {
                                                                    hasFound = true;
                                                                    if (write(fd, &bufferChar[headerPos + 4], responseLength - headerPos - 4) == -1)
                                                                        break;

                                                                    totalReceived = (responseLength - headerPos - 4);
                                                                    Utils::stringToLowerCase(header);
                                                                    LoggerUtil_debug(logger, "header:\n" << header.substr(0, headerPos).c_str());
                                                                    headerPos = header.find("content-length:");
                                                                    if (headerPos != string::npos) {
                                                                        stringstream ssTmp;
                                                                        string contentLength = header.substr(headerPos + 15);
                                                                        ssTmp << contentLength.substr(0, contentLength.find("\r\n")).c_str();
                                                                        ssTmp >> total;
                                                                        LoggerUtil_debug(logger, "total: " << total);
                                                                        if (total > 0) {
                                                                            progressBarData data;
                                                                            data.totalReceived = &totalReceived;
                                                                            data.total = total;

                                                                            pthread_t progressThread;
                                                                            pthread_create(&progressThread, NULL, progressBarThread, &data);
                                                                            pthread_detach(progressThread);
                                                                        }
                                                                    }
                                                                }
                                                            }
                                                        }
                                                        close(fd);

                                                        LoggerUtil_debug(logger, "temporary file: " << tmpFile.c_str());

                                                        if ((imgProvider = GraphicsHandler::createImageProvider((char*) (tmpFile.c_str()))) == NULL)
                                                            avProvider = GraphicsHandler::createAVProvider((char*) (tmpFile.c_str()));
                                                    }
                                                }
                                            }

                                            shutdown(sock, SHUT_RDWR);
                                            close(sock);
                                        } else {
                                            LoggerUtil_debug(logger, "socket not connected!");
                                        }
                                    }

                                }

                                LoggerUtil_debug(logger, "create content provider");

                                DFBSurfaceDescription dsc;

                                if (avProvider != NULL) {
                                    ((IDirectFBVideoProvider*) avProvider)->GetSurfaceDescription((IDirectFBVideoProvider*) avProvider, &dsc);
                                } else if (imgProvider != NULL) {
                                    ((IDirectFBImageProvider*) imgProvider)->GetSurfaceDescription((IDirectFBImageProvider*) imgProvider, &dsc);
                                }

                                LoggerUtil_debug(logger, "get content description");

                                if ((avProvider != NULL) || (imgProvider != NULL)) {
                                    width = dsc.width;
                                    height = dsc.height;
                                    if (type.find("audio") == string::npos)
                                        hasVisualContent = true;
                                } else {
                                    IXML_Document* xml = ixmlParseBuffer(metadata.c_str());
                                    if (xml != NULL) {
                                        IXML_NodeList* resList = ixmlDocument_getElementsByTagName(xml, "res");
                                        int listLength = 0;
                                        if ( (resList != NULL) && ((listLength = ixmlNodeList_length(resList)) > 0) ) {
                                            int indexUri = -1;
                                            // varre a lista de URIs a procura da URI atual
                                            for(int index = 0; index < listLength; index++) {
                                                IXML_Node* resNode = ixmlNodeList_item(resList, index);
                                                if( (resNode != NULL) && (uri.compare(Utils::getNodeValue(resNode)) == 0) )
                                                    indexUri = index;
                                            }
                                            // caso nao encontre a URI atual na lista, pega a primeira da lista
                                            if (indexUri == -1) {
                                                IXML_Node* resNode = ixmlNodeList_item(resList, 0);
                                                if (resNode != NULL) {
                                                    uri = Utils::getNodeValue(resNode);
                                                    prepareMedia();
                                                }
                                            } else {
                                                // caso encontre a URI atual na lista, pega a seguinte
                                                int index = indexUri + 1;
                                                if (index < listLength) {
                                                    IXML_Node* resNode = ixmlNodeList_item(resList, index);
                                                    if (resNode != NULL) {
                                                        uri = Utils::getNodeValue(resNode);
                                                        prepareMedia();
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }

                            }

                            /* encerra a janela informativa: "Carregando..." */
                            if (font != NULL){
                                ((IDirectFBFont*) font)->Release((IDirectFBFont*) font);
                                font = NULL;
                            }
                            if (win != NULL) {
                                ((IDirectFBWindow*) win)->Destroy((IDirectFBWindow*) win);
                                ((IDirectFBWindow*) win)->Release((IDirectFBWindow*) win);
                                win = NULL;
                                surface = NULL;
                            }
                            mediaReady = true;
                        }

                        void avRenderCallback(void* surface){
                            IDirectFBSurface* s = (IDirectFBSurface*) surface;
                            s->Blit(s, s, NULL, 0, 0);
                            s->Flip(s, NULL, (DFBSurfaceFlipFlags) DSFLIP_BLIT);
                        }

                        DIRenderCallbackResult imageRenderCallback(DFBRectangle* rect, void* surface) {
                            IDirectFBSurface* image = (IDirectFBSurface*) surface;
                            image->Blit(image, image, rect, 0, 0);
                            image->Flip(image, NULL, (DFBSurfaceFlipFlags) DSFLIP_WAITFORSYNC);
                            return DIRCR_OK;
                        }

                        void* createPlayerThreadProc(void *param) {
                            PlayerHandler *thread = (PlayerHandler *) param;
                            thread->run();
                            return NULL;
                        }
                        
                        bool PlayerHandler::play() {
                            LoggerUtil_info(logger, "Entrando no método play()!");

                            if (mediaReady == false)
                                return false;

                            if (surface == NULL)
                                initializeWindow();

                            if ((avProvider == NULL) && (imgProvider == NULL)) {
                                DFBFontDescription fdsc;
                                fdsc.flags = (DFBFontDescriptionFlags) (DFDESC_HEIGHT | DFDESC_ATTRIBUTES);
                                fdsc.attributes = DFFA_NONE;
                                fdsc.height = 24;
                                IDirectFB* dfb = (IDirectFB*) GraphicsHandler::getGraphicsRoot();
                                if (dfb->CreateFont(dfb, NULL, &fdsc, (IDirectFBFont**) &font) == DFB_OK) {
                                    IDirectFBSurface* surfacePtr = (IDirectFBSurface*) surface;
                                    surfacePtr->SetColor(surfacePtr, 0xdd, 0xdd, 0xdd, 0xff);
                                    surfacePtr->SetFont(surfacePtr, (IDirectFBFont*) font);

                                    string text = "Nao ha suporte!";
                                    int textWidth = 0;
                                    ((IDirectFBFont*)font)->GetStringWidth((IDirectFBFont*)font, text.c_str(), -1, &textWidth);

                                    surfacePtr->DrawString(surfacePtr, text.c_str(), -1, ((GraphicsHandler::getScreenWidth() * 20 / 100) - textWidth)/2, ((GraphicsHandler::getScreenHeight() * 15 / 100) - (0.6 * fdsc.height))/2, DSTF_TOPLEFT);
                                    surfacePtr->Flip(surfacePtr, NULL, (DFBSurfaceFlipFlags) DSFLIP_WAITFORSYNC);
                                }

                                return false;
                            }

                            if (avProvider != NULL) {
                                if (((IDirectFBVideoProvider*) avProvider)->PlayTo((IDirectFBVideoProvider*) avProvider, (IDirectFBSurface*) surface, NULL, avRenderCallback, surface) != DFB_OK)
                                    return false;
                            } else if (imgProvider != NULL) {
                                ((IDirectFBImageProvider*) imgProvider)->SetRenderCallback((IDirectFBImageProvider*) imgProvider, imageRenderCallback, surface);
                                if (((IDirectFBImageProvider*) imgProvider)->RenderTo((IDirectFBImageProvider*) imgProvider, (IDirectFBSurface*) surface, NULL) != DFB_OK)
                                    return false;
                            }

                            setRunning(true);

                            if (pthread_create(&thread, NULL, createPlayerThreadProc, this) != 0) {
                                setRunning(false);
                            }
                            pthread_detach(thread);
                            
                            return true;
                        }

                        bool PlayerHandler::stop() {
                            LoggerUtil_info(logger, "Entrando no método stop()!");

                            if (mediaReady == false)
                                return false;
                            
                            bool result = true;

                            if (avProvider != NULL) {
                                if (((IDirectFBVideoProvider*) avProvider)->Stop((IDirectFBVideoProvider*) avProvider) != DFB_OK) {
                                    result = false;
                                }
                                ((IDirectFBVideoProvider*) avProvider)->Release((IDirectFBVideoProvider*) avProvider);
                                avProvider = NULL;
                            } else if (imgProvider != NULL) {
                                ((IDirectFBImageProvider*) imgProvider)->Release((IDirectFBImageProvider*) imgProvider);
                                imgProvider = NULL;
                            }

                            if (streamingThread != NULL){
                                pthread_cancel(*streamingThread);
                                pthread_detach(*streamingThread);
                                delete streamingThread;
                                streamingThread = NULL;
                            }

                            if (buffer != NULL) {
                                ((IDirectFBDataBuffer*) buffer)->Release((IDirectFBDataBuffer*) buffer);
                                buffer = NULL;
                            }

                            if (font != NULL){
                                ((IDirectFBFont*) font)->Release((IDirectFBFont*) font);
                            }

                            if (win != NULL) {
                                ((IDirectFBWindow*) win)->Destroy((IDirectFBWindow*) win);
                                ((IDirectFBWindow*) win)->Release((IDirectFBWindow*) win);
                                win = NULL;
                                surface = NULL;
                            }

                            if (tmpFile.size() != 0) {
                                if (prevFile.size() != 0)
                                    remove(prevFile.c_str());
                                prevFile = tmpFile;
                                tmpFile = "";
                            }

                            if (isRunning() == true) {
                                pthread_cancel(thread);
                                pthread_detach(thread);
                                setRunning(false);
                            }

                            return result;
                        }

                        bool PlayerHandler::pause() {
                            LoggerUtil_info(logger, "Entrando no método pause()!");

                            if (mediaReady == false)
                                return false;
                            
                            if ((avProvider != NULL) && (((IDirectFBVideoProvider*) avProvider)->Stop((IDirectFBVideoProvider*) avProvider) != DFB_OK)) {
                                return false;
                            } else {
                                return true;
                            }
                        }

                        bool PlayerHandler::seek(double pos) {
                            LoggerUtil_info(logger, "Entrando no método seek(pos)!");
                            LoggerUtil_debug(logger, "pos: " << pos);

                            if (mediaReady == false)
                                return false;
                            
                            if ((avProvider != NULL) && (((IDirectFBVideoProvider*) avProvider)->SeekTo((IDirectFBVideoProvider*) avProvider, pos) != DFB_OK)) {
                                return false;
                            } else {
                                return true;
                            }
                        }

                        bool PlayerHandler::setVolume(int v) {
                            LoggerUtil_info(logger, "Entrando no método setVolume(v)!");
                            LoggerUtil_debug(logger, "v: " << v);

                            if (mediaReady == false)
                                return false;
                            
                            if ((v < 0) || (v > 100)) {
                                return false;
                            }

                            float volume = v / 100.0;

                            if ((avProvider != NULL) && (((IDirectFBVideoProvider*) avProvider)->SetVolume((IDirectFBVideoProvider*) avProvider, volume) != DFB_OK)) {
                                return false;
                            } else {
                                return true;
                            }
                        }

                        int PlayerHandler::getVolume() {
                            LoggerUtil_info(logger, "Entrando no método getVolume()!");

                            if (mediaReady == false)
                                return 100;

                            float value;

                            if ((avProvider != NULL) && (((IDirectFBVideoProvider*) avProvider)->GetVolume((IDirectFBVideoProvider*) avProvider, &value) == DFB_OK)) {
                                return value * 100;
                            } else {
                                return 100;
                            }
                        }

                        double PlayerHandler::getMediaTimePosition() {
                            LoggerUtil_info(logger, "Entrando no método getMediaTimePosition()!");

                            if (mediaReady == false)
                                return 0;
                            
                            double pos;

                            if ((avProvider != NULL) && (((IDirectFBVideoProvider*) avProvider)->GetPos((IDirectFBVideoProvider*) avProvider, &pos) != DFB_EOF)) {
                                LoggerUtil_debug(logger, "position: " << pos);
                                return pos;
                            } else {
                                return 0;
                            }
                        }

                        double PlayerHandler::getMediaDuration() {
                            LoggerUtil_info(logger, "Entrando no método getMediaDuration()!");

                            if (mediaReady == false)
                                return 0;
                            
                            double duration;

                            if ((avProvider != NULL) && (((IDirectFBVideoProvider*) avProvider)->GetLength((IDirectFBVideoProvider*) avProvider, &duration) == DFB_OK)) {
                                LoggerUtil_debug(logger, "duration: " << duration);
                                return duration;
                            } else {
                                return 0;
                            }
                        }

                        bool PlayerHandler::setBrightness(int v) {
                            LoggerUtil_info(logger, "Entrando no método setBrightness(v)!");

                            if (mediaReady == false)
                                return false;

                            if ((v < 0) || (v > 100)) {
                                return false;
                            }

                            DFBColorAdjustment adj;

                            if ((avProvider != NULL) && (((IDirectFBVideoProvider*) avProvider)->GetColorAdjustment((IDirectFBVideoProvider*) avProvider, &adj) == DFB_OK)) {
                                float brightness = v / 100.0;

                                adj.brightness = brightness * 0xFFFF;

                                if (((IDirectFBVideoProvider*) avProvider)->SetColorAdjustment((IDirectFBVideoProvider*) avProvider, &adj) == DFB_OK)
                                    return true;
                            }

                            return false;
                        }

                        int PlayerHandler::getBrightness() {
                            LoggerUtil_info(logger, "Entrando no método getBrightness()!");

                            if (mediaReady == false)
                                return 50;

                            DFBColorAdjustment adj;

                            if ((avProvider != NULL) && (((IDirectFBVideoProvider*) avProvider)->GetColorAdjustment((IDirectFBVideoProvider*) avProvider, &adj) == DFB_OK)) {
                                float value;

                                value = adj.brightness / 0xFFFF;

                                return value * 100;
                            } else {
                                return 50;
                            }
                        }

                        bool PlayerHandler::setContrast(int v) {
                            LoggerUtil_info(logger, "Entrando no método setContrast(v)!");

                            if (mediaReady == false)
                                return false;

                            if ((v < 0) || (v > 100)) {
                                return false;
                            }

                            DFBColorAdjustment adj;

                            if ((avProvider != NULL) && (((IDirectFBVideoProvider*) avProvider)->GetColorAdjustment((IDirectFBVideoProvider*) avProvider, &adj) == DFB_OK)) {
                                float contrast = v / 100.0;

                                adj.contrast = contrast * 0xFFFF;

                                if (((IDirectFBVideoProvider*) avProvider)->SetColorAdjustment((IDirectFBVideoProvider*) avProvider, &adj) == DFB_OK)
                                    return true;
                            }

                            return false;
                        }

                        int PlayerHandler::getContrast() {
                            LoggerUtil_info(logger, "Entrando no método getContrast()!");

                            if (mediaReady == false)
                                return 50;

                            DFBColorAdjustment adj;

                            if ((avProvider != NULL) && (((IDirectFBVideoProvider*) avProvider)->GetColorAdjustment((IDirectFBVideoProvider*) avProvider, &adj) == DFB_OK)) {
                                float value;

                                value = adj.contrast / 0xFFFF;

                                return value * 100;
                            } else {
                                return 50;
                            }
                        }

                        void PlayerHandler::run() {
                            LoggerUtil_info(logger, "Entrando no método run()!");

                            while ((isRunning() == true) && (avProvider != NULL)) {
                                pthread_testcancel();
                                
                                DFBVideoProviderStatus status;
                                
                                Utils::wait(1000);

                                ((IDirectFBVideoProvider*) avProvider)->GetStatus((IDirectFBVideoProvider*) avProvider, &status);
                                if (status == DVSTATE_FINISHED)
                                    stop();
                            }

                        }

                    }
                }
            }
        }
    }
}
