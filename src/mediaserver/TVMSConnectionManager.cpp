#include "honesconfig.h"

#include <sstream>
#include "mediaserver/TVMediaServer.h"
#include "mediaserver/TVMSConnectionManager.h"
#include "util/Utils.h"
using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediaserver {

                        const char *TVMSConnectionManager::SERVICE_TYPE = "urn:schemas-upnp-org:service:ConnectionManager:1";

                        const char *TVMSConnectionManager::GET_PROTOCOL_INFO = "GetProtocolInfo";
                        const char *TVMSConnectionManager::SOURCE = "Source";
                        const char *TVMSConnectionManager::SINK = "Sink";
                        const char *TVMSConnectionManager::SOURCE_PROTOCOL_INFO = "SourceProtocolInfo";
                        const char *TVMSConnectionManager::SINK_PROTOCOL_INFO = "SinkProtocolInfo";
                        const char *TVMSConnectionManager::CURRENT_CONNECTION_IDS = "CurrentConnectionIDs";

                        const char *TVMSConnectionManager::PEER_CONNECTION_MANAGER = "PeerConnectionManager";
                        const char *TVMSConnectionManager::PEER_CONNECTION_ID = "PeerConnectionID";
                        const char *TVMSConnectionManager::DIRECTION = "Direction";
                        const char *TVMSConnectionManager::AV_TRNSPORT_ID = "AVTransportID";
                        const char *TVMSConnectionManager::RCS_ID = "RcsID";

                        const char *TVMSConnectionManager::GET_CURRENT_CONNECTION_IDS = "GetCurrentConnectionIDs";
                        const char *TVMSConnectionManager::CONNECTION_IDS = "ConnectionIDs";

                        const char *TVMSConnectionManager::GET_CURRENT_CONNECTION_INFO = "GetCurrentConnectionInfo";
                        const char *TVMSConnectionManager::PROTOCOL_INFO = "ProtocolInfo";
                        const char *TVMSConnectionManager::STATUS = "Status";
                        const char *TVMSConnectionManager::OUTPUT = "Output";
                        const char *TVMSConnectionManager::OK = "OK";

                        const char *TVMSConnectionManager::SCPD =
                                "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">"
                                "  <specVersion>"
                                "    <major>1</major>"
                                "    <minor>0</minor>"
                                "  </specVersion>"
                                "  <actionList>"
                                "    <action>"
                                "      <name>GetCurrentConnectionInfo</name>"
                                "      <argumentList>"
                                "        <argument>"
                                "          <name>ConnectionID</name>"
                                "          <direction>in</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_ConnectionID</relatedStateVariable>"
                                "        </argument>"
                                "        <argument>"
                                "          <name>RcsID</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_RcsID</relatedStateVariable>"
                                "        </argument>"
                                "        <argument>"
                                "          <name>AVTransportID</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_AVTransportID</relatedStateVariable>"
                                "        </argument>"
                                "        <argument>"
                                "          <name>ProtocolInfo</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_ProtocolInfo</relatedStateVariable>"
                                "        </argument>"
                                "        <argument>"
                                "          <name>PeerConnectionManager</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_ConnectionManager</relatedStateVariable>"
                                "        </argument>"
                                "        <argument>"
                                "          <name>PeerConnectionID</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_ConnectionID</relatedStateVariable>"
                                "        </argument>"
                                "        <argument>"
                                "          <name>Direction</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_Direction</relatedStateVariable>"
                                "        </argument>"
                                "        <argument>"
                                "          <name>Status</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_ConnectionStatus</relatedStateVariable>"
                                "        </argument>"
                                "      </argumentList>"
                                "    </action>"
                                "    <action>"
                                "      <name>GetProtocolInfo</name>"
                                "      <argumentList>"
                                "        <argument>"
                                "          <name>Source</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>SourceProtocolInfo</relatedStateVariable>"
                                "        </argument>"
                                "        <argument>"
                                "          <name>Sink</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>SinkProtocolInfo</relatedStateVariable>"
                                "        </argument>"
                                "      </argumentList>"
                                "    </action>"
                                "    <action>"
                                "      <name>GetCurrentConnectionIDs</name>"
                                "      <argumentList>"
                                "        <argument>"
                                "          <name>ConnectionIDs</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>CurrentConnectionIDs</relatedStateVariable>"
                                "        </argument>"
                                "      </argumentList>"
                                "    </action>"
                                "  </actionList>"
                                "  <serviceStateTable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>A_ARG_TYPE_ProtocolInfo</name>"
                                "      <dataType>string</dataType>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>A_ARG_TYPE_ConnectionStatus</name>"
                                "     <dataType>string</dataType>"
                                "      <allowedValueList>"
                                "        <allowedValue>OK</allowedValue>"
                                "        <allowedValue>ContentFormatMismatch</allowedValue>"
                                "        <allowedValue>InsufficientBandwidth</allowedValue>"
                                "        <allowedValue>UnreliableChannel</allowedValue>"
                                "        <allowedValue>Unknown</allowedValue>"
                                "      </allowedValueList>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>A_ARG_TYPE_AVTransportID</name>"
                                "      <dataType>i4</dataType>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>A_ARG_TYPE_RcsID</name>"
                                "      <dataType>i4</dataType>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>A_ARG_TYPE_ConnectionID</name>"
                                "      <dataType>i4</dataType>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>A_ARG_TYPE_ConnectionManager</name>"
                                "      <dataType>string</dataType>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"yes\">"
                                "      <name>SourceProtocolInfo</name>"
                                "      <dataType>string</dataType>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"yes\">"
                                "      <name>SinkProtocolInfo</name>"
                                "      <dataType>string</dataType>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>A_ARG_TYPE_Direction</name>"
                                "      <dataType>string</dataType>"
                                "      <allowedValueList>"
                                "        <allowedValue>Input</allowedValue>"
                                "        <allowedValue>Output</allowedValue>"
                                "      </allowedValueList>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"yes\">"
                                "      <name>CurrentConnectionIDs</name>"
                                "      <dataType>string</dataType>"
                                "    </stateVariable>"
                                "  </serviceStateTable>"
                                "</scpd>";

                        TVMSConnectionManager::TVMSConnectionManager(TVMediaServer* tvs) {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.mediarenderer.tvmsconnectionmanager");
                            LoggerUtil_info(logger, "Entrando no construtor TVMSConnectionManager(tvs)!");

                            setMediaServer(tvs);

                            UPnPService* serv = getService();

                            bool ret = serv->loadSCPD(SCPD);
                            if (ret == false)
                                throw "Invalid description";
                            serv->setUPnPActionListener(this);
                            serv->setUPnPQueryListener(this);

                            initializeUPnPStateVariables();

                            LoggerUtil_info(logger, "Saindo do construtor TVMSConnectionManager(tvs)!");
                        }

                        TVMSConnectionManager::~TVMSConnectionManager() {
                        }

                        void TVMSConnectionManager::initializeUPnPStateVariables(){
                            ostringstream sourceProtocol;
                            int length = Utils::getDLNAProfilesLength();
                            for (int i = 0; i < length; i++) {
                                if (i > 0)
                                    sourceProtocol << ",";
                                sourceProtocol << "http-get:*:" << Utils::getDLNAProfiles()[i][0] << ":DLNA.ORG_PN=" << Utils::getDLNAProfiles()[i][1];
                            }

                            getService()->getUPnPStateVariable(CURRENT_CONNECTION_IDS)->setValue("0");
                            getService()->getUPnPStateVariable(SINK_PROTOCOL_INFO)->setValue("");
                            getService()->getUPnPStateVariable(SOURCE_PROTOCOL_INFO)->setValue(sourceProtocol.str().c_str());
                        }

                        void TVMSConnectionManager::setMediaServer(TVMediaServer* tvs) {
                            tvServer = tvs;
                        }

                        TVMediaServer* TVMSConnectionManager::getMediaServer() {
                            return tvServer;
                        }

                        UPnPService *TVMSConnectionManager::getService() {
                            return getMediaServer()->getUPnPService(SERVICE_TYPE);
                        }

                        bool TVMSConnectionManager::actionRequest(UPnPAction *action) {
                            string actionName = action->getName();

                            if (actionName.compare(GET_PROTOCOL_INFO) == 0)
                                return actionGetProtocolInfo(action);

                            if (actionName.compare(GET_CURRENT_CONNECTION_INFO) == 0)
                                return actionGetCurrentConnectionInfo(action);

                            if (actionName.compare(GET_CURRENT_CONNECTION_IDS) == 0)
                                return actionGetCurrentConnectionIDs(action);

                            return false;
                        }

                        bool TVMSConnectionManager::actionGetProtocolInfo(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no metodo actionGetProtocolInfo(action)!");

                            action->getUPnPArgument(SOURCE)->setValue(getService()->getUPnPStateVariable(SOURCE_PROTOCOL_INFO)->getValue());
                            action->getUPnPArgument(SINK)->setValue(getService()->getUPnPStateVariable(SINK_PROTOCOL_INFO)->getValue());
                            return true;
                        }

                        bool TVMSConnectionManager::actionGetCurrentConnectionInfo(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no metodo actionGetCurrentConnectionInfo(action)!");

                            action->getUPnPArgument(RCS_ID)->setValue(-1);
                            action->getUPnPArgument(AV_TRNSPORT_ID)->setValue(-1);
                            action->getUPnPArgument(PROTOCOL_INFO)->setValue("");
                            action->getUPnPArgument(PEER_CONNECTION_MANAGER)->setValue("");
                            action->getUPnPArgument(PEER_CONNECTION_ID)->setValue(0);
                            action->getUPnPArgument(DIRECTION)->setValue(OUTPUT);
                            action->getUPnPArgument(STATUS)->setValue(OK);
                            return true;
                        }

                        bool TVMSConnectionManager::actionGetCurrentConnectionIDs(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no metodo actionGetCurrentConnectionIDs(action)!");

                            action->getUPnPArgument(CONNECTION_IDS)->setValue(getService()->getUPnPStateVariable(CURRENT_CONNECTION_IDS)->getValue());
                            return true;
                        }

                        bool TVMSConnectionManager::variableRequest(UPnPStateVariable *stateVar) {
                            LoggerUtil_info(logger, "Entrando no metodo variableRequest(stateVar)!");

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