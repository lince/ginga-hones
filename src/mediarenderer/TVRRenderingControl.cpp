#include "honesconfig.h"

#include <upnp/upnp.h>
#include "mediarenderer/TVRenderer.h"
#include "mediarenderer/TVRRenderingControl.h"
#include "util/Utils.h"

using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediarenderer {

                        const char *TVRRenderingControl::SERVICE_TYPE = "urn:schemas-upnp-org:service:RenderingControl:1";

                        const char *TVRRenderingControl::SCPD =
                                "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                                "<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">\n"
                                "   <specVersion>\n"
                                "      <major>1</major>\n"
                                "      <minor>0</minor>\n"
                                "   </specVersion>\n"
                                "   <actionList>\n"
                                "      <action>\n"
                                "         <name>GetBrightness</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
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
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>CurrentContrast</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>Contrast</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>GetMute</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Channel</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_Channel</relatedStateVariable>\n"
                                "            </argument>\n"
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
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Channel</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_Channel</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>CurrentVolume</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>Volume</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>ListPresets</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>CurrentPresetNameList</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>PresetNameList</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>SelectPreset</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>PresetName</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_PresetName</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>SetBrightness</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>DesiredBrightness</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>Brightness</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>SetContrast</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>DesiredContrast</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>Contrast</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>SetMute</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Channel</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_Channel</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>DesiredMute</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>Mute</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>SetVolume</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>InstanceID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_InstanceID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Channel</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_Channel</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>DesiredVolume</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>Volume</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "   </actionList>\n"
                                "   <serviceStateTable>\n"
                                "      <stateVariable sendEvents=\"yes\">\n"
                                "         <name>LastChange</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>A_ARG_TYPE_Channel</name>\n"
                                "         <dataType>string</dataType>\n"
                                "         <allowedValueList>\n"
                                "            <allowedValue>Master</allowedValue>\n"
                                "            <allowedValue>LF</allowedValue>\n"
                                "            <allowedValue>RF</allowedValue>\n"
                                "         </allowedValueList>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>A_ARG_TYPE_InstanceID</name>\n"
                                "         <dataType>ui4</dataType>\n"
                                "      </stateVariable>\n"
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
                                "         <name>PresetNameList</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>A_ARG_TYPE_PresetName</name>\n"
                                "         <dataType>string</dataType>\n"
                                "         <allowedValueList>\n"
                                "            <allowedValue>FactoryDefaults</allowedValue>\n"
                                "            <allowedValue>InstallationDefaults</allowedValue>\n"
                                "         </allowedValueList>\n"
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
                                "   </serviceStateTable>\n"
                                "</scpd>";

                        const char *TVRRenderingControl::INSTANCE_ID = "InstanceID";
                        const char *TVRRenderingControl::PRESET_NAME_LIST = "PresetNameList";
                        const char *TVRRenderingControl::CURRENT_PRESET_NAME_LIST = "CurrentPresetNameList";
                        const char *TVRRenderingControl::PRESET_NAME = "PresetName";
                        const char *TVRRenderingControl::MUTE = "Mute";
                        const char *TVRRenderingControl::CURRENT_MUTE = "CurrentMute";
                        const char *TVRRenderingControl::CHANNEL = "Channel";
                        const char *TVRRenderingControl::MASTER = "Master";
                        const char *TVRRenderingControl::DESIRED_MUTE = "DesiredMute";
                        const char *TVRRenderingControl::VOLUME = "Volume";
                        const char *TVRRenderingControl::CURRENT_VOLUME = "CurrentVolume";
                        const char *TVRRenderingControl::DESIRED_VOLUME = "DesiredVolume";
                        const char *TVRRenderingControl::CURRENT_CONTRAST = "CurrentContrast";
                        const char *TVRRenderingControl::CONTRAST = "Contrast";
                        const char *TVRRenderingControl::DESIRED_CONTRAST = "DesiredContrast";
                        const char *TVRRenderingControl::CURRENT_BRIGHTNESS = "CurrentBrightness";
                        const char *TVRRenderingControl::BRIGHTNESS = "Brightness";
                        const char *TVRRenderingControl::DESIRED_BRIGHTNESS = "DesiredBrightness";

                        const char *TVRRenderingControl::LIST_PRESETS = "ListPresets";
                        const char *TVRRenderingControl::SELECT_PRESET = "SelectPreset";
                        const char *TVRRenderingControl::GET_MUTE = "GetMute";
                        const char *TVRRenderingControl::SET_MUTE = "SetMute";
                        const char *TVRRenderingControl::GET_VOLUME = "GetVolume";
                        const char *TVRRenderingControl::SET_VOLUME = "SetVolume";
                        const char *TVRRenderingControl::GET_CONTRAST = "GetContrast";
                        const char *TVRRenderingControl::SET_CONTRAST = "SetContrast";
                        const char *TVRRenderingControl::GET_BRIGHTNESS = "GetBrightness";
                        const char *TVRRenderingControl::SET_BRIGHTNESS = "SetBrightness";

                        TVRRenderingControl::TVRRenderingControl(TVRenderer *tvr) {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.mediarenderer.tvrrenderingcontrol");
                            LoggerUtil_info(logger, "Entrando no construtor TVRRenderingControl(tvr)!");

                            setMediaRenderer(tvr);

                            UPnPService *serv = getService();
                            bool ret = serv->loadSCPD(SCPD);
                            if (ret == false)
                                throw "Invalid description!";
                            serv->setUPnPActionListener(this);
                            serv->setUPnPQueryListener(this);

                            LoggerUtil_info(logger, "Saindo do construtor TVRRenderingControl(tvr)!");
                        }

                        TVRRenderingControl::~TVRRenderingControl() {
                        }

                        void TVRRenderingControl::initializeStateVariables() {
                            getService()->getUPnPStateVariable(PRESET_NAME_LIST)->setValue("FactoryDefaults");
                            getService()->getUPnPStateVariable(MUTE)->setValue(0);
                            getService()->getUPnPStateVariable(VOLUME)->setValue(100);
                            getService()->getUPnPStateVariable(CONTRAST)->setValue(50);
                            getService()->getUPnPStateVariable(BRIGHTNESS)->setValue(50);
                        }

                        TVRenderer* TVRRenderingControl::getMediaRenderer() {
                            return tvRender;
                        }

                        void TVRRenderingControl::setMediaRenderer(TVRenderer *tvr) {
                            tvRender = tvr;
                        }

                        UPnPService *TVRRenderingControl::getService() {
                            return getMediaRenderer()->getUPnPService(SERVICE_TYPE);
                        }

                        bool TVRRenderingControl::actionRequest(UPnPAction *action) {
                            string actionName = action->getName();
                            string id = action->getUPnPArgumentValue(INSTANCE_ID);
                            if (id.compare("0") != 0) {
                                action->setStatus(702, "Invalid InstanceID");
                                return false;
                            }

                            if (actionName.compare(LIST_PRESETS) == 0)
                                return actionListPresets(action);

                            if (actionName.compare(SELECT_PRESET) == 0)
                                return actionSelectPreset(action);

                            if (actionName.compare(GET_MUTE) == 0)
                                return actionGetMute(action);

                            if (actionName.compare(SET_MUTE) == 0)
                                return actionSetMute(action);

                            if (actionName.compare(GET_VOLUME) == 0)
                                return actionGetVolume(action);

                            if (actionName.compare(SET_VOLUME) == 0)
                                return actionSetVolume(action);

                            if (actionName.compare(GET_CONTRAST) == 0)
                                return actionGetContrast(action);

                            if (actionName.compare(SET_CONTRAST) == 0)
                                return actionSetContrast(action);

                            if (actionName.compare(GET_BRIGHTNESS) == 0)
                                return actionGetBrightness(action);

                            if (actionName.compare(SET_BRIGHTNESS) == 0)
                                return actionSetBrightness(action);

                            return false;
                        }

                        bool TVRRenderingControl::actionListPresets(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionListPresets(action)!");

                            action->getUPnPArgument(CURRENT_PRESET_NAME_LIST)->setValue(getService()->getUPnPStateVariable(PRESET_NAME_LIST)->getValue());
                            return true;
                        }

                        bool TVRRenderingControl::actionSelectPreset(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionSelectPreset(action)!");

                            string presetName = action->getUPnPArgumentValue(PRESET_NAME);
                            //TODO
                            return true;
                        }

                        bool TVRRenderingControl::actionGetMute(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionGetMute(action)!");

                            string channelName = action->getUPnPArgumentValue(CHANNEL);
                            if (channelName.compare(MASTER) == 0) {
                                action->getUPnPArgument(CURRENT_MUTE)->setValue(atoi(getService()->getUPnPStateVariable(MUTE)->getValue()));
                                return true;
                            } else {
                                action->setStatus(UPNP_SOAP_E_INVALID_ARGS, "Invalid Args");
                                return false;
                            }
                        }

                        bool TVRRenderingControl::actionSetMute(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionSetMute(action)!");

                            string channelName = action->getUPnPArgumentValue(CHANNEL);
                            if (channelName.compare(MASTER) == 0) {
                                int value = atoi(action->getUPnPArgumentValue(DESIRED_MUTE));
                                if (((value == 0) &&
                                        getMediaRenderer()->getPlayerHandler()->setVolume(atoi(getService()->getUPnPStateVariable(VOLUME)->getValue()))) ||
                                        ((value == 1) &&
                                        getMediaRenderer()->getPlayerHandler()->setVolume(0))) {

                                    getService()->getUPnPStateVariable(MUTE)->setValue(value);
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(MUTE));

                                    return true;

                                } else {

                                    return false;

                                }
                            } else {
                                action->setStatus(UPNP_SOAP_E_INVALID_ARGS, "Invalid Args");
                                return false;
                            }
                        }

                        bool TVRRenderingControl::actionGetVolume(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionGetVolume(action)!");

                            string channelName = action->getUPnPArgumentValue(CHANNEL);
                            if (channelName.compare(MASTER) == 0) {
                                action->getUPnPArgument(CURRENT_VOLUME)->setValue(atoi(getService()->getUPnPStateVariable(VOLUME)->getValue()));
                                return true;
                            } else {
                                action->setStatus(UPNP_SOAP_E_INVALID_ARGS, "Invalid Args");
                                return false;
                            }
                        }

                        bool TVRRenderingControl::actionSetVolume(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionSetVolume(action)!");

                            string channelName = action->getUPnPArgumentValue(CHANNEL);
                            if (channelName.compare(MASTER) == 0) {
                                int value = atoi(action->getUPnPArgumentValue(DESIRED_VOLUME));

                                if (getMediaRenderer()->getPlayerHandler()->setVolume(value)) {
                                    getService()->getUPnPStateVariable(VOLUME)->setValue(value);
                                    Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(VOLUME));
                                    return true;
                                } else {
                                    return false;
                                }
                            } else {
                                action->setStatus(UPNP_SOAP_E_INVALID_ARGS, "Invalid Args");
                                return false;
                            }
                        }

                        bool TVRRenderingControl::actionGetContrast(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionGetContrast(action)!");

                            action->getUPnPArgument(CURRENT_CONTRAST)->setValue(atoi(getService()->getUPnPStateVariable(CONTRAST)->getValue()));

                            return true;
                        }

                        bool TVRRenderingControl::actionSetContrast(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionSetContrast(action)!");

                            int value = atoi(action->getUPnPArgumentValue(DESIRED_CONTRAST));

                            if (getMediaRenderer()->getPlayerHandler()->setContrast(value)) {
                                getService()->getUPnPStateVariable(CONTRAST)->setValue(value);
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(CONTRAST));
                                return true;
                            } else {
                                return false;
                            }
                        }

                        bool TVRRenderingControl::actionGetBrightness(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionGetBrightness(action)!");

                            action->getUPnPArgument(CURRENT_BRIGHTNESS)->setValue(atoi(getService()->getUPnPStateVariable(BRIGHTNESS)->getValue()));

                            return true;
                        }

                        bool TVRRenderingControl::actionSetBrightness(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionSetBrightness(action)!");

                            int value = atoi(action->getUPnPArgumentValue(DESIRED_BRIGHTNESS));

                            if (getMediaRenderer()->getPlayerHandler()->setBrightness(value)) {
                                getService()->getUPnPStateVariable(BRIGHTNESS)->setValue(value);
                                Utils::updateLastChangeStateVariable(getService(), getService()->getUPnPStateVariable(BRIGHTNESS));
                                return true;
                            } else {
                                return false;
                            }
                        }

                        bool TVRRenderingControl::variableRequest(UPnPStateVariable *stateVar) {
                            LoggerUtil_info(logger, "Entrando no método queryControlReceived(stateVar)!");
                            LoggerUtil_debug(logger, "stateVar nome: " << stateVar->getName() << " valor: " << stateVar->getValue());

                            UPnPStateVariable* serviceStateVar = getService()->getUPnPStateVariable(stateVar->getName());
                            if(serviceStateVar == NULL)
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
