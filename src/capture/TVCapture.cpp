#include "honesconfig.h"

#include <string.h>
#include <vector>
#include "util/Utils.h"
#include "capture/VideoCapture.h"
#include "capture/TVCapture.h"

using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace capture {

                        const char* TVCapture::TVCAPTURE_DEVICE_DESCRIPTION =
                                "<?xml version=\"1.0\" ?>\n"
                                "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\n"
                                " 	<specVersion>\n"
                                " 		<major>1</major>\n"
                                " 		<minor>0</minor>\n"
                                " 	</specVersion>\n"
                                " 	<device>\n"
                                " 		<deviceType>urn:schemas-upnp-org:device:tvcapture:1</deviceType>\n"
                                " 		<friendlyName>Lince TV</friendlyName>\n"
                                " 		<manufacturer>Lince</manufacturer>\n"
                                " 		<manufacturerURL>http://lince.dc.ufscar.br</manufacturerURL>\n"
                                " 		<modelDescription>UPnP TV Capture Device</modelDescription>\n"
                                " 		<modelName>TV Capture</modelName>\n"
                                " 		<modelNumber>1.0</modelNumber>\n"
                                " 		<modelURL>http://lince.dc.ufscar.br</modelURL>\n"
                                " 		<serviceList>\n"
                                " 			<service>\n"
                                " 				<serviceType>urn:schemas-upnp-org:service:capture:1</serviceType>\n"
                                " 				<serviceId>urn:schemas-upnp-org:serviceId:capture:1</serviceId>\n"
                                " 				<SCPDURL>/service/capture/description.xml</SCPDURL>\n"
                                " 				<controlURL>/service/capture/control</controlURL>\n"
                                " 				<eventSubURL>/service/capture/eventSub</eventSubURL>\n"
                                " 			</service>\n"
                                " 		</serviceList>\n"
                                "	</device>\n"
                                "</root>\n";

                        const char* TVCapture::TVCAPTURE_SERVICE_DESCRIPTION =
                                "<?xml version=\"1.0\"?>\n"
                                "<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\" >\n"
                                " 	<specVersion>\n"
                                " 		<major>1</major>\n"
                                " 		<minor>0</minor>\n"
                                " 	</specVersion>\n"
                                " 	<actionList>\n"
                                " 		<action>\n"
                                " 			<name>GetCaptureInfo</name>\n"
                                " 			<argumentList>\n"
                                " 				<argument>\n"
                                " 					<name>AudioBitrate</name>\n"
                                " 					<relatedStateVariable>CurrentAudioBitrate</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>AudioChannels</name>\n"
                                " 					<relatedStateVariable>CurrentAudioChannels</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>AudioCodec</name>\n"
                                " 					<relatedStateVariable>CurrentAudioCodec</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>FrameRate</name>\n"
                                " 					<relatedStateVariable>CurrentFrameRate</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>Height</name>\n"
                                " 					<relatedStateVariable>CurrentHeight</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>SampleRate</name>\n"
                                " 					<relatedStateVariable>CurrentSampleRate</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>VideoBitrate</name>\n"
                                " 					<relatedStateVariable>CurrentVideoBitrate</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>VideoCodec</name>\n"
                                " 					<relatedStateVariable>CurrentVideoCodec</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>VideoFormat</name>\n"
                                " 					<relatedStateVariable>CurrentVideoFormat</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>Width</name>\n"
                                " 					<relatedStateVariable>CurrentWidth</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>VideoURI</name>\n"
                                " 					<relatedStateVariable>CurrentVideoURI</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>AudioURI</name>\n"
                                " 					<relatedStateVariable>CurrentAudioURI</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 			</argumentList>\n"
                                " 		</action>\n"
                                " 		<action>\n"
                                " 			<name>Start</name>\n"
                                " 			<argumentList>\n"
                                " 				<argument>\n"
                                " 					<name>Result</name>\n"
                                " 					<relatedStateVariable>CurrentResult</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 			</argumentList>\n"
                                " 		</action>\n"
                                " 		<action>\n"
                                " 			<name>Stop</name>\n"
                                " 			<argumentList>\n"
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
                                " 			<name>CurrentVideoURI</name>\n"
                                " 			<dataType>string</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentAudioURI</name>\n"
                                " 			<dataType>string</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentResult</name>\n"
                                " 			<dataType>boolean</dataType>\n"
                                " 		</stateVariable>\n"
                                " 	</serviceStateTable>\n"
                                "</scpd>\n";

                        TVCapture::TVCapture() : UPnPDevice() {
                            restoreUUID(string(Utils::getHomeDir()).append("/").append(".tvcapture.uuid").c_str());

                            if (loadDescription(TVCAPTURE_DEVICE_DESCRIPTION) == false)
                                throw "Invalid description!";

                            if (getService()->loadSCPD(TVCAPTURE_SERVICE_DESCRIPTION) == false)
                                throw "Invalid description!";

                            setUPnPActionListener(this);
                            setUPnPQueryListener(this);
                        }

                        TVCapture::~TVCapture() {
                            stop();
                        }

                        UPnPService* TVCapture::getService() {
                            return getUPnPService("urn:schemas-upnp-org:service:capture:1");
                        }

                        bool TVCapture::actionRequest(UPnPAction *action) {
                            const char* actionName = action->getName();
                            VideoCapture* videoCap = VideoCapture::VideoCaptureFactory();

                            if ((strcmp(actionName, "GetCaptureInfo") == 0) && (videoCap->isRunning())) {
                                action->getUPnPArgument("AudioBitrate")->setValue(videoCap->getAudioBitrate());
                                action->getUPnPArgument("AudioChannels")->setValue(videoCap->getAudioChannels());
                                action->getUPnPArgument("AudioCodec")->setValue(videoCap->getAudioCodec());
                                action->getUPnPArgument("FrameRate")->setValue(videoCap->getFrameRate());
                                action->getUPnPArgument("Height")->setValue(videoCap->getHeight());
                                action->getUPnPArgument("SampleRate")->setValue(videoCap->getSampleRate());
                                action->getUPnPArgument("VideoBitrate")->setValue(videoCap->getVideoBitrate());
                                action->getUPnPArgument("VideoCodec")->setValue(videoCap->getVideoCodec());
                                action->getUPnPArgument("VideoFormat")->setValue(videoCap->getVideoFormat());
                                action->getUPnPArgument("Width")->setValue(videoCap->getWidth());
                                action->getUPnPArgument("VideoURI")->setValue(videoCap->getDestinations()[0]);
                                action->getUPnPArgument("AudioURI")->setValue(videoCap->getDestinations()[1]);

                                return true;
                            } else if (strcmp(actionName, "Start") == 0) {
                                vector<const char*> currentDestinations = videoCap->getDestinations();
                                bool video = true;
                                bool audio = true;
                                for (int i = 0; i <  currentDestinations.size(); i++){
                                     if(strcmp(currentDestinations[i], "rtp://239.255.255.250:10000") == 0)
                                         video = false;
                                     else if(strcmp(currentDestinations[i], "rtp://239.255.255.250:20000") == 0)
                                         audio = false;
                                }

                                if ((video == true) || (audio == true)) {
                                    previousVideoFormat = videoCap->getVideoFormat();
                                    videoCap->setVideoFormat("rtp");
                                    vector<const char*> destinations;
                                    if (video == true)
                                        destinations.push_back("rtp://239.255.255.250:10000");
                                    if (audio == true)
                                        destinations.push_back("rtp://239.255.255.250:20000");
                                    videoCap->addDestination(destinations);
                                }

                                action->getUPnPArgument("Result")->setValue(videoCap->isRunning() == true ? "1" : "0");

                                return true;
                            } else if (strcmp(actionName, "Stop") == 0) {
                                vector<const char*> currentDestinations = videoCap->getDestinations();
                                bool video = false;
                                bool audio = false;
                                for (int i = 0; i <  currentDestinations.size(); i++){
                                     if(strcmp(currentDestinations[i], "rtp://239.255.255.250:10000") == 0)
                                         video = true;
                                     else if(strcmp(currentDestinations[i], "rtp://239.255.255.250:20000") == 0)
                                         audio = true;
                                }

                                if ((video == true) || (audio == true)) {
                                    videoCap->setVideoFormat(previousVideoFormat.c_str());
                                    vector<const char*> destinations;
                                    if (video == true)
                                        destinations.push_back("rtp://239.255.255.250:10000");
                                    if (audio == true)
                                        destinations.push_back("rtp://239.255.255.250:20000");
                                    videoCap->removeDestination(destinations);
                                }

                                action->getUPnPArgument("Result")->setValue(((video == true) || (audio == true)) || ((video == false) && (audio == false)) ? "1" : "0");

                                return true;
                            }

                            return false;
                        }

                        bool TVCapture::variableRequest(UPnPStateVariable *stateVar) {
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
