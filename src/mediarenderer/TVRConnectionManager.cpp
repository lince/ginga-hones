#include "honesconfig.h"

#include "mediarenderer/TVRenderer.h"
#include "mediarenderer/TVRConnectionManager.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediarenderer {

                        const char *TVRConnectionManager::SERVICE_TYPE = "urn:schemas-upnp-org:service:ConnectionManager:1";

                        const char *TVRConnectionManager::GET_PROTOCOL_INFO = "GetProtocolInfo";
                        const char *TVRConnectionManager::SOURCE = "Source";
                        const char *TVRConnectionManager::SINK = "Sink";
                        const char *TVRConnectionManager::SOURCE_PROTOCOL_INFO = "SourceProtocolInfo";
                        const char *TVRConnectionManager::SINK_PROTOCOL_INFO = "SinkProtocolInfo";
                        const char *TVRConnectionManager::CURRENT_CONNECTION_IDS = "CurrentConnectionIDs";

                        const char *TVRConnectionManager::PREPARE_FOR_CONNECTION = "PrepareForConnection";
                        const char *TVRConnectionManager::REMOTE_PROTOCOL_INFO = "RemoteProtocolInfo";
                        const char *TVRConnectionManager::PEER_CONNECTION_MANAGER = "PeerConnectionManager";
                        const char *TVRConnectionManager::PEER_CONNECTION_ID = "PeerConnectionID";
                        const char *TVRConnectionManager::DIRECTION = "Direction";
                        const char *TVRConnectionManager::CONNECTION_ID = "ConnectionID";
                        const char *TVRConnectionManager::AV_TRNSPORT_ID = "AVTransportID";
                        const char *TVRConnectionManager::RCS_ID = "RcsID";

                        const char *TVRConnectionManager::CONNECTION_COMPLETE = "ConnectionComplete";

                        const char *TVRConnectionManager::GET_CURRENT_CONNECTION_IDS = "GetCurrentConnectionIDs";
                        const char *TVRConnectionManager::CONNECTION_IDS = "ConnectionIDs";

                        const char *TVRConnectionManager::GET_CURRENT_CONNECTION_INFO = "GetCurrentConnectionInfo";
                        const char *TVRConnectionManager::PROTOCOL_INFO = "ProtocolInfo";
                        const char *TVRConnectionManager::STATUS = "Status";
                        const char *TVRConnectionManager::INPUT = "Input";
                        const char *TVRConnectionManager::OUTPUT = "Output";
                        const char *TVRConnectionManager::OK = "OK";

                        const char *TVRConnectionManager::SCPD =
                                "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
                                "<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">\n"
                                "   <specVersion>\n"
                                "      <major>1</major>\n"
                                "      <minor>0</minor>\n"
                                "	</specVersion>\n"
                                "	<actionList>\n"
                                "	<action>\n"
                                "         <name>GetCurrentConnectionInfo</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>ConnectionID</name>\n"
                                "               <direction>in</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_ConnectionID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>RcsID</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_RcsID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>AVTransportID</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_AVTransportID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>ProtocolInfo</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_ProtocolInfo</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>PeerConnectionManager</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_ConnectionManager</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>PeerConnectionID</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_ConnectionID</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Direction</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_Direction</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Status</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>A_ARG_TYPE_ConnectionStatus</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>GetProtocolInfo</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>Source</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>SourceProtocolInfo</relatedStateVariable>\n"
                                "            </argument>\n"
                                "            <argument>\n"
                                "               <name>Sink</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>SinkProtocolInfo</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "      <action>\n"
                                "         <name>GetCurrentConnectionIDs</name>\n"
                                "         <argumentList>\n"
                                "            <argument>\n"
                                "               <name>ConnectionIDs</name>\n"
                                "               <direction>out</direction>\n"
                                "               <relatedStateVariable>CurrentConnectionIDs</relatedStateVariable>\n"
                                "            </argument>\n"
                                "         </argumentList>\n"
                                "      </action>\n"
                                "   </actionList>\n"
                                "   <serviceStateTable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>A_ARG_TYPE_ProtocolInfo</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>A_ARG_TYPE_ConnectionStatus</name>\n"
                                "         <dataType>string</dataType>\n"
                                "         <allowedValueList>\n"
                                "            <allowedValue>OK</allowedValue>\n"
                                "            <allowedValue>ContentFormatMismatch</allowedValue>\n"
                                "            <allowedValue>InsufficientBandwidth</allowedValue>\n"
                                "            <allowedValue>UnreliableChannel</allowedValue>\n"
                                "            <allowedValue>Unknown</allowedValue>\n"
                                "         </allowedValueList>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>A_ARG_TYPE_AVTransportID</name>\n"
                                "         <dataType>i4</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>A_ARG_TYPE_RcsID</name>\n"
                                "         <dataType>i4</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>A_ARG_TYPE_ConnectionID</name>\n"
                                "         <dataType>i4</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>A_ARG_TYPE_ConnectionManager</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"yes\">\n"
                                "         <name>SourceProtocolInfo</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"yes\">\n"
                                "         <name>SinkProtocolInfo</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"no\">\n"
                                "         <name>A_ARG_TYPE_Direction</name>\n"
                                "         <dataType>string</dataType>\n"
                                "         <allowedValueList>\n"
                                "            <allowedValue>Input</allowedValue>\n"
                                "            <allowedValue>Output</allowedValue>\n"
                                "         </allowedValueList>\n"
                                "      </stateVariable>\n"
                                "      <stateVariable sendEvents=\"yes\">\n"
                                "         <name>CurrentConnectionIDs</name>\n"
                                "         <dataType>string</dataType>\n"
                                "      </stateVariable>\n"
                                "   </serviceStateTable>\n"
                                "</scpd>";

                        TVRConnectionManager::TVRConnectionManager(TVRenderer *tvr) {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.mediarenderer.tvrconnectionmanager");

                            LoggerUtil_info(logger, "Entrando no construtor TVRConnectionManager(tvr)!");

                            setMediaRenderer(tvr);

                            UPnPService *serv = getService();

                            LoggerUtil_debug(logger, "Passou pelo getService() do construtor TVRConnectionManager(tvr)! ");

                            bool ret = serv->loadSCPD(SCPD);
                            if (ret == false)
                                throw "Invalid description";
                            serv->setUPnPActionListener(this);
                            serv->setUPnPQueryListener(this);

                            LoggerUtil_info(logger, "Saindo do construtor TVRConnectionManager(tvr)!");
                        }

                        TVRConnectionManager::~TVRConnectionManager() {
                        }

                        void TVRConnectionManager::initializeStateVariables() {
                            ostringstream sinkProtocol;
                            int length = Utils::getDLNAProfilesLength();
                            for (int i = 0; i < length; i++) {
                                if (i > 0)
                                    sinkProtocol << ",";
                                sinkProtocol << "http-get:*:" << Utils::getDLNAProfiles()[i][0] << ":DLNA.ORG_PN=" << Utils::getDLNAProfiles()[i][1];
                            }

                            getService()->getUPnPStateVariable(CURRENT_CONNECTION_IDS)->setValue(""); // "" ou "0"
                            getService()->getUPnPStateVariable(SINK_PROTOCOL_INFO)->setValue(sinkProtocol.str().c_str()); // "http-get:*:*:*,rtsp:*:*:*,http-get:*:video/mpeg:*,http-get:*:audio/mpeg:*,http-get:*:image/jpeg:*,http-get:*:image/png:*"
                            getService()->getUPnPStateVariable(SOURCE_PROTOCOL_INFO)->setValue("");
                        }

                        TVRenderer* TVRConnectionManager::getMediaRenderer() {
                            return tvRender;
                        }

                        void TVRConnectionManager::setMediaRenderer(TVRenderer *tvr) {
                            tvRender = tvr;
                        }

                        UPnPService *TVRConnectionManager::getService() {
                            return getMediaRenderer()->getUPnPService(SERVICE_TYPE);
                        }

                        bool TVRConnectionManager::actionRequest(UPnPAction *action) {
                            string actionName = action->getName();

                            if (actionName.compare(GET_PROTOCOL_INFO) == 0)
                                return actionGetProtocolInfo(action);
                            
                            if (actionName.compare(GET_CURRENT_CONNECTION_INFO) == 0)
                                return actionGetCurrentConnectionInfo(action);

                            if (actionName.compare(GET_CURRENT_CONNECTION_IDS) == 0)
                                return actionGetCurrentConnectionIDs(action);

                            return false;
                        }

                        bool TVRConnectionManager::actionGetProtocolInfo(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionGetProtocolInfo(action)!");

                            // Source
                            action->getUPnPArgument(SOURCE)->setValue(getService()->getUPnPStateVariable(SOURCE_PROTOCOL_INFO)->getValue());
                            // Sink
                            action->getUPnPArgument(SINK)->setValue(getService()->getUPnPStateVariable(SINK_PROTOCOL_INFO)->getValue());
                            return true;
                        }

                        bool TVRConnectionManager::actionGetCurrentConnectionInfo(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionGetCurrentConnectionInfo(action)!");

                            action->getUPnPArgument(RCS_ID)->setValue(0);
                            action->getUPnPArgument(AV_TRNSPORT_ID)->setValue(0);
                            action->getUPnPArgument(PROTOCOL_INFO)->setValue(""); // ":::" ou "http-get:*:*:*" ou ""
                            action->getUPnPArgument(PEER_CONNECTION_MANAGER)->setValue(""); // "/" ou ""
                            action->getUPnPArgument(PEER_CONNECTION_ID)->setValue(-1);
                            action->getUPnPArgument(DIRECTION)->setValue(INPUT);
                            action->getUPnPArgument(STATUS)->setValue(OK);
                            return true;
                        }

                        bool TVRConnectionManager::actionGetCurrentConnectionIDs(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no método actionGetCurrentConnectionIDs(action)!");

                            action->getUPnPArgument(CONNECTION_IDS)->setValue(getService()->getUPnPStateVariable(CURRENT_CONNECTION_IDS)->getValue());
                            return true;
                        }

                        bool TVRConnectionManager::variableRequest(UPnPStateVariable *stateVar) {
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
