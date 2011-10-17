#include "honesconfig.h"

#include <time.h>
#include <string.h>
#include <upnp/ixml.h>
#include "capture/VideoCapture.h"
#include "util/Utils.h"
#include "recorder/VideoRecorder.h"

using namespace br::ufscar::lince::ginga::hones::util;
using namespace br::ufscar::lince::ginga::hones::capture;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace recorder {

                        VideoRecorder* VideoRecorder::instance = NULL;

                        VideoRecorder::VideoRecorder() {
                            running = false;
                            load();
                            start();
                        }

                        VideoRecorder::~VideoRecorder() {
                            stop();
                        }

                        VideoRecorder* VideoRecorder::VideoRecorderFactory() {
                            if (instance == NULL) {
                                instance = new VideoRecorder();
                            }
                            return instance;
                        }

                        void VideoRecorder::VideoRecorderDestructor() {
                            if (instance != NULL) {
                                delete instance;
                                instance = NULL;
                            }
                        }

                        void VideoRecorder::addScheduledItem(ScheduledItem* item) {
                            stop();
                            scheduledItems.push_back(item);
                            load();
                            start();
                        }

                        void VideoRecorder::removeScheduledItem(ScheduledItem* item) {
                            for (int i = 0; i < scheduledItems.size(); i++) {
                                if (scheduledItems[i] == item) {
                                    stop();
                                    scheduledItems.erase(scheduledItems.begin() + i--);
                                    load();
                                    start();
                                }
                            }
                        }

                        vector<ScheduledItem*> VideoRecorder::getScheduledItems() {
                            return scheduledItems;
                        }

                        bool VideoRecorder::isRunning() {
                            return running;
                        }

                        bool VideoRecorder::start() {
                            if (isRunning() == false) {
                                running = true;

                                if (pthread_create(&thread, NULL, createVideoRecorderThread, this) != 0) {
                                    running = false;
                                    return false;
                                }

                                pthread_detach(thread);

                                return true;
                            } else {
                                return false;
                            }
                        }

                        bool VideoRecorder::stop() {
                            if (isRunning() == true) {
                                pthread_cancel(thread);
                                pthread_detach(thread);

                                running = false;

                                return true;
                            } else {
                                return false;
                            }
                        }

                        void VideoRecorder::load() {
                            string file = string(Utils::getHomeDir()).append("/").append(".videorecorder.xml").c_str();

                            string tmp;
                            char* c = new char[2];
                            c[1] = '\0';
                            int configfd = open(file.c_str(), O_RDONLY);
                            if (configfd != -1) {
                                while (read(configfd, c, 1) > 0) {
                                    tmp.append(c);
                                }
                                close(configfd);
                            }
                            delete[] c;

                            IXML_Document* xml = ixmlParseBuffer(tmp.c_str());
                            if (xml != NULL) {
                                IXML_NodeList* scheduledItemList = ixmlDocument_getElementsByTagName(xml, "ScheduledItem");
                                int listLength = 0;
                                if ((scheduledItemList != NULL) && ((listLength = ixmlNodeList_length(scheduledItemList)) > 0)) {

                                    time_t timeNow = time(NULL);

                                    for (int index = 0; index < listLength; index++) {
                                        IXML_Node* scheduledItemNode = ixmlNodeList_item(scheduledItemList, index);
                                        if (scheduledItemNode != NULL) {
                                            string destinationFile = Utils::getNodeValue(scheduledItemNode, "DestinationFile");
                                            bool canAdd = true;
                                            for (int i = 0; (i < scheduledItems.size()) && (canAdd == true); i++) {
                                                if (destinationFile.compare(scheduledItems[i]->getDestinationFile()) == 0)
                                                    canAdd = false;
                                            }

                                            if (canAdd == true){
                                                ScheduledItem* aux = new ScheduledItem();
                                                aux->setAudioBitrate(Utils::stringToInteger(Utils::getNodeValue(scheduledItemNode, "AudioBitrate")));
                                                aux->setAudioChannels(Utils::stringToInteger(Utils::getNodeValue(scheduledItemNode, "AudioChannels")));
                                                aux->setAudioCodec(Utils::getNodeValue(scheduledItemNode, "AudioCodec"));
                                                aux->setBeginRecordingDate(Utils::getNodeValue(scheduledItemNode, "BeginRecordingDate"));
                                                aux->setBeginRecordingTime(Utils::getNodeValue(scheduledItemNode, "BeginRecordingTime"));
                                                aux->setDestinationFile(Utils::getNodeValue(scheduledItemNode, "DestinationFile"));
                                                aux->setEndRecordingDate(Utils::getNodeValue(scheduledItemNode, "EndRecordingDate"));
                                                aux->setEndRecordingTime(Utils::getNodeValue(scheduledItemNode, "EndRecordingTime"));
                                                aux->setFrameRate(Utils::stringToInteger(Utils::getNodeValue(scheduledItemNode, "FrameRate")));
                                                aux->setHeight(Utils::stringToInteger(Utils::getNodeValue(scheduledItemNode, "Height")));
                                                aux->setSampleRate(Utils::stringToInteger(Utils::getNodeValue(scheduledItemNode, "SampleRate")));
                                                aux->setVideoBitrate(Utils::stringToInteger(Utils::getNodeValue(scheduledItemNode, "VideoBitrate")));
                                                aux->setVideoCodec(Utils::getNodeValue(scheduledItemNode, "VideoCodec"));
                                                aux->setVideoFormat(Utils::getNodeValue(scheduledItemNode, "VideoFormat"));
                                                aux->setWidth(Utils::stringToInteger(Utils::getNodeValue(scheduledItemNode, "Width")));

                                                string date1 = aux->getBeginRecordingDate();
                                                string time1 = aux->getBeginRecordingTime();
                                                int day1, month1, year1;
                                                int hour1, minute1, second1;

                                                day1 = atol(date1.substr(0, date1.find("/")).c_str());
                                                date1 = date1.substr(date1.find("/") + 1);
                                                month1 = atol(date1.substr(0, date1.find("/")).c_str());
                                                date1 = date1.substr(date1.find("/") + 1);
                                                year1 = atol(date1.c_str());

                                                hour1 = atol(time1.substr(0, time1.find(":")).c_str());
                                                time1 = time1.substr(time1.find(":") + 1);
                                                minute1 = atol(time1.substr(0, time1.find(":")).c_str());
                                                time1 = time1.substr(time1.find(":") + 1);
                                                second1 = atol(time1.c_str());

                                                struct tm timeinfo1;
                                                timeinfo1.tm_year = year1 - 1900;
                                                timeinfo1.tm_mon = month1 - 1;
                                                timeinfo1.tm_mday = day1;
                                                timeinfo1.tm_hour = hour1;
                                                timeinfo1.tm_min = minute1;
                                                timeinfo1.tm_sec = second1;

                                                time_t timeAux = mktime(&timeinfo1);

                                                double dif = difftime(timeAux, timeNow);

                                                // tolerancia de 5 segundos, se passar da hora da gravacao
                                                if (dif < -5)
                                                    delete aux;
                                                else
                                                    scheduledItems.push_back(aux);
                                            }
                                        }
                                    }
                                }
                            } else {
                                xml = ixmlParseBuffer("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<list>\n</list>");
                            }

                            IXML_Node* rootNode = xml->n.firstChild;
                            if (rootNode != NULL) {
                                for (int i = 0; i < scheduledItems.size(); i++) {
                                    ScheduledItem* aux = scheduledItems[i];

                                    IXML_Element* newElem = (IXML_Element*) malloc(sizeof (IXML_Element));
                                    memset(newElem, 0, sizeof (IXML_Element));
                                    IXML_Node* newNode = (IXML_Node*) newElem;
                                    newNode->nodeName = strdup("ScheduledItem");
                                    newNode->nodeType = eELEMENT_NODE;
                                    ixmlNode_appendChild(rootNode, newNode);

                                    Utils::setNode(newNode, "AudioBitrate", Utils::integerToString(aux->getAudioBitrate()));
                                    Utils::setNode(newNode, "AudioChannels", Utils::integerToString(aux->getAudioChannels()));
                                    Utils::setNode(newNode, "AudioCodec", aux->getAudioCodec());
                                    Utils::setNode(newNode, "BeginRecordingDate", aux->getBeginRecordingDate());
                                    Utils::setNode(newNode, "BeginRecordingTime", aux->getBeginRecordingTime());
                                    Utils::setNode(newNode, "DestinationFile", aux->getDestinationFile());
                                    Utils::setNode(newNode, "EndRecordingDate", aux->getEndRecordingDate());
                                    Utils::setNode(newNode, "EndRecordingTime", aux->getEndRecordingTime());
                                    Utils::setNode(newNode, "FrameRate", Utils::integerToString(aux->getFrameRate()));
                                    Utils::setNode(newNode, "Height", Utils::integerToString(aux->getHeight()));
                                    Utils::setNode(newNode, "SampleRate", Utils::integerToString(aux->getSampleRate()));
                                    Utils::setNode(newNode, "VideoBitrate", Utils::integerToString(aux->getVideoBitrate()));
                                    Utils::setNode(newNode, "VideoCodec", aux->getVideoCodec());
                                    Utils::setNode(newNode, "VideoFormat", aux->getVideoFormat());
                                    Utils::setNode(newNode, "Width", Utils::integerToString(aux->getWidth()));
                                }

                                tmp = ixmlPrintDocument(xml);
                            }

                            configfd = open(file.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
                            if (configfd != -1) {
                                write(configfd, tmp.c_str(), tmp.size());
                                close(configfd);
                            }

                            ixmlDocument_free(xml);
                        }

                        void* VideoRecorder::createVideoRecorderThread(void* param) {
                            VideoRecorder* ptr = (VideoRecorder*) param;
                            while (ptr->isRunning() == true) {
                                pthread_testcancel();

                                vector<ScheduledItem*> scheduledItems = ptr->getScheduledItems();

                                time_t timeNow = time(NULL);

                                for (int i = 0; i < scheduledItems.size(); i++) {
                                    ScheduledItem* item = scheduledItems[i];

                                    string date1 = item->getBeginRecordingDate();
                                    string time1 = item->getBeginRecordingTime();
                                    int day1, month1, year1;
                                    int hour1, minute1, second1;

                                    day1 = atol(date1.substr(0, date1.find("/")).c_str());
                                    date1 = date1.substr(date1.find("/") + 1);
                                    month1 = atol(date1.substr(0, date1.find("/")).c_str());
                                    date1 = date1.substr(date1.find("/") + 1);
                                    year1 = atol(date1.c_str());

                                    hour1 = atol(time1.substr(0, time1.find(":")).c_str());
                                    time1 = time1.substr(time1.find(":") + 1);
                                    minute1 = atol(time1.substr(0, time1.find(":")).c_str());
                                    time1 = time1.substr(time1.find(":") + 1);
                                    second1 = atol(time1.c_str());

                                    struct tm timeinfo1;
                                    timeinfo1.tm_year = year1 - 1900;
                                    timeinfo1.tm_mon = month1 - 1;
                                    timeinfo1.tm_mday = day1;
                                    timeinfo1.tm_hour = hour1;
                                    timeinfo1.tm_min = minute1;
                                    timeinfo1.tm_sec = second1;

                                    time_t timeItem = mktime(&timeinfo1);

                                    double dif = difftime(timeItem, timeNow);

                                    // tolerancia de 5 segundos
                                    if ((dif <= 5) && (dif >= -5)) {
                                        // grava
                                        VideoCapture* videoCap = VideoCapture::VideoCaptureFactory();
                                        videoCap->setWidth(item->getWidth());
                                        videoCap->setHeight(item->getHeight());
                                        videoCap->setVideoFormat(item->getVideoFormat());
                                        videoCap->setVideoCodec(item->getVideoCodec());
                                        videoCap->setVideoBitrate(item->getVideoBitrate());
                                        videoCap->setSampleRate(item->getSampleRate());
                                        videoCap->setFrameRate(item->getFrameRate());
                                        videoCap->setAudioCodec(item->getAudioCodec());
                                        videoCap->setAudioChannels(item->getAudioChannels());
                                        videoCap->setAudioBitrate(item->getAudioBitrate());
                                        videoCap->addDestination(item->getDestinationFile());

                                        pthread_t t;
                                        if (pthread_create(&t, NULL, createScheduledItemThread, item) == 0)
                                            ptr->removeScheduledItem(item);
                                    } else if (dif < -5) {
                                        // passou da hora da gravacao
                                        ptr->removeScheduledItem(item);
                                    }
                                }

                                Utils::wait(1000);
                            }

                            return NULL;
                        }

                        void* VideoRecorder::createScheduledItemThread(void* param){
                            ScheduledItem* item = (ScheduledItem*) param;
                            string destination = item->getDestinationFile();
                            double duration = item->getRecordingDuration();

                            pthread_testcancel();

                            Utils::wait((long)(duration * 1000));

                            VideoCapture* videoCap = VideoCapture::VideoCaptureFactory();
                            videoCap->removeDestination(destination.c_str());

                            return NULL;
                        }

                    }
                }
            }
        }
    }
}
