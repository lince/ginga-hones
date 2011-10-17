#include "honesconfig.h"

#include <string.h>
#include <vector>
#include "util/Utils.h"
#include "recorder/VideoRecorder.h"
#include "recorder/ScheduledItem.h"
#include "recorder/TVRecorder.h"

using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace recorder {

                        const char* TVRecorder::TVRECORDER_DEVICE_DESCRIPTION =
                                "<?xml version=\"1.0\" ?>\n"
                                "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\n"
                                " 	<specVersion>\n"
                                " 		<major>1</major>\n"
                                " 		<minor>0</minor>\n"
                                " 	</specVersion>\n"
                                " 	<device>\n"
                                " 		<deviceType>urn:schemas-upnp-org:device:tvrecorder:1</deviceType>\n"
                                " 		<friendlyName>Lince TV</friendlyName>\n"
                                " 		<manufacturer>Lince</manufacturer>\n"
                                " 		<manufacturerURL>http://lince.dc.ufscar.br</manufacturerURL>\n"
                                " 		<modelDescription>UPnP TV Recorder Device</modelDescription>\n"
                                " 		<modelName>TV Recorder</modelName>\n"
                                " 		<modelNumber>1.0</modelNumber>\n"
                                " 		<modelURL>http://lince.dc.ufscar.br</modelURL>\n"
                                " 		<serviceList>\n"
                                " 			<service>\n"
                                " 				<serviceType>urn:schemas-upnp-org:service:recorder:1</serviceType>\n"
                                " 				<serviceId>urn:schemas-upnp-org:serviceId:recorder:1</serviceId>\n"
                                " 				<SCPDURL>/service/recorder/description.xml</SCPDURL>\n"
                                " 				<controlURL>/service/recorder/control</controlURL>\n"
                                " 				<eventSubURL>/service/recorder/eventSub</eventSubURL>\n"
                                " 			</service>\n"
                                " 		</serviceList>\n"
                                "	</device>\n"
                                "</root>\n";

                        const char* TVRecorder::TVRECORDER_SERVICE_DESCRIPTION =
                                "<?xml version=\"1.0\"?>\n"
                                "<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\" >\n"
                                " 	<specVersion>\n"
                                " 		<major>1</major>\n"
                                " 		<minor>0</minor>\n"
                                " 	</specVersion>\n"
                                " 	<actionList>\n"
                                " 		<action>\n"
                                " 			<name>AddScheduledItem</name>\n"
                                " 			<argumentList>\n"
                                " 				<argument>\n"
                                " 					<name>AudioBitrate</name>\n"
                                " 					<relatedStateVariable>CurrentAudioBitrate</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>AudioChannels</name>\n"
                                " 					<relatedStateVariable>CurrentAudioChannels</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>AudioCodec</name>\n"
                                " 					<relatedStateVariable>CurrentAudioCodec</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>FrameRate</name>\n"
                                " 					<relatedStateVariable>CurrentFrameRate</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>Height</name>\n"
                                " 					<relatedStateVariable>CurrentHeight</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>SampleRate</name>\n"
                                " 					<relatedStateVariable>CurrentSampleRate</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>VideoBitrate</name>\n"
                                " 					<relatedStateVariable>CurrentVideoBitrate</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>VideoCodec</name>\n"
                                " 					<relatedStateVariable>CurrentVideoCodec</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>VideoFormat</name>\n"
                                " 					<relatedStateVariable>CurrentVideoFormat</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>Width</name>\n"
                                " 					<relatedStateVariable>CurrentWidth</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>BeginRecordingDate</name>\n"
                                " 					<relatedStateVariable>CurrentBeginRecordingDate</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>BeginRecordingTime</name>\n"
                                " 					<relatedStateVariable>CurrentBeginRecordingTime</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>EndRecordingDate</name>\n"
                                " 					<relatedStateVariable>CurrentEndRecordingDate</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>EndRecordingTime</name>\n"
                                " 					<relatedStateVariable>CurrentEndRecordingTime</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>DestinationFile</name>\n"
                                " 					<relatedStateVariable>CurrentDestinationFile</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>Result</name>\n"
                                " 					<relatedStateVariable>CurrentResult</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 			</argumentList>\n"
                                " 		</action>\n"
                                " 		<action>\n"
                                " 			<name>RemoveScheduledItem</name>\n"
                                " 			<argumentList>\n"
                                " 				<argument>\n"
                                " 					<name>DestinationFile</name>\n"
                                " 					<relatedStateVariable>CurrentDestinationFile</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>Result</name>\n"
                                " 					<relatedStateVariable>CurrentResult</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 			</argumentList>\n"
                                " 		</action>\n"
                                " 	</actionList>\n"
                                " 	<serviceStateTable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentAudioBitrate</name>\n"
                                " 			<dataType>ui4</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentAudioChannels</name>\n"
                                " 			<dataType>i4</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentAudioCodec</name>\n"
                                " 			<dataType>string</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentFrameRate</name>\n"
                                " 			<dataType>i4</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentHeight</name>\n"
                                " 			<dataType>ui4</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentSampleRate</name>\n"
                                " 			<dataType>ui4</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentVideoBitrate</name>\n"
                                " 			<dataType>ui4</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentVideoCodec</name>\n"
                                " 			<dataType>string</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentVideoFormat</name>\n"
                                " 			<dataType>string</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentWidth</name>\n"
                                " 			<dataType>ui4</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentBeginRecordingDate</name>\n"
                                " 			<dataType>string</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentBeginRecordingTime</name>\n"
                                " 			<dataType>string</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentEndRecordingDate</name>\n"
                                " 			<dataType>string</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentEndRecordingTime</name>\n"
                                " 			<dataType>string</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentDestinationFile</name>\n"
                                " 			<dataType>string</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentResult</name>\n"
                                " 			<dataType>boolean</dataType>\n"
                                " 		</stateVariable>\n"
                                " 	</serviceStateTable>\n"
                                "</scpd>\n";

                        TVRecorder::TVRecorder() : UPnPDevice() {
                            restoreUUID(string(Utils::getHomeDir()).append("/").append(".tvrecorder.uuid").c_str());

                            if (loadDescription(TVRECORDER_DEVICE_DESCRIPTION) == false)
                                throw "Invalid description!";

                            if (getService()->loadSCPD(TVRECORDER_SERVICE_DESCRIPTION) == false)
                                throw "Invalid description!";

                            setUPnPActionListener(this);
                            setUPnPQueryListener(this);
                        }

                        TVRecorder::~TVRecorder() {
                            stop();
                        }

                        UPnPService* TVRecorder::getService() {
                            return getUPnPService("urn:schemas-upnp-org:service:recorder:1");
                        }

                        bool TVRecorder::actionRequest(UPnPAction *action) {
                            const char* actionName = action->getName();
                            VideoRecorder* videoRec = VideoRecorder::VideoRecorderFactory();

                            if (strcmp(actionName, "AddScheduledItem") == 0) {
                                ScheduledItem* item = new ScheduledItem();

                                item->setAudioBitrate(Utils::stringToInteger(action->getUPnPArgument("AudioBitrate")->getValue()));
                                item->setAudioChannels(Utils::stringToInteger(action->getUPnPArgument("AudioChannels")->getValue()));
                                item->setAudioCodec(action->getUPnPArgument("AudioCodec")->getValue());
                                item->setBeginRecordingDate(action->getUPnPArgument("BeginRecordingDate")->getValue());
                                item->setBeginRecordingTime(action->getUPnPArgument("BeginRecordingTime")->getValue());
                                item->setDestinationFile(action->getUPnPArgument("DestinationFile")->getValue());
                                item->setEndRecordingDate(action->getUPnPArgument("EndRecordingDate")->getValue());
                                item->setEndRecordingTime(action->getUPnPArgument("EndRecordingTime")->getValue());
                                item->setFrameRate(Utils::stringToInteger(action->getUPnPArgument("FrameRate")->getValue()));
                                item->setHeight(Utils::stringToInteger(action->getUPnPArgument("Height")->getValue()));
                                item->setSampleRate(Utils::stringToInteger(action->getUPnPArgument("SampleRate")->getValue()));
                                item->setVideoBitrate(Utils::stringToInteger(action->getUPnPArgument("VideoBitrate")->getValue()));
                                item->setVideoCodec(action->getUPnPArgument("VideoCodec")->getValue());
                                item->setVideoFormat(action->getUPnPArgument("VideoFormat")->getValue());
                                item->setWidth(Utils::stringToInteger(action->getUPnPArgument("Width")->getValue()));
                                
                                videoRec->addScheduledItem(item);

                                action->getUPnPArgument("Result")->setValue("1");

                                return true;
                            } else if (strcmp(actionName, "RemoveScheduledItem") == 0) {
                                const char* destination = action->getUPnPArgument("DestinationFile")->getValue();

                                vector<ScheduledItem*> scheduledItems = videoRec->getScheduledItems();
                                bool removed = false;
                                for (int i = 0; (i <  scheduledItems.size()) && (removed == false); i++){
                                     if(strcmp(destination, scheduledItems[i]->getDestinationFile()) == 0){
                                         videoRec->removeScheduledItem(scheduledItems[i]);
                                         removed = true;
                                     }
                                }

                                action->getUPnPArgument("Result")->setValue(removed == true ? "1" : "0");

                                return true;
                            }

                            return false;
                        }

                        bool TVRecorder::variableRequest(UPnPStateVariable *stateVar) {
                            UPnPStateVariable* serviceStateVar = getService()->getUPnPStateVariable(stateVar->getName());
                            if (serviceStateVar == NULL)
                                return false;

                            stateVar->setValue(serviceStateVar->getValue());

                            return true;
                        }

                    }
                }
            }
        }
    }
}

