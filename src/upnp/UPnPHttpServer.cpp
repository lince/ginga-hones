#include "honesconfig.h"

#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <sstream>
#include <new>
#include "util/Utils.h"
#include "upnp/UPnPHttpServer.h"
using namespace std;
using std::bad_alloc;
using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        static void* HttpServerThreadProc(void *param) {
                            UPnPHttpServer *thread = (UPnPHttpServer*) param;
                            thread->run();
                            return NULL;
                        }

                        UPnPHttpServer* UPnPHttpServer::instance = NULL;
                        Vector* UPnPHttpServer::listeners = NULL;

                        UPnPHttpServer::UPnPHttpServer() {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnphttpserver");
                            LoggerUtil_info(logger, "Entrando no construtor UPnPHttpServer()!");

                            setRunning(false);
                            setPort(0);
                            setServerAddress("");
                            listeners = new Vector();
                            serverSock = -1;
                        }

                        UPnPHttpServer::~UPnPHttpServer(){
                            LoggerUtil_info(logger, "Entrando no destrutor ~UPnPHttpServer()!");
                            if (listeners != NULL){
                                delete listeners;
                                listeners = NULL;
                            }
                        }

                        UPnPHttpServer* UPnPHttpServer::getInstance() {
                            if(instance == NULL){
                                instance = new UPnPHttpServer();
                            }
                            return instance;
                        }

                        void UPnPHttpServer::deleteInstance() {
                            if(instance != NULL){
                                delete instance;
                                instance = NULL;
                            }
                        }

                        bool UPnPHttpServer::addUPnPHttpListener(UPnPHttpListener* listener) {
                            return listeners->add(listener);
                        }

                        bool UPnPHttpServer::removeUPnPHttpListener(UPnPHttpListener* listener) {
                            return listeners->remove(listener);
                        }

                        void UPnPHttpServer::run() {
                            LoggerUtil_info(logger, "Entrando no metodo run");

                            serverSock = Utils::socketServer(ip.c_str(), &port);

                            LoggerUtil_debug(logger, "serverSock: " << serverSock);

                            if(serverSock >= 0){
                                while(isRunning() == true){
                                    pthread_testcancel();
                                    
                                    int newSock = accept(serverSock, NULL, NULL);

                                    LoggerUtil_debug(logger, "newSock: " << newSock);

                                    if(newSock >= 0){
                                        pthread_t sockThread;
                                        pthread_create(&sockThread, NULL, serve, (void *) newSock);
                                        pthread_detach(sockThread);
                                    }
                                }
                            }
                        }

                        bool UPnPHttpServer::isRunning() {
                            return running;
                        }

                        bool UPnPHttpServer::start() {
                            if (isRunning() == false) {
                                setRunning(true);
                                if (pthread_create(&thread, NULL, HttpServerThreadProc, this) != 0) {
                                    setRunning(false);
                                }
                                pthread_detach(thread);
                                return true;
                            } else {
                                return false;
                            }
                        }

                        bool UPnPHttpServer::stop() {
                            if (isRunning() == true) {
                                pthread_cancel(thread);
                                pthread_detach(thread);
                                setRunning(false);
                                close(serverSock);
                                return true;
                            } else {
                                return false;
                            }
                        }

                        void UPnPHttpServer::setPort(int port) {
                            this->port = port;
                        }

                        int UPnPHttpServer::getPort() {
                            if (port == 0)
                                start();

                            int retry = 0;
                            while ((port == 0) && (retry <= 10)){
                                Utils::wait(1000);
                                retry++;
                            }

                            return port;
                        }

                        void UPnPHttpServer::setServerAddress(const char* ip){
                            this->ip = ip;
                        }

                        void UPnPHttpServer::setRunning(bool running) {
                            this->running = running;
                        }

                        void* UPnPHttpServer::serve(void* param) {
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnphttpserver");
                            LoggerUtil_info(logger, "Entrando no metodo serve");

                            int newSock = (int) param;
                            timeval rcvTimeOut, sndTimeOut;
                            rcvTimeOut.tv_sec=2;
                            rcvTimeOut.tv_usec=500;
                            sndTimeOut.tv_sec=2;
                            sndTimeOut.tv_usec=500;
                            const int flag = 1;
                            int clientResponse = 1;

                            if ( (setsockopt(newSock, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) >= 0) && (setsockopt(newSock, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag)) >= 0) && (setsockopt(newSock, SOL_SOCKET, SO_RCVTIMEO, &rcvTimeOut, sizeof(rcvTimeOut)) >= 0) && (setsockopt(newSock, SOL_SOCKET, SO_SNDTIMEO, &sndTimeOut, sizeof(sndTimeOut)) >= 0) ){
                                off_t fileLength = (off_t) 0;
                                time_t lastModified = time(NULL);
                                bool isDirectory = false;
                                bool isReadable = false;
                                bool newPath = false;
                                char* contentType = NULL;
                                char* absolutePath = NULL;
                                char* contentFeatures = NULL;
                                string method, path, protocol;
                                UPnPHttpListener* listener = NULL;

                                while (clientResponse != 0) {
                                    string input;
                                    char* bufferChar = new char[2];
                                    bufferChar[1] = '\0';

                                    while ((clientResponse = recv(newSock, bufferChar, 1, MSG_NOSIGNAL)) > 0) {
                                        input.append(bufferChar);
                                    }

                                    LoggerUtil_debug(logger, "clientResponse: " << clientResponse << "\nRequisicao recebida:\n" << input.c_str());

                                    char* tmp = strtok((char*) input.c_str(), " ");
                                    if ((tmp != NULL) && (method.compare(tmp) != 0)){
                                        method = tmp;
                                    }

                                    tmp = strtok(NULL, " ");
                                    if ((tmp != NULL) && (path.compare(tmp) != 0)){
                                        path = tmp;
                                        newPath = true;
                                    }

                                    tmp = strtok(NULL, "\r");
                                    if ((tmp != NULL) && (protocol.compare(tmp) != 0)){
                                        protocol = tmp;
                                    }

                                    if ((method.compare("GET") == 0) || (method.compare("HEAD") == 0)) {
                                        if (newPath == true){
                                            newPath = false;
                                            if (contentType != NULL)
                                                free(contentType);
                                            if (absolutePath != NULL)
                                                free(absolutePath);
                                            if (contentFeatures != NULL)
                                                free(contentFeatures);

                                            int size = listeners->size();
                                            for (int i = 0; i < size; i++) {
                                                listener = (UPnPHttpListener*) listeners->get(i);
                                                if ((listener != NULL) && (listener->httpGetInfo(path.c_str(), &fileLength, &lastModified, &isDirectory, &isReadable, &contentType, &absolutePath) == true)) {
                                                    i = size;
                                                } else {
                                                    listener = NULL;
                                                }
                                            }
                                            contentFeatures = Utils::getDLNAAdditionalInfo(absolutePath);
                                        }

                                        if ((listener != NULL) && (isReadable == true)) {
                                            if (method.compare("HEAD") == 0) {
                                                const char* response = getHeader("200", "OK", contentType, NULL, 0, lastModified, contentFeatures, protocol.c_str());
                                                Utils::socketSend(newSock, response, strlen(response));

                                                LoggerUtil_debug(logger, response);
                                            }

                                            if (method.compare("GET") == 0) {
                                                UPnPHttpFileHandle* fh = listener->httpOpen(path.c_str(), READ);

                                                if (fh != NULL) {
                                                    off_t beginOffset = (off_t) 0;
                                                    off_t endOffset = (off_t) (32 * 1024) - (off_t) 1;

                                                    size_t pos = input.find("Range: bytes");
                                                    if (pos != string::npos) {
                                                        size_t pos1 = input.find("-", pos + 12);
                                                        size_t length = pos1 - pos - 13;
                                                        beginOffset = atol(input.substr(pos + 13, length).c_str());
                                                        endOffset = atol(input.substr(pos1 + 1).c_str());

                                                        if (endOffset < beginOffset) {
                                                            endOffset = beginOffset + (32 * 1024) - (off_t) 1;
                                                        }
                                                    }

                                                    off_t bufferLength = endOffset - beginOffset + (off_t) 1;

                                                    char* buffer = NULL;

                                                    try{
                                                        if(bufferLength > MAX_BUFFER) {
                                                            endOffset = beginOffset + (off_t) MAX_BUFFER - (off_t) 1;
                                                            bufferLength = endOffset - beginOffset + (off_t) 1;
                                                        }
                                                        buffer = new char[bufferLength];
                                                    }catch(bad_alloc &memoryAllocationException){
                                                        endOffset = beginOffset + (32 * 1024) - (off_t) 1;
                                                        bufferLength = endOffset - beginOffset + (off_t) 1;
                                                        buffer = new char[bufferLength];
                                                    }

                                                    setsockopt(newSock, SOL_SOCKET, SO_SNDBUF, &bufferLength, sizeof (bufferLength));

                                                    if ((pos != string::npos) && (beginOffset < fileLength)) {
                                                        listener->httpSeek(fh, beginOffset, SEEK_SET);

                                                        off_t responseLength = listener->httpRead(fh, buffer, bufferLength);

                                                        if (responseLength > 0) {
                                                            ostringstream range;
                                                            range << beginOffset;
                                                            range << "-";
                                                            range << (beginOffset + responseLength - 1);
                                                            range << "/";
                                                            range << fileLength;

                                                            const char* response = getHeader("206", "Partial Content", contentType, range.str().c_str(), responseLength, lastModified, contentFeatures, protocol.c_str());
                                                            Utils::socketSend(newSock, response, strlen(response));

                                                            LoggerUtil_debug(logger, response);

                                                            Utils::socketSend(newSock, buffer, responseLength);
                                                        } else {
                                                            const char* response = getHeader("500", "Internal Server Error", "text/html", NULL, strlen("Internal Server Error"), time(NULL), NULL, protocol.c_str());
                                                            Utils::socketSend(newSock, response, strlen(response));

                                                            LoggerUtil_debug(logger, response);

                                                            Utils::socketSend(newSock, "Internal Server Error", strlen("Internal Server Error"));
                                                        }
                                                    } else {
                                                        listener->httpSeek(fh, (off_t) 0, SEEK_SET);

                                                        off_t responseLength = listener->httpRead(fh, buffer, bufferLength);

                                                        if (responseLength > 0) {
                                                            const char* response = getHeader("200", "OK", contentType, NULL, fileLength, lastModified, contentFeatures, protocol.c_str());
                                                            Utils::socketSend(newSock, response, strlen(response));

                                                            LoggerUtil_debug(logger, response);

                                                            off_t totalLength = (off_t) 0;
                                                            do {
                                                                totalLength += responseLength;

                                                                LoggerUtil_debug(logger, "bytes read: " << totalLength << " / " << fileLength);

                                                                Utils::socketSend(newSock, buffer, responseLength);
                                                            } while ((responseLength = listener->httpRead(fh, buffer, bufferLength)) > 0);
                                                        } else {
                                                            const char* response = getHeader("500", "Internal Server Error", "text/html", NULL, strlen("Internal Server Error"), time(NULL), NULL, protocol.c_str());
                                                            Utils::socketSend(newSock, response, strlen(response));

                                                            LoggerUtil_debug(logger, response);

                                                            Utils::socketSend(newSock, "Internal Server Error", strlen("Internal Server Error"));
                                                        }
                                                    }

                                                    listener->httpClose(fh);

                                                    delete[] buffer;
                                                }
                                            }
                                        } else {
                                            const char* response = getHeader("404", "Not Found", "text/html", NULL, strlen("Not Found"), lastModified, NULL, protocol.c_str());
                                            Utils::socketSend(newSock, response, strlen(response));

                                            LoggerUtil_debug(logger, response);

                                            Utils::socketSend(newSock, "Not Found", strlen("Not Found"));
                                        }
                                    } else if (method.compare("POST") == 0) {
                                        LoggerUtil_debug(logger, input.c_str());
                                    } else if (method.length() > 0) {
                                        const char* response = getHeader("501", "Method Not Implemented", "text/html", NULL, strlen("Method Not Implemented"), time(NULL), NULL, protocol.c_str());
                                        Utils::socketSend(newSock, response, strlen(response));

                                        LoggerUtil_debug(logger, response);

                                        Utils::socketSend(newSock, "Method Not Implemented", strlen("Method Not Implemented"));
                                    }

                                    delete[] bufferChar;
                                }

                                if (contentType != NULL)
                                    free(contentType);
                                if (absolutePath != NULL)
                                    free(absolutePath);
                                if (contentFeatures != NULL)
                                    free(contentFeatures);
                            }

                            shutdown(newSock,SHUT_RDWR);
                            close(newSock);

                            LoggerUtil_debug(logger, "close(newSock)");

                            return NULL;
                        }

                        char* UPnPHttpServer::getHeader(const char* statusCode, const char* statusStr, const char* contentType, const char* range, off_t contentLength, time_t lastModified, const char* contentFeatures, const char* protocol){
                            ostringstream header;
                            time_t t = time(NULL);
                            char timeNow[128];
                            char timeLast[128];
                            strftime(timeNow, sizeof(timeNow), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&t));
                            strftime(timeLast, sizeof(timeLast), "%a, %d %b %Y %H:%M:%S GMT", gmtime(&lastModified));

                            header << protocol << " " << statusCode << " " << statusStr << "\r\n";
                            header << "Date: " << timeNow << "\r\n";
                            header << "Server: UPnPHttpServer/0.0.0 UPnP/1.0 DLNADOC/1.50" << "\r\n";
                            header << "Last-Modified: " << timeLast << "\r\n";
                            header << "Cache-Control: no-cache" << "\r\n";
                            header << "Accept-Ranges: bytes" << "\r\n";
                            if(range != NULL)
                                header << "Content-Range: bytes " << range << "\r\n";
                            if (contentLength > 0)
                                header << "Content-length: " << contentLength << "\r\n";
                            if (contentType != NULL)
                                header << "Content-type: " << contentType << "\r\n";
                            header << "Connection: keep-alive" << "\r\n";
                            header << "transferMode.dlna.org: Streaming" << "\r\n";
                            if (contentFeatures != NULL)
                                header << "contentFeatures.dlna.org: " << contentFeatures << "\r\n";
                            else
                                header << "contentFeatures.dlna.org: " << Utils::getDLNAAdditionalInfo(NULL) << "\r\n";
                            header << "\r\n";

                            return strdup(header.str().c_str());
                        }

                    }
                }
            }
        }
    }
}
