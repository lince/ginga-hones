#include "honesconfig.h"

#include <pthread.h>
#include <upnp/ixml.h>
#include "mediacenter/TVMediaCenter.h"
#include "ui/GraphicsHandler.h"
#include "upnp/UPnPAction.h"
#include "upnp/UPnPArgument.h"
#include "upnp/UPnPDevice.h"
#include "upnp/UPnPService.h"
#include "upnp/UPnPDeviceList.h"
#ifdef HAVE_GINGACCCM
#include <ginga/cm/IComponentManager.h>
using namespace ::br::pucrio::telemidia::ginga::core::cm;
#else
#include <ginga/io/InputManager.h>
using namespace ::br::pucrio::telemidia::ginga::core::io;
#endif

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediacenter {

                        IInputManager* TVMediaCenter::im = NULL;
                        
                        TVMediaCenter::TVMediaCenter(){
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.mediacenter.tvmediacenter");
                            LoggerUtil_info(logger, "Entrando no construtor TVMediaCenter()!");

                            hasBeenInit = false;
                            initWindow = NULL;
                            serverWindow = NULL;
                            audioWindow = NULL;
                            videoWindow = NULL;
                            imageWindow = NULL;
                            mediaWindow = NULL;
                            clientWindow = NULL;
                            playbackWindow = NULL;
                            startingIndex = 0;
                            totalMatches = -1;
                            upnpParentID = "0";
                            playbackWindowVisibleCount = 5;
                            playbackRunning = false;
                            upnpCtrl = UPnPDeviceController::getInstance();
                            player = PlayerHandler::PlayerHandlerFactory();

#ifdef HAVE_GINGACCCM
                            IComponentManager* cm = IComponentManager::getCMInstance();

                            LoggerUtil_debug(logger, "ComponentManager recuperado!");

                            if(cm != NULL)
                                im = ((InputManagerCreator*) (cm->getObject("InputManager")))();

                            LoggerUtil_debug(logger, "InputManager recuperado!");

                            if (im != NULL) {
                                set<int>* evs = new set<int>;
                                evs->insert(CodeMap::KEY_CURSOR_DOWN);
                                evs->insert(CodeMap::KEY_CURSOR_LEFT);
                                evs->insert(CodeMap::KEY_CURSOR_RIGHT);
                                evs->insert(CodeMap::KEY_CURSOR_UP);
                                evs->insert(CodeMap::KEY_ENTER);
                                evs->insert(CodeMap::KEY_OK);
                                evs->insert(CodeMap::KEY_RED);

                                LoggerUtil_debug(logger, "Mapeamento de teclas realizado!");

                                im->addInputEventListener((IInputEventListener*)this, evs);

                                LoggerUtil_debug(logger, "Mapeamento de teclas registrado!");
                            }
#else
                            im = InputManager::getInstance();
#endif
                        }

                        TVMediaCenter::~TVMediaCenter(){
                            stop();
                            if (initWindow != NULL)
                                delete initWindow;
                            if (serverWindow != NULL)
                                delete serverWindow;
                            if(audioWindow != NULL)
                                delete audioWindow;
                            if(videoWindow != NULL)
                                delete videoWindow;
                            if(imageWindow != NULL)
                                delete imageWindow;
                            if(mediaWindow != NULL)
                                delete mediaWindow;
                            if(clientWindow != NULL)
                                delete clientWindow;
                            if(playbackWindow != NULL)
                                delete playbackWindow;
                        }

                        PlayerHandler* TVMediaCenter::getPlayerHandler(){
                            return player;
                        }

                        bool TVMediaCenter::start(){
                            initComponents();
                            if (initWindow != NULL)
                                initWindow->setVisible(true);
                        }

                        bool TVMediaCenter::stop(){
                            if (initWindow != NULL)
                                initWindow->setVisible(false);
                            if (serverWindow != NULL)
                                serverWindow->setVisible(false);
                            if (mediaWindow != NULL)
                                mediaWindow->setVisible(false);
                            if (clientWindow != NULL)
                                clientWindow->setVisible(false);
                            if (playbackWindow != NULL)
                                playbackWindow->setVisible(false);
                            startingIndex = 0;
                            totalMatches = -1;
                            upnpParentID = "0";
                            playbackRunning = false;
                        }
                        
                        bool TVMediaCenter::userEventReceived(IInputEvent* ev) {
                            const int code = ev->getKeyCode();

                            if (code == CodeMap::KEY_RED){
                                delete this;
                                return true;
                            }
                            
                            Container* container = NULL;
                            SelectableTextArea* currentTextArea = NULL;
                            SelectableTextArea* previousTextArea = NULL;
                            SelectableTextArea* nextTextArea = NULL;

                            if ((initWindow != NULL) && (initWindow->isVisible())) {
                                container = initWindow;
                            } else if ((serverWindow != NULL) && (serverWindow->isVisible())) {
                                container = serverWindow;
                            } else if ((mediaWindow != NULL) && (mediaWindow->isVisible())) {
                                container = mediaWindow;
                            } else if((clientWindow != NULL) && (clientWindow->isVisible())){
                                container = clientWindow;
                            } else if((playbackWindow != NULL) && (playbackWindow->isVisible())){
                                container = playbackWindow;
                            }

                            if (container != NULL){
                                Vector* taList = container->getTextAreaList();
                                int listSize = taList->size();
                                for (int i = 0; i < listSize; i++) {
                                    TextArea* current = (TextArea*) taList->get(i);
                                    try {
                                        if (typeid (*current) == typeid (SelectableTextArea)) {
                                            SelectableTextArea* aux = (SelectableTextArea*) current;
                                            if ((aux->isFocusable() == true) && (aux->isSelected() == true)){

                                                currentTextArea = aux;

                                                int previous = i;
                                                while (previousTextArea == NULL) {
                                                    if (previous > 0) {
                                                        previous -= 1;
                                                    } else {
                                                        previous = listSize - 1;
                                                    }
                                                    current = (TextArea*) taList->get(previous);
                                                    try {
                                                        if (typeid (*current) == typeid (SelectableTextArea)) {
                                                            aux = (SelectableTextArea*) current;
                                                            if(aux->isFocusable() == true)
                                                                previousTextArea = aux;
                                                        }
                                                    } catch (...) {
                                                    }
                                                }

                                                int next = i;
                                                while (nextTextArea == NULL) {
                                                    if (next < (listSize - 1)) {
                                                        next += 1;
                                                    } else {
                                                        next = 0;
                                                    }
                                                    current = (TextArea*) taList->get(next);
                                                    try {
                                                        if (typeid (*current) == typeid (SelectableTextArea)) {
                                                            aux = (SelectableTextArea*) current;
                                                            if(aux->isFocusable() == true)
                                                                nextTextArea = aux;
                                                        }
                                                    } catch (...) {
                                                    }
                                                }
                                                
                                                i = listSize;
                                            }
                                        }
                                    } catch (...) {
                                    }
                                }
                            }

                            if (currentTextArea != NULL) {
                                if ((code == CodeMap::KEY_CURSOR_UP) || (code == CodeMap::KEY_CURSOR_LEFT)) {
                                    currentTextArea->setSelected(false);
                                    previousTextArea->setSelected(true);
                                } else if ((code == CodeMap::KEY_CURSOR_DOWN) || (code == CodeMap::KEY_CURSOR_RIGHT)) {
                                    currentTextArea->setSelected(false);
                                    nextTextArea->setSelected(true);
                                }

                                if ((code == CodeMap::KEY_ENTER) || (code == CodeMap::KEY_OK)) {
                                    if ((initWindow != NULL) && (initWindow->isVisible())) {
                                        if (string("image").compare(currentTextArea->getValue()) == 0) {
                                            mediaClass = "image";
                                            initWindow->setVisible(false);
                                            upnpCtrl->searchAll();
                                            showServerWindow();
                                        } else if (string("video").compare(currentTextArea->getValue()) == 0) {
                                            mediaClass = "video";
                                            initWindow->setVisible(false);
                                            upnpCtrl->searchAll();
                                            showServerWindow();
                                        } else if (string("audio").compare(currentTextArea->getValue()) == 0) {
                                            mediaClass = "audio";
                                            initWindow->setVisible(false);
                                            upnpCtrl->searchAll();
                                            showServerWindow();
                                        } else if (string("exit").compare(currentTextArea->getValue()) == 0) {
                                            stop();
                                        }
                                    } else if ((serverWindow != NULL) && (serverWindow->isVisible())) {
                                        if (string("back").compare(currentTextArea->getValue()) == 0) {
                                            serverWindow->setVisible(false);
                                            initWindow->setVisible(true);
                                        } else if (string("up").compare(currentTextArea->getValue()) == 0) {
                                            itemListIndex--;
                                            showItemList(serverWindow);
                                        } else if (string("down").compare(currentTextArea->getValue()) == 0) {
                                            itemListIndex++;
                                            showItemList(serverWindow);
                                        } else {
                                            serverUDN = currentTextArea->getValue();
                                            LoggerUtil_debug(logger, "serverUDN: " << serverUDN.c_str());
                                            serverWindow->setVisible(false);
                                            showMediaWindow("0");
                                        }
                                    } else if ((mediaWindow != NULL) && (mediaWindow->isVisible())) {
                                        if (string("back").compare(currentTextArea->getValue()) == 0) {
                                            mediaWindow->setVisible(false);
                                            startingIndex = 0;
                                            totalMatches = -1;
                                            upnpParentID = "0";
                                            showServerWindow();
                                        } else if (string("up").compare(currentTextArea->getValue()) == 0) {
                                            startingIndex = 0;
                                            showMediaWindow(upnpParentID.c_str());
                                        } else if (string("down").compare(currentTextArea->getValue()) == 0) {
                                            showMediaWindow(upnpParentID.c_str());
                                        } else {
                                            mediaWindow->setVisible(false);
                                            int parent = Utils::stringToInteger(currentTextArea->getValue());
                                            if(parent != 0){
                                                // browse
                                                showMediaWindow(Utils::integerToString(parent));
                                            } else {
                                                mediaURI = currentTextArea->getValue();
                                                showClientWindow();
                                            }
                                        }
                                    } else if((clientWindow != NULL) && (clientWindow->isVisible())){
                                        if (string("back").compare(currentTextArea->getValue()) == 0) {
                                            clientWindow->setVisible(false);
                                            showMediaWindow(upnpParentID.c_str());
                                        } else if (string("up").compare(currentTextArea->getValue()) == 0) {
                                            itemListIndex--;
                                            showItemList(clientWindow);
                                        } else if (string("down").compare(currentTextArea->getValue()) == 0) {
                                            itemListIndex++;
                                            showItemList(clientWindow);
                                        } else {
                                            clientUDN = currentTextArea->getValue();
                                            LoggerUtil_debug(logger, "clientUDN: " << clientUDN.c_str());
                                            LoggerUtil_debug(logger, "serverUDN: " << serverUDN.c_str());
                                            LoggerUtil_debug(logger, "mediaURI: " << mediaURI.c_str());
                                            clientWindow->setVisible(false);
                                            
                                            /** play media */
                                            string uri = mediaURI;
                                            mediaURI = "";
                                            playpause(uri.c_str());

                                            playbackWindowVisibleCount = 5;
                                            playbackRunning = true;
                                            playbackWindow->setVisible(true);
                                        }
                                    } else if((playbackWindow != NULL) && (playbackWindow->isVisible())){
                                        if (string("back").compare(currentTextArea->getValue()) == 0) {
                                            playbackRunning = false;
                                            playbackWindow->setVisible(false);
                                            showClientWindow();
                                        } else if (string("backward").compare(currentTextArea->getValue()) == 0) {
                                            playbackWindow->setVisible(false);
                                            backward();
                                        } else if (string("playpause").compare(currentTextArea->getValue()) == 0) {
                                            playbackWindow->setVisible(false);
                                            playpause(mediaURI.c_str());
                                        } else if (string("forward").compare(currentTextArea->getValue()) == 0) {
                                            playbackWindow->setVisible(false);
                                            forward();
                                        }
                                    }
                                }
                            } else if (container != NULL) {
                                Vector* taList = container->getTextAreaList();
                                int listSize = taList->size();
                                for (int i = 0; i < listSize; i++) {
                                    TextArea* current = (TextArea*) taList->get(i);
                                    try {
                                        if (typeid (*current) == typeid (SelectableTextArea)) {
                                            SelectableTextArea* aux = (SelectableTextArea*) current;
                                            if (aux->isFocusable() == true){
                                                aux->setSelected(true);
                                                i = listSize;
                                            }
                                        }
                                    } catch (...) {
                                    }
                                }
                            }

                            if (((code == CodeMap::KEY_CURSOR_UP) || (code == CodeMap::KEY_CURSOR_LEFT) || (code == CodeMap::KEY_CURSOR_DOWN) || (code == CodeMap::KEY_CURSOR_RIGHT)) && (playbackRunning == true)){
                                playbackWindowVisibleCount = 5;
                                if((playbackWindow != NULL) && (playbackWindow->isVisible() == false))
                                    playbackWindow->setVisible(true);
                            }

                            return true;
                        }

                        void* createPlaybackWindowThread(void* param){
                            TVMediaCenter* mediacenter = (TVMediaCenter*) param;
                            mediacenter->playbackWindowThread();
                            return NULL;
                        }

                        void TVMediaCenter::initComponents() {
                            if (hasBeenInit == false){
                                pthread_t playbackThread;
                                pthread_create(&playbackThread, NULL, createPlaybackWindowThread, this);
                                pthread_detach(playbackThread);

                                int screenHeight = GraphicsHandler::getScreenHeight();
                                int screenWidth = GraphicsHandler::getScreenWidth();

                                int width = screenWidth * 96/100;
                                int height = screenHeight * 96/100;

                                if (width > 840)
                                    width = 840;

                                if (height > 630)
                                    height = 630;

                                int posX = (screenWidth - width)/2;
                                int posY = (screenHeight - height)/2;

                                initWindow = new Frame(posX, posY, width, height, false, true);
                                initWindow->setBackgroundColor(0,0,0,0.95*0xFF);
                                initWindow->setBorder(255, 255, 255, 255, 2);

                                SelectableTextArea* logout = new SelectableTextArea(5, height - (height * 10/100) - 5, height * 10/100, height * 10/100, "", true);
                                logout->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Exit1.png");
                                logout->setValue("exit");
                                logout->setFocusable(true);

                                TextArea* logoutFocus = new TextArea(*logout);
                                logoutFocus->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Exit2.png");
                                //logoutFocus->setBorder(255,255,255,255,1);
                                logout->setSelectedTextArea(logoutFocus);

                                initWindow->addComponent(logout);

                                serverWindow = new Frame(posX, posY, width, height, false, true);
                                serverWindow->setBackgroundColor(0,0,0,0.95*0xFF);
                                serverWindow->setBorder(255, 255, 255, 255, 2);

                                SelectableTextArea* back = new SelectableTextArea(5, height - (height * 10/100) - 5, height * 10/100, height * 10/100, "", true);
                                back->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Back1.png");
                                back->setValue("back");
                                back->setFocusable(true);

                                TextArea* backFocus = new TextArea(*back);
                                backFocus->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Back2.png");
                                //backFocus->setBorder(255,255,255,255,1);
                                back->setSelectedTextArea(backFocus);

                                serverWindow->addComponent(back);

                                back->setSelected(true);

                                TextArea* imgServer = new TextArea(((width/2) - (height/2))/2, height/4, (height/2), (height/2), "", false);
                                imgServer->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Server.png");

                                serverWindow->addComponent(imgServer);

                                audioWindow = new Frame(posX, posY, width, height, false, true);
                                audioWindow->setBackgroundColor(0,0,0,0.95*0xFF);
                                audioWindow->setBorder(255, 255, 255, 255, 2);

                                SelectableTextArea* back2 = new SelectableTextArea(5, height - (height * 10/100) - 5, height * 10/100, height * 10/100, "", true);
                                back2->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Back1.png");
                                back2->setValue("back");
                                back2->setFocusable(true);

                                TextArea* back2Focus = new TextArea(*back2);
                                back2Focus->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Back2.png");
                                //back2Focus->setBorder(255,255,255,255,1);
                                back2->setSelectedTextArea(back2Focus);

                                audioWindow->addComponent(back2);

                                back2->setSelected(true);

                                TextArea* imgAudio = new TextArea(((width/2) - (height/2))/2, height/4, (height/2), (height/2), "", false);
                                imgAudio->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Music.png");

                                audioWindow->addComponent(imgAudio);

                                videoWindow = new Frame(posX, posY, width, height, false, true);
                                videoWindow->setBackgroundColor(0,0,0,0.95*0xFF);
                                videoWindow->setBorder(255, 255, 255, 255, 2);

                                SelectableTextArea* back3 = new SelectableTextArea(5, height - (height * 10/100) - 5, height * 10/100, height * 10/100, "", true);
                                back3->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Back1.png");
                                back3->setValue("back");
                                back3->setFocusable(true);

                                TextArea* back3Focus = new TextArea(*back3);
                                back3Focus->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Back2.png");
                                //back3Focus->setBorder(255,255,255,255,1);
                                back3->setSelectedTextArea(back3Focus);

                                videoWindow->addComponent(back3);

                                back3->setSelected(true);

                                TextArea* imgVideo = new TextArea(((width/2) - (height/2))/2, height/4, (height/2), (height/2), "", false);
                                imgVideo->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Movies.png");

                                videoWindow->addComponent(imgVideo);

                                imageWindow = new Frame(posX, posY, width, height, false, true);
                                imageWindow->setBackgroundColor(0,0,0,0.95*0xFF);
                                imageWindow->setBorder(255, 255, 255, 255, 2);

                                SelectableTextArea* back4 = new SelectableTextArea(5, height - (height * 10/100) - 5, height * 10/100, height * 10/100, "", true);
                                back4->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Back1.png");
                                back4->setValue("back");
                                back4->setFocusable(true);

                                TextArea* back4Focus = new TextArea(*back4);
                                back4Focus->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Back2.png");
                                //back4Focus->setBorder(255,255,255,255,1);
                                back4->setSelectedTextArea(back4Focus);

                                imageWindow->addComponent(back4);

                                back4->setSelected(true);

                                TextArea* imgImage = new TextArea(((width/2) - (height/2))/2, height/4, (height/2), (height/2), "", false);
                                imgImage->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Pictures.png");

                                imageWindow->addComponent(imgImage);

                                clientWindow = new Frame(posX, posY, width, height, false, true);
                                clientWindow->setBackgroundColor(0,0,0,0.95*0xFF);
                                clientWindow->setBorder(255, 255, 255, 255, 2);

                                SelectableTextArea* back5 = new SelectableTextArea(5, height - (height * 10/100) - 5, height * 10/100, height * 10/100, "", true);
                                back5->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Back1.png");
                                back5->setValue("back");
                                back5->setFocusable(true);

                                TextArea* back5Focus = new TextArea(*back5);
                                back5Focus->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Back2.png");
                                //back5Focus->setBorder(255,255,255,255,1);
                                back5->setSelectedTextArea(back5Focus);

                                clientWindow->addComponent(back5);

                                back5->setSelected(true);

                                TextArea* imgClient = new TextArea(((width/2) - (height/2))/2, height/4, (height/2), (height/2), "", false);
                                imgClient->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Client.png");

                                clientWindow->addComponent(imgClient);

                                int playbackWidth = screenWidth/4;
                                int playbackHeight = screenHeight * 10/100;
                                int buttonSize = playbackWidth / 5;
                                
                                playbackWindow = new Frame(screenWidth * 2/100, screenHeight * 2/100, playbackWidth, playbackHeight, true, true);
                                playbackWindow->setBackgroundColor(0,0,0,0.95*0xFF);
                                playbackWindow->setBorder(128, 128, 128, 255, 2);

                                SelectableTextArea* back6 = new SelectableTextArea(5, (playbackHeight - buttonSize)/2, buttonSize, buttonSize, "", true);
                                back6->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Back1.png");
                                back6->setValue("back");
                                back6->setFocusable(true);

                                TextArea* back6Focus = new TextArea(*back6);
                                back6Focus->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Back2.png");
                                //back6Focus->setBorder(255,255,255,255,1);
                                back6->setSelectedTextArea(back6Focus);

                                playbackWindow->addComponent(back6);

                                back6->setSelected(true);

                                SelectableTextArea* backwardButton = new SelectableTextArea((2 * buttonSize) - 5 - 10, (playbackHeight - buttonSize)/2, buttonSize, buttonSize, "", true);
                                backwardButton->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/backward.png");
                                backwardButton->setBorder(128,128,128,255,1);
                                backwardButton->setValue("backward");
                                backwardButton->setFocusable(true);

                                TextArea* backwardFocus = new TextArea(*backwardButton);
                                backwardFocus->setBorder(0,255,0,255,1);
                                backwardButton->setSelectedTextArea(backwardFocus);

                                playbackWindow->addComponent(backwardButton);

                                SelectableTextArea* playpauseButton = new SelectableTextArea((3 * buttonSize) - 5 - 5, (playbackHeight - buttonSize)/2, buttonSize, buttonSize, "", true);
                                playpauseButton->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/playpause.png");
                                playpauseButton->setBorder(128,128,128,255,1);
                                playpauseButton->setValue("playpause");
                                playpauseButton->setFocusable(true);

                                TextArea* playpauseFocus = new TextArea(*playpauseButton);
                                playpauseFocus->setBorder(0,255,0,255,1);
                                playpauseButton->setSelectedTextArea(playpauseFocus);

                                playbackWindow->addComponent(playpauseButton);

                                SelectableTextArea* forwardButton = new SelectableTextArea((4 * buttonSize) - 5, (playbackHeight - buttonSize)/2, buttonSize, buttonSize, "", true);
                                forwardButton->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/forward.png");
                                forwardButton->setBorder(128,128,128,255,1);
                                forwardButton->setValue("forward");
                                forwardButton->setFocusable(true);

                                TextArea* forwardFocus = new TextArea(*forwardButton);
                                forwardFocus->setBorder(0,255,0,255,1);
                                forwardButton->setSelectedTextArea(forwardFocus);

                                playbackWindow->addComponent(forwardButton);

                                if(width >= 1600){
                                    SelectableTextArea* photo = new SelectableTextArea((width - 1536)/2, (height - 512)/2, 512, 512, "", true);
                                    photo->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Pictures1.png");
                                    photo->setValue("image");
                                    photo->setFocusable(true);

                                    TextArea* photoFocus = new TextArea(*photo);
                                    photoFocus->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Pictures2.png");
                                    photoFocus->setBorder(255,255,255,255,1);
                                    photo->setSelectedTextArea(photoFocus);

                                    initWindow->addComponent(photo);

                                    SelectableTextArea* video = new SelectableTextArea((width - 1536)/2 + 512, (height - 512)/2, 512, 512, "", true);
                                    video->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Movies1.png");
                                    video->setValue("video");
                                    video->setFocusable(true);

                                    TextArea* videoFocus = new TextArea(*video);
                                    videoFocus->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Movies2.png");
                                    videoFocus->setBorder(255,255,255,255,1);
                                    video->setSelectedTextArea(videoFocus);

                                    initWindow->addComponent(video);

                                    SelectableTextArea* music = new SelectableTextArea((width - 1536)/2 + 1024, (height - 512)/2, 512, 512, "", true);
                                    music->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Music1.png");
                                    music->setValue("audio");
                                    music->setFocusable(true);

                                    TextArea* musicFocus = new TextArea(*music);
                                    musicFocus->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Music2.png");
                                    musicFocus->setBorder(255,255,255,255,1);
                                    music->setSelectedTextArea(musicFocus);

                                    initWindow->addComponent(music);

                                    photo->setSelected(true);
                                } else {
                                    SelectableTextArea* photo = new SelectableTextArea((width - 768)/2, (height - 256)/2, 256, 256, "", true);
                                    photo->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Pictures1.png");
                                    photo->setValue("image");
                                    photo->setFocusable(true);

                                    TextArea* photoFocus = new TextArea(*photo);
                                    photoFocus->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Pictures2.png");
                                    photoFocus->setBorder(255,255,255,255,1);
                                    photo->setSelectedTextArea(photoFocus);

                                    initWindow->addComponent(photo);

                                    SelectableTextArea* video = new SelectableTextArea((width - 768)/2 + 256, (height - 256)/2, 256, 256, "", true);
                                    video->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Movies1.png");
                                    video->setValue("video");
                                    video->setFocusable(true);

                                    TextArea* videoFocus = new TextArea(*video);
                                    videoFocus->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Movies2.png");
                                    videoFocus->setBorder(255,255,255,255,1);
                                    video->setSelectedTextArea(videoFocus);

                                    initWindow->addComponent(video);

                                    SelectableTextArea* music = new SelectableTextArea((width - 768)/2 + 512, (height - 256)/2, 256, 256, "", true);
                                    music->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Music1.png");
                                    music->setValue("audio");
                                    music->setFocusable(true);

                                    TextArea* musicFocus = new TextArea(*music);
                                    musicFocus->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Music2.png");
                                    musicFocus->setBorder(255,255,255,255,1);
                                    music->setSelectedTextArea(musicFocus);

                                    initWindow->addComponent(music);

                                    photo->setSelected(true);
                                }

                                upnpCtrl->start();

                                hasBeenInit = true;
                            }
                        }

                        void TVMediaCenter::showServerWindow() {
                            clearItemList();
                            
                            UPnPDeviceList* list = upnpCtrl->getUPnPDeviceList();
                            int size = list->size();
                            LoggerUtil_debug(logger, "Tamanho da lista de dispositivos: " << size);
                            for(int i=0; i < size; i++){
                                LoggerUtil_debug(logger, "Dispositivo: " << list->getUPnPDevice(i)->getFriendlyName() << " tipo: " << list->getUPnPDevice(i)->getDeviceType() << " ip: " << list->getUPnPDevice(i)->getLocation());
                                if(string("urn:schemas-upnp-org:device:MediaServer:1").compare(list->getUPnPDevice(i)->getDeviceType()) == 0){
                                    LoggerUtil_debug(logger, "Servidor!");
                                    SelectableTextArea* item = new SelectableTextArea(0, 0, 0, 0, list->getUPnPDevice(i)->getFriendlyName(), true);
                                    item->setFontSize(28);
                                    item->setBackgroundColor(0,0,0,0xFF);
                                    item->setForegroundColor(128,128,128,255);
                                    item->setValue(list->getUPnPDevice(i)->getUDN());
                                    item->setFocusable(true);

                                    TextArea* itemFocus = new TextArea(*item);
                                    itemFocus->setForegroundColor(255,255,255,255);
                                    item->setSelectedTextArea(itemFocus);

                                    itemList.add(item);
                                }
                            }

                            serverWindow->setVisible(true);

                            showItemList(serverWindow);
                        }

                        void TVMediaCenter::showClientWindow() {
                            clearItemList();

                            UPnPDeviceList* list = upnpCtrl->getUPnPDeviceList();
                            int size = list->size();
                            LoggerUtil_debug(logger, "Tamanho da lista de dispositivos: " << size);
                            for(int i=0; i < size; i++){
                                LoggerUtil_debug(logger, "Dispositivo: " << list->getUPnPDevice(i)->getFriendlyName() << " tipo: " << list->getUPnPDevice(i)->getDeviceType() << " ip: " << list->getUPnPDevice(i)->getLocation());
                                if(string("urn:schemas-upnp-org:device:MediaRenderer:1").compare(list->getUPnPDevice(i)->getDeviceType()) == 0){
                                    LoggerUtil_debug(logger, "Cliente!");
                                    SelectableTextArea* item = new SelectableTextArea(0, 0, 0, 0, list->getUPnPDevice(i)->getFriendlyName(), true);
                                    item->setFontSize(28);
                                    item->setBackgroundColor(0,0,0,0xFF);
                                    item->setForegroundColor(128,128,128,255);
                                    item->setValue(list->getUPnPDevice(i)->getUDN());
                                    item->setFocusable(true);

                                    TextArea* itemFocus = new TextArea(*item);
                                    itemFocus->setForegroundColor(255,255,255,255);
                                    item->setSelectedTextArea(itemFocus);

                                    itemList.add(item);
                                }
                            }

                            clientWindow->setVisible(true);

                            showItemList(clientWindow);
                        }
                        
                        void TVMediaCenter::showMediaWindow(const char* parent) {
                            clearItemList();
                            if(upnpParentID.compare(parent) != 0){
                                upnpParentID = parent;
                                startingIndex = 0;
                                totalMatches = -1;
                            }
                            if(startingIndex < 0)
                                startingIndex = 0;
                            
                            UPnPDevice* server = upnpCtrl->getUPnPDevice(serverUDN.c_str());
                            if (server != NULL) {
                                UPnPService* service = server->getUPnPService("urn:schemas-upnp-org:service:ContentDirectory:1");
                                if (service != NULL){
                                    int tmpIndex = startingIndex;

                                    LoggerUtil_debug(logger, "StartingIndex: " << startingIndex);
                                    
                                    while(((tmpIndex < totalMatches) || (totalMatches == -1)) && (itemList.size() < 5)){
                                        UPnPAction* action = service->getUPnPAction("Browse");
                                        if (action != NULL) {
                                            action->setUPnPArgumentValue("ObjectID", parent);
                                            action->setUPnPArgumentValue("BrowseFlag", "BrowseDirectChildren");
                                            action->setUPnPArgumentValue("Filter", "*");
                                            action->setUPnPArgumentValue("SortCriteria", "+dc:title");
                                            action->setUPnPArgumentValue("StartingIndex", tmpIndex);
                                            action->setUPnPArgumentValue("RequestedCount", "1");

                                            if (action->sendUPnPAction()) {
                                                string result = action->getUPnPArgumentValue("Result");
                                                totalMatches = Utils::stringToInteger(action->getUPnPArgumentValue("TotalMatches"));

                                                LoggerUtil_debug(logger, "Result: " << result.c_str());

                                                //Utils::parseXMLChars(result);
                                                IXML_Document* xml = ixmlParseBuffer(result.c_str());
                                                if (xml != NULL) {
                                                    LoggerUtil_debug(logger, "XML Parsed!");

                                                    // folder
                                                    IXML_NodeList* resList = ixmlDocument_getElementsByTagName(xml, "container");
                                                    int listLength = 0;
                                                    if ((resList != NULL) && ((listLength = ixmlNodeList_length(resList)) > 0)) {
                                                        for (int index = 0; index < listLength; index++) {
                                                            IXML_Node* resNode = ixmlNodeList_item(resList, index);
                                                            if (resNode != NULL) {
                                                                string id = Utils::getAttributeValue(resNode, "id");
                                                                string title = Utils::getNodeValue(resNode, "dc:title");
                                                                string folderTitle = "[";
                                                                folderTitle.append(title);
                                                                folderTitle.append("]");

                                                                LoggerUtil_debug(logger, "Title: " << title.c_str() << " id: " << id.c_str());

                                                                SelectableTextArea* item = new SelectableTextArea(0, 0, 0, 0, folderTitle.c_str(), true);
                                                                item->setFontSize(28);
                                                                item->setBackgroundColor(0,0,0,0xFF);
                                                                item->setForegroundColor(128, 128, 128, 255);
                                                                item->setValue(id.c_str());
                                                                item->setFocusable(true);

                                                                TextArea* itemFocus = new TextArea(*item);
                                                                itemFocus->setForegroundColor(255, 255, 255, 255);
                                                                item->setSelectedTextArea(itemFocus);

                                                                itemList.add(item);
                                                            }
                                                        }
                                                    }

                                                    // item
                                                    resList = ixmlDocument_getElementsByTagName(xml, "item");
                                                    listLength = 0;
                                                    if ((resList != NULL) && ((listLength = ixmlNodeList_length(resList)) > 0)) {
                                                        for (int index = 0; index < listLength; index++) {
                                                            IXML_Node* resNode = ixmlNodeList_item(resList, index);
                                                            if (resNode != NULL) {
                                                                string tmp = Utils::getNodeValue(resNode, "upnp:class");
                                                                if (((mediaClass.compare("video") == 0) && ((tmp.find("video") != string::npos) || (tmp.find("movie") != string::npos)))
                                                                        || ((mediaClass.compare("audio") == 0) && ((tmp.find("audio") != string::npos) || (tmp.find("music") != string::npos)))
                                                                        || ((mediaClass.compare("image") == 0) && ((tmp.find("image") != string::npos) || (tmp.find("photo") != string::npos)))) {

                                                                    string title = Utils::getNodeValue(resNode, "dc:title");
                                                                    string author = Utils::getNodeValue(resNode, "dc:creator");
                                                                    string uri = Utils::getNodeValue(resNode, "res");

                                                                    LoggerUtil_debug(logger, "Title: " << title.c_str() << " Author: " << author.c_str());

                                                                    string mediaTitle;
                                                                    if (author.size() > 0) {
                                                                        mediaTitle = author;
                                                                        mediaTitle.append(" - ");
                                                                    }
                                                                    mediaTitle.append(title);

                                                                    SelectableTextArea* item = new SelectableTextArea(0, 0, 0, 0, mediaTitle.c_str(), true);
                                                                    item->setFontSize(28);
                                                                    item->setBackgroundColor(0,0,0,0xFF);
                                                                    item->setForegroundColor(128, 128, 128, 255);
                                                                    item->setValue(uri.c_str());
                                                                    item->setFocusable(true);

                                                                    TextArea* itemFocus = new TextArea(*item);
                                                                    itemFocus->setForegroundColor(255, 255, 255, 255);
                                                                    item->setSelectedTextArea(itemFocus);

                                                                    itemList.add(item);
                                                                }
                                                            }
                                                        }
                                                    }
                                                }

                                            }
                                        }
                                        tmpIndex++;
                                    }
                                    startingIndex = tmpIndex;
                                }
                            }

                            if (mediaClass.compare("image") == 0) {
                                mediaWindow = imageWindow;
                            } else if (mediaClass.compare("video") == 0) {
                                mediaWindow = videoWindow;
                            } else if (mediaClass.compare("audio") == 0) {
                                mediaWindow = audioWindow;
                            }

                            mediaWindow->setVisible(true);

                            showItemList(mediaWindow);
                        }

                        void TVMediaCenter::clearItemList(){
                            int size = itemList.size();
                            for(int i=0; i < size; i++){
                                SelectableTextArea* item = (SelectableTextArea*) itemList.get(i);
                                delete item;
                            }
                            itemList.clear();
                            itemListIndex = 0;
                        }

                        void TVMediaCenter::showItemList(Container* container){
                            LoggerUtil_info(logger, "Entrando no metodo showItemList(Container*)!");

                            int height = container->getHeight();
                            int width = container->getWidth();

                            LoggerUtil_debug(logger, "height e width recuperados!");

                            Vector* tmp = container->getTextAreaList();
                            int index = 0;
                            while(index < tmp->size()){
                                TextArea* item = (TextArea*) tmp->get(index);
                                LoggerUtil_debug(logger, "TextArea: " << item->getValue());
                                if((string("back").compare(item->getValue()) != 0) && (string("").compare(item->getValue()) != 0)){
                                    //tmp->remove(item);
                                    item->setParent(NULL);
                                    LoggerUtil_debug(logger, "REMOVIDA!");
                                } else {
                                    index++;
                                }
                            }
                            //container->validate();

                            LoggerUtil_debug(logger, "TextAreas removidas!");

                            int size = itemList.size();
                            
                            if(((itemListIndex > 0) && (itemListIndex < size)) || ((startingIndex > 5) && (mediaWindow != NULL) && (mediaWindow->isVisible()))){
                                SelectableTextArea* upList = new SelectableTextArea(width - (height * 10/100) - 5, 5, height * 10/100, height * 10/100, "", true);
                                upList->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Up1.png");
                                upList->setValue("up");
                                upList->setFocusable(true);

                                TextArea* upFocus = new TextArea(*upList);
                                upFocus->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Up2.png");
                                //upFocus->setBorder(255,255,255,255,1);
                                upList->setSelectedTextArea(upFocus);

                                container->addComponent(upList);
                            }

                            for(int i=itemListIndex; (i < size) && (i < (itemListIndex + 5)); i++){
                                SelectableTextArea* item = (SelectableTextArea*) itemList.get(i);
                                item->setBounds(width/2 - 5, (i - itemListIndex + 1)*(height/7), width/2, height/7);
                                item->getSelectedTextArea()->setBounds(width/2 - 5, (i - itemListIndex + 1)*(height/7), width/2, height/7);
                                container->addComponent(item);
                            }

                            if(((itemListIndex >= 0) && ((itemListIndex + 5) < size)) || ((startingIndex < (totalMatches - 1)) && (mediaWindow != NULL) && (mediaWindow->isVisible()))) {
                                SelectableTextArea* downList = new SelectableTextArea(width - (height * 10/100) - 5, height - (height * 10/100) - 5, height * 10/100, height * 10/100, "", true);
                                downList->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Down1.png");
                                downList->setValue("down");
                                downList->setFocusable(true);

                                TextArea* downFocus = new TextArea(*downList);
                                downFocus->setBackgroundImage("/usr/local/etc/ginga/files/img/hones/Down2.png");
                                //downFocus->setBorder(255,255,255,255,1);
                                downList->setSelectedTextArea(downFocus);

                                container->addComponent(downList);
                            }
                        }

                        void TVMediaCenter::playbackWindowThread(){
                            if (hasBeenInit == false) {
                                while (true) {
                                    pthread_testcancel();

                                    if ((playbackWindow != NULL) && (playbackWindow->isVisible())) {
                                        if(playbackWindowVisibleCount == 0){
                                            playbackWindow->setVisible(false);
                                        }
                                        playbackWindowVisibleCount--;
                                    }

                                    Utils::wait(1000);
                                }
                            }
                        }

                        void TVMediaCenter::playpause(const char* uri){
                            UPnPDevice* client = upnpCtrl->getUPnPDevice(clientUDN.c_str());
                            if (client != NULL) {
                                UPnPService* service = client->getUPnPService("urn:schemas-upnp-org:service:AVTransport:1");
                                if (service != NULL){
                                    string tstate;
                                    UPnPAction* action = service->getUPnPAction("GetTransportInfo");
                                    if (action != NULL) {
                                        action->setUPnPArgumentValue("InstanceID", "0");
                                        if (action->sendUPnPAction()) {
                                            tstate = action->getUPnPArgumentValue("CurrentTransportState");
                                        }
                                    }
                                    if(mediaURI.compare(uri) == 0){
                                        // mesma midia
                                        if ((tstate.compare("STOPPED") == 0) || (tstate.compare("PAUSED_PLAYBACK") == 0)){
                                            action = service->getUPnPAction("Play");
                                            if (action != NULL){
                                                action->setUPnPArgumentValue("InstanceID", "0");
                                                action->setUPnPArgumentValue("Speed", "1");
                                                action->sendUPnPAction();
                                            }
                                        } else if (tstate.compare("PLAYING") == 0){
                                            action = service->getUPnPAction("Pause");
                                            if (action != NULL){
                                                action->setUPnPArgumentValue("InstanceID", "0");
                                                action->sendUPnPAction();
                                            }
                                        }
                                    } else {
                                        // nova midia
                                        mediaURI = uri;
                                        Utils::parseXMLChars(mediaURI);
                                        // interrompe midia anterior
                                        if (tstate.compare("PLAYING") == 0){
                                            action = service->getUPnPAction("Stop");
                                            if (action != NULL){
                                                action->setUPnPArgumentValue("InstanceID", "0");
                                                action->sendUPnPAction();
                                            }
                                        }

                                        action = service->getUPnPAction("SetAVTransportURI");
                                        if (action != NULL) {
                                            string metadata = "";
                                            getMetadata(metadata);

                                            action->setUPnPArgumentValue("InstanceID", "0");
                                            action->setUPnPArgumentValue("CurrentURI", mediaURI.c_str());
                                            action->setUPnPArgumentValue("CurrentURIMetaData", metadata.c_str());
                                            if (action->sendUPnPAction() == true) {
                                                action = service->getUPnPAction("Play");
                                                if (action != NULL) {
                                                    action->setUPnPArgumentValue("InstanceID", "0");
                                                    action->setUPnPArgumentValue("Speed", "1");
                                                    action->sendUPnPAction();
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }

                        void TVMediaCenter::backward(){
                            string previousURI;
                            bool hasFound = false;
                            UPnPDevice* server = upnpCtrl->getUPnPDevice(serverUDN.c_str());
                            if (server != NULL) {
                                UPnPService* service = server->getUPnPService("urn:schemas-upnp-org:service:ContentDirectory:1");
                                if (service != NULL){
                                    int tmpIndex = 0;
                                    int tmpTotalMatches = -1;

                                    while((tmpIndex < tmpTotalMatches) || (tmpTotalMatches == -1)){
                                        UPnPAction* action = service->getUPnPAction("Browse");
                                        if (action != NULL) {
                                            action->setUPnPArgumentValue("ObjectID", upnpParentID.c_str());
                                            action->setUPnPArgumentValue("BrowseFlag", "BrowseDirectChildren");
                                            action->setUPnPArgumentValue("Filter", "*");
                                            action->setUPnPArgumentValue("SortCriteria", "+dc:title");
                                            action->setUPnPArgumentValue("StartingIndex", tmpIndex);
                                            action->setUPnPArgumentValue("RequestedCount", "1");

                                            if (action->sendUPnPAction()) {
                                                string result = action->getUPnPArgumentValue("Result");
                                                tmpTotalMatches = Utils::stringToInteger(action->getUPnPArgumentValue("TotalMatches"));

                                                LoggerUtil_debug(logger, "Result: " << result.c_str());

                                                //Utils::parseXMLChars(result);
                                                IXML_Document* xml = ixmlParseBuffer(result.c_str());
                                                if (xml != NULL) {
                                                    LoggerUtil_debug(logger, "XML Parsed!");

                                                    // item
                                                    IXML_NodeList* resList = ixmlDocument_getElementsByTagName(xml, "item");
                                                    int listLength = 0;
                                                    if ((resList != NULL) && ((listLength = ixmlNodeList_length(resList)) > 0)) {
                                                        for (int index = 0; index < listLength; index++) {
                                                            IXML_Node* resNode = ixmlNodeList_item(resList, index);
                                                            if (resNode != NULL) {
                                                                string tmp = Utils::getNodeValue(resNode, "upnp:class");
                                                                if (((mediaClass.compare("video") == 0) && ((tmp.find("video") != string::npos) || (tmp.find("movie") != string::npos)))
                                                                        || ((mediaClass.compare("audio") == 0) && ((tmp.find("audio") != string::npos) || (tmp.find("music") != string::npos)))
                                                                        || ((mediaClass.compare("image") == 0) && ((tmp.find("image") != string::npos) || (tmp.find("photo") != string::npos)))) {

                                                                    string uri = Utils::getNodeValue(resNode, "res");

                                                                    if(mediaURI.compare(uri) == 0){
                                                                        tmpIndex = tmpTotalMatches;
                                                                        hasFound = true;
                                                                    } else {
                                                                        previousURI = uri;
                                                                    }

                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        tmpIndex++;
                                    }
                                }
                            }

                            if((hasFound == true) && (previousURI.size() > 0))
                                playpause(previousURI.c_str());
                        }

                        void TVMediaCenter::forward(){
                            UPnPDevice* server = upnpCtrl->getUPnPDevice(serverUDN.c_str());
                            if (server != NULL) {
                                UPnPService* service = server->getUPnPService("urn:schemas-upnp-org:service:ContentDirectory:1");
                                if (service != NULL){
                                    int tmpIndex = 0;
                                    int tmpTotalMatches = -1;
                                    bool hasFound = false;

                                    while((tmpIndex < tmpTotalMatches) || (tmpTotalMatches == -1)){
                                        UPnPAction* action = service->getUPnPAction("Browse");
                                        if (action != NULL) {
                                            action->setUPnPArgumentValue("ObjectID", upnpParentID.c_str());
                                            action->setUPnPArgumentValue("BrowseFlag", "BrowseDirectChildren");
                                            action->setUPnPArgumentValue("Filter", "*");
                                            action->setUPnPArgumentValue("SortCriteria", "+dc:title");
                                            action->setUPnPArgumentValue("StartingIndex", tmpIndex);
                                            action->setUPnPArgumentValue("RequestedCount", "1");

                                            if (action->sendUPnPAction()) {
                                                string result = action->getUPnPArgumentValue("Result");
                                                tmpTotalMatches = Utils::stringToInteger(action->getUPnPArgumentValue("TotalMatches"));

                                                LoggerUtil_debug(logger, "Result: " << result.c_str());

                                                //Utils::parseXMLChars(result);
                                                IXML_Document* xml = ixmlParseBuffer(result.c_str());
                                                if (xml != NULL) {
                                                    LoggerUtil_debug(logger, "XML Parsed!");

                                                    // item
                                                    IXML_NodeList* resList = ixmlDocument_getElementsByTagName(xml, "item");
                                                    int listLength = 0;
                                                    if ((resList != NULL) && ((listLength = ixmlNodeList_length(resList)) > 0)) {
                                                        for (int index = 0; index < listLength; index++) {
                                                            IXML_Node* resNode = ixmlNodeList_item(resList, index);
                                                            if (resNode != NULL) {
                                                                string tmp = Utils::getNodeValue(resNode, "upnp:class");
                                                                if (((mediaClass.compare("video") == 0) && ((tmp.find("video") != string::npos) || (tmp.find("movie") != string::npos)))
                                                                        || ((mediaClass.compare("audio") == 0) && ((tmp.find("audio") != string::npos) || (tmp.find("music") != string::npos)))
                                                                        || ((mediaClass.compare("image") == 0) && ((tmp.find("image") != string::npos) || (tmp.find("photo") != string::npos)))) {

                                                                    string uri = Utils::getNodeValue(resNode, "res");

                                                                    if(hasFound == true){
                                                                        playpause(uri.c_str());
                                                                        tmpIndex = tmpTotalMatches;
                                                                    }

                                                                    if(mediaURI.compare(uri) == 0)
                                                                        hasFound = true;

                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        tmpIndex++;
                                    }
                                }
                            }
                        }

                        void TVMediaCenter::getMetadata(string& metadata){
                            UPnPDevice* server = upnpCtrl->getUPnPDevice(serverUDN.c_str());
                            if (server != NULL) {
                                UPnPService* service = server->getUPnPService("urn:schemas-upnp-org:service:ContentDirectory:1");
                                if (service != NULL){
                                    int tmpIndex = 0;
                                    int tmpTotalMatches = -1;

                                    while((tmpIndex < tmpTotalMatches) || (tmpTotalMatches == -1)){
                                        UPnPAction* action = service->getUPnPAction("Browse");
                                        if (action != NULL) {
                                            action->setUPnPArgumentValue("ObjectID", upnpParentID.c_str());
                                            action->setUPnPArgumentValue("BrowseFlag", "BrowseDirectChildren");
                                            action->setUPnPArgumentValue("Filter", "*");
                                            action->setUPnPArgumentValue("SortCriteria", "+dc:title");
                                            action->setUPnPArgumentValue("StartingIndex", tmpIndex);
                                            action->setUPnPArgumentValue("RequestedCount", "1");

                                            if (action->sendUPnPAction()) {
                                                string result = action->getUPnPArgumentValue("Result");
                                                tmpTotalMatches = Utils::stringToInteger(action->getUPnPArgumentValue("TotalMatches"));

                                                LoggerUtil_debug(logger, "Result: " << result.c_str());

                                                //Utils::parseXMLChars(result);
                                                IXML_Document* xml = ixmlParseBuffer(result.c_str());
                                                if (xml != NULL) {
                                                    LoggerUtil_debug(logger, "XML Parsed!");

                                                    // item
                                                    IXML_NodeList* resList = ixmlDocument_getElementsByTagName(xml, "item");
                                                    int listLength = 0;
                                                    if ((resList != NULL) && ((listLength = ixmlNodeList_length(resList)) > 0)) {
                                                        for (int index = 0; index < listLength; index++) {
                                                            IXML_Node* resNode = ixmlNodeList_item(resList, index);
                                                            if (resNode != NULL) {
                                                                string tmp = Utils::getNodeValue(resNode, "upnp:class");
                                                                if (((mediaClass.compare("video") == 0) && ((tmp.find("video") != string::npos) || (tmp.find("movie") != string::npos)))
                                                                        || ((mediaClass.compare("audio") == 0) && ((tmp.find("audio") != string::npos) || (tmp.find("music") != string::npos)))
                                                                        || ((mediaClass.compare("image") == 0) && ((tmp.find("image") != string::npos) || (tmp.find("photo") != string::npos)))) {

                                                                    string uri = Utils::getNodeValue(resNode, "res");

                                                                    if (mediaURI.compare(uri) == 0) {
                                                                        string id = Utils::getAttributeValue(resNode, "id");
                                                                        action->setUPnPArgumentValue("ObjectID", id.c_str());
                                                                        action->setUPnPArgumentValue("BrowseFlag", "BrowseMetadata");
                                                                        action->setUPnPArgumentValue("Filter", "*");
                                                                        action->setUPnPArgumentValue("SortCriteria", "+dc:title");
                                                                        action->setUPnPArgumentValue("StartingIndex", "0");
                                                                        action->setUPnPArgumentValue("RequestedCount", "1");

                                                                        if (action->sendUPnPAction())
                                                                            metadata = action->getUPnPArgumentValue("Result");

                                                                        tmpIndex = tmpTotalMatches;
                                                                    }

                                                                }
                                                            }
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        tmpIndex++;
                                    }
                                }
                            }
                        }

                    }
                }
            }
        }
    }
}
