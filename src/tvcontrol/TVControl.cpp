#include "honesconfig.h"

#include "tvcontrol/TVControl.h"
#include "util/Utils.h"

using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace tvcontrol {

                        const char* TVControl::TVCONTROL_DEVICE_DESCRIPTION =
                                "<?xml version=\"1.0\" ?>\n"
                                "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\n"
                                " 	<specVersion>\n"
                                " 		<major>1</major>\n"
                                " 		<minor>0</minor>\n"
                                " 	</specVersion>\n"
                                " 	<device>\n"
                                " 		<deviceType>urn:schemas-upnp-org:device:tvcontrol:1</deviceType>\n"
                                " 		<friendlyName>Lince TV</friendlyName>\n"
                                " 		<manufacturer>Lince</manufacturer>\n"
                                " 		<manufacturerURL>http://lince.dc.ufscar.br</manufacturerURL>\n"
                                " 		<modelDescription>UPnP TV Control</modelDescription>\n"
                                " 		<modelName>TV Control</modelName>\n"
                                " 		<modelNumber>1.0</modelNumber>\n"
                                " 		<modelURL>http://lince.dc.ufscar.br</modelURL>\n"
                                " 		<serviceList>\n"
                                " 			<service>\n"
                                " 				<serviceType>urn:schemas-upnp-org:service:tvcontrolservice:1</serviceType>\n"
                                " 				<serviceId>urn:schemas-upnp-org:serviceId:tvcontrolservice:1</serviceId>\n"
                                " 				<SCPDURL>/service/tvcontrolservice/description.xml</SCPDURL>\n"
                                " 				<controlURL>/service/tvcontrolservice/control</controlURL>\n"
                                " 				<eventSubURL>/service/tvcontrolservice/eventSub</eventSubURL>\n"
                                " 			</service>\n"
                                " 		</serviceList>\n"
                                "	</device>\n"
                                "</root>\n";

                        const char* TVControl::TVCONTROL_SERVICE_DESCRIPTION =
                                "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                                "<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">\n"
                                "   <specVersion>\n"
                                "      <major>1</major>\n"
                                "      <minor>0</minor>\n"
                                "   </specVersion>\n"
                                "   <actionList>\n"
                                "      <action>\n"
                                "         <name>ChannelUp</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>Result</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentResult</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>ChannelDown</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>Result</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentResult</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>SetChannel</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>Channel</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>CurrentChannel</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Result</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentResult</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>GetChannel</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>Channel</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentChannel</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>GetBrightness</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>CurrentBrightness</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>Brightness</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>GetContrast</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>CurrentContrast</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>Contrast</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>GetSaturation</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>CurrentSaturation</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>Saturation</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>GetHue</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>CurrentHue</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>Hue</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>GetMute</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>CurrentMute</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>Mute</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>GetVolume</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>CurrentVolume</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>Volume</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>SetBrightness</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>DesiredBrightness</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>Brightness</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Result</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentResult</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>SetContrast</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>DesiredContrast</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>Contrast</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Result</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentResult</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>SetSaturation</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>DesiredSaturation</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>Saturation</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Result</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentResult</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>SetHue</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>DesiredHue</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>Hue</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Result</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentResult</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>SetMute</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>DesiredMute</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>Mute</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Result</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentResult</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>SetVolume</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>DesiredVolume</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>Volume</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Result</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentResult</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "   </actionList>\n"
                                "   <serviceStateTable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>Volume</name>\n"
                                "         <dataType>ui2</dataType>\n"
                                "         <allowedValueRange>\n"
                                "            <minimum>0</minimum>\n"
                                "            <maximum>100</maximum>\n"
                                "            <step>1</step>\n"
                                "         </allowedValueRange>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>Mute</name>\n"
                                "         <dataType>boolean</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>Brightness</name>\n"
                                "         <dataType>ui2</dataType>\n"
                                "         <allowedValueRange>\n"
                                "            <minimum>0</minimum>\n"
                                "            <maximum>100</maximum>\n"
                                "            <step>1</step>\n"
                                "         </allowedValueRange>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>Contrast</name>\n"
                                "         <dataType>ui2</dataType>\n"
                                "         <allowedValueRange>\n"
                                "            <minimum>0</minimum>\n"
                                "            <maximum>100</maximum>\n"
                                "            <step>1</step>\n"
                                "         </allowedValueRange>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>Saturation</name>\n"
                                "         <dataType>ui2</dataType>\n"
                                "         <allowedValueRange>\n"
                                "            <minimum>0</minimum>\n"
                                "            <maximum>100</maximum>\n"
                                "            <step>1</step>\n"
                                "         </allowedValueRange>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>Hue</name>\n"
                                "         <dataType>ui2</dataType>\n"
                                "         <allowedValueRange>\n"
                                "            <minimum>0</minimum>\n"
                                "            <maximum>100</maximum>\n"
                                "            <step>1</step>\n"
                                "         </allowedValueRange>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>CurrentResult</name>\n"
                                "         <dataType>boolean</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>CurrentChannel</name>\n"
                                "         <dataType>ui2</dataType>\n"
                                "      </stateVariable>\n"
                                "   </serviceStateTable>\n"
                                "</scpd>";

                        TVControl::TVControl() : UPnPDevice() {
                            restoreUUID(string(Utils::getHomeDir()).append("/").append(".tvcontrol.uuid").c_str());

                            if (loadDescription(TVCONTROL_DEVICE_DESCRIPTION) == false)
                                throw "Invalid description!";

                            if (getService()->loadSCPD(TVCONTROL_SERVICE_DESCRIPTION) == false)
                                throw "Invalid description!";

                            setUPnPActionListener(this);
                            setUPnPQueryListener(this);
                            control = TVControlHandler::TVControlHandlerFactory();

                            getService()->getUPnPStateVariable("Volume")->setValue(100);
                            getService()->getUPnPStateVariable("Mute")->setValue(0);
                        }

                        TVControl::~TVControl() {
                            stop();
                        }

                        UPnPService* TVControl::getService() {
                            return getUPnPService("urn:schemas-upnp-org:service:tvcontrolservice:1");
                        }

                        bool TVControl::actionRequest(UPnPAction *action) {
                            const char* actionName = action->getName();

                            if (strcmp(actionName, "ChannelUp") == 0) {
                                if (control->channelUp())
                                    action->getUPnPArgument("Result")->setValue("1");
                                else
                                    action->getUPnPArgument("Result")->setValue("0");
                                return true;
                            } else if (strcmp(actionName, "ChannelDown") == 0) {
                                if (control->channelDown())
                                    action->getUPnPArgument("Result")->setValue("1");
                                else
                                    action->getUPnPArgument("Result")->setValue("0");
                                return true;
                            } else if (strcmp(actionName, "SetChannel") == 0) {

                                const char* channel = action->getUPnPArgument("Channel")->getValue();

                                getService()->getUPnPStateVariable("CurrentChannel")->setValue(channel);

                                if (control->setChannel(Utils::stringToInteger(channel)))
                                    action->getUPnPArgument("Result")->setValue("1");
                                else
                                    action->getUPnPArgument("Result")->setValue("0");
                                return true;
                            } else if (strcmp(actionName, "GetChannel") == 0) {
                                action->getUPnPArgument("Channel")->setValue(control->getChannel());
                                return true;
                            } else if (strcmp(actionName, "GetBrightness") == 0) {
                                action->getUPnPArgument("CurrentBrightness")->setValue(control->getBrightness());
                                return true;
                            } else if (strcmp(actionName, "GetContrast") == 0) {
                                action->getUPnPArgument("CurrentContrast")->setValue(control->getContrast());
                                return true;
                            } else if (strcmp(actionName, "GetSaturation") == 0) {
                                action->getUPnPArgument("CurrentSaturation")->setValue(control->getSaturation());
                                return true;
                            } else if (strcmp(actionName, "GetHue") == 0) {
                                action->getUPnPArgument("CurrentHue")->setValue(control->getHue());
                                return true;
                            } else if (strcmp(actionName, "GetMute") == 0) {
                                action->getUPnPArgument("CurrentMute")->setValue(getService()->getUPnPStateVariable("Mute")->getValue());
                                return true;
                            } else if (strcmp(actionName, "GetVolume") == 0) {
                                action->getUPnPArgument("CurrentVolume")->setValue(getService()->getUPnPStateVariable("Volume")->getValue());
                                return true;
                            } else if (strcmp(actionName, "SetBrightness") == 0) {
                                const char* value = action->getUPnPArgument("DesiredBrightness")->getValue();
                                getService()->getUPnPStateVariable("Brightness")->setValue(value);

                                if (control->setBrightness(Utils::stringToInteger(value)))
                                    action->getUPnPArgument("Result")->setValue("1");
                                else
                                    action->getUPnPArgument("Result")->setValue("0");
                                return true;
                            } else if (strcmp(actionName, "SetContrast") == 0) {
                                const char* value = action->getUPnPArgument("DesiredContrast")->getValue();
                                getService()->getUPnPStateVariable("Contrast")->setValue(value);

                                if (control->setContrast(Utils::stringToInteger(value)))
                                    action->getUPnPArgument("Result")->setValue("1");
                                else
                                    action->getUPnPArgument("Result")->setValue("0");
                                return true;
                            } else if (strcmp(actionName, "SetSaturation") == 0) {
                                const char* value = action->getUPnPArgument("DesiredSaturation")->getValue();
                                getService()->getUPnPStateVariable("Saturation")->setValue(value);

                                if (control->setSaturation(Utils::stringToInteger(value)))
                                    action->getUPnPArgument("Result")->setValue("1");
                                else
                                    action->getUPnPArgument("Result")->setValue("0");
                                return true;
                            } else if (strcmp(actionName, "SetHue") == 0) {
                                const char* value = action->getUPnPArgument("DesiredHue")->getValue();
                                getService()->getUPnPStateVariable("Hue")->setValue(value);

                                if (control->setHue(Utils::stringToInteger(value)))
                                    action->getUPnPArgument("Result")->setValue("1");
                                else
                                    action->getUPnPArgument("Result")->setValue("0");
                                return true;
                            } else if (strcmp(actionName, "SetMute") == 0) {
                                const char* value = action->getUPnPArgument("DesiredMute")->getValue();
                                getService()->getUPnPStateVariable("Mute")->setValue(value);

                                if (strcmp(value, "1") == 0 ? control->setVolume(0) : control->setVolume(Utils::stringToInteger(getService()->getUPnPStateVariable("Volume")->getValue())))
                                    action->getUPnPArgument("Result")->setValue("1");
                                else
                                    action->getUPnPArgument("Result")->setValue("0");
                                return true;
                            } else if (strcmp(actionName, "SetVolume") == 0) {
                                const char* value = action->getUPnPArgument("DesiredVolume")->getValue();
                                getService()->getUPnPStateVariable("Volume")->setValue(value);

                                if (control->setVolume(Utils::stringToInteger(value)))
                                    action->getUPnPArgument("Result")->setValue("1");
                                else
                                    action->getUPnPArgument("Result")->setValue("0");
                                return true;
                            }

                            return false;
                        }

                        bool TVControl::variableRequest(UPnPStateVariable *stateVar) {
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

