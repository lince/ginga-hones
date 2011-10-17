#include "honesconfig.h"

#include <string>
#include "upnp/UPnPDevice.h"
#include "upnp/UPnPAction.h"
#include "upnp/UPnPArgument.h"
#include "upnp/UPnPService.h"
#include "upnp/UPnPRegistrarService.h"
using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        const char *UPnPRegistrarService::SERVICE_TYPE = "urn:microsoft.com:service:X_MS_MediaReceiverRegistrar:1";
                        const char *UPnPRegistrarService::SCPD =
                                "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">"
                                "<specVersion>"
                                "  <major>1</major>"
                                "  <minor>0</minor>"
                                "</specVersion>"
                                "<actionList>"
                                "  <action>"
                                "    <name>IsAuthorized</name>"
                                "    <argumentList>"
                                "      <argument>"
                                "        <name>DeviceID</name>"
                                "        <direction>in</direction>"
                                "        <relatedStateVariable>A_ARG_TYPE_DeviceID</relatedStateVariable>"
                                "      </argument>"
                                "      <argument>"
                                "        <name>Result</name>"
                                "        <direction>out</direction>"
                                "        <relatedStateVariable>A_ARG_TYPE_Result</relatedStateVariable>"
                                "      </argument>"
                                "    </argumentList>"
                                "  </action>"
                                "  <action>"
                                "    <name>RegisterDevice</name>"
                                "    <argumentList>"
                                "      <argument>"
                                "        <name>RegistrationReqMsg</name>"
                                "        <direction>in</direction>"
                                "        <relatedStateVariable>A_ARG_TYPE_RegistrationReqMsg</relatedStateVariable>"
                                "      </argument>"
                                "      <argument>"
                                "        <name>RegistrationRespMsg</name>"
                                "        <direction>out</direction>"
                                "        <relatedStateVariable>A_ARG_TYPE_RegistrationRespMsg</relatedStateVariable>"
                                "      </argument>"
                                "    </argumentList>"
                                "  </action>"
                                "  <action>"
                                "    <name>IsValidated</name>"
                                "    <argumentList>"
                                "      <argument>"
                                "        <name>DeviceID</name>"
                                "        <direction>in</direction>"
                                "        <relatedStateVariable>A_ARG_TYPE_DeviceID</relatedStateVariable>"
                                "      </argument>"
                                "      <argument>"
                                "        <name>Result</name>"
                                "        <direction>out</direction>"
                                "        <relatedStateVariable>A_ARG_TYPE_Result</relatedStateVariable>"
                                "      </argument>"
                                "    </argumentList>"
                                "  </action>"
                                "</actionList>"
                                "<serviceStateTable>"
                                "  <stateVariable sendEvents=\"no\">"
                                "    <name>A_ARG_TYPE_DeviceID</name>"
                                "    <dataType>string</dataType>"
                                "  </stateVariable>"
                                "  <stateVariable sendEvents=\"no\">"
                                "    <name>A_ARG_TYPE_Result</name>"
                                "    <dataType>int</dataType>"
                                "  </stateVariable>"
                                "  <stateVariable sendEvents=\"no\">"
                                "    <name>A_ARG_TYPE_RegistrationReqMsg</name>"
                                "    <dataType>bin.base64</dataType>"
                                "  </stateVariable>"
                                "  <stateVariable sendEvents=\"no\">"
                                "    <name>A_ARG_TYPE_RegistrationRespMsg</name>"
                                "    <dataType>bin.base64</dataType>"
                                "  </stateVariable>"
                                "  <stateVariable sendEvents=\"yes\">"
                                "    <name>AuthorizationGrantedUpdateID</name>"
                                "    <dataType>ui4</dataType>"
                                "  </stateVariable>"
                                "  <stateVariable sendEvents=\"yes\">"
                                "    <name>AuthorizationDeniedUpdateID</name>"
                                "    <dataType>ui4</dataType>"
                                "  </stateVariable>"
                                "  <stateVariable sendEvents=\"yes\">"
                                "    <name>ValidationSucceededUpdateID</name>"
                                "    <dataType>ui4</dataType>"
                                "  </stateVariable>"
                                "  <stateVariable sendEvents=\"yes\">"
                                "    <name>ValidationRevokedUpdateID</name>"
                                "    <dataType>ui4</dataType>"
                                "  </stateVariable>"
                                "</serviceStateTable>"
                                "</scpd>";

                        UPnPRegistrarService::UPnPRegistrarService(UPnPDevice* device) {
                            this->device = device;

                            UPnPService* serv = getService();
                            bool ret = serv->loadSCPD(SCPD);
                            if (ret == false)
                                throw "Invalid description";
                            serv->setUPnPActionListener(this);
                            serv->setUPnPQueryListener(this);

                            initializeUPnPStateVariables();
                        }

                        UPnPRegistrarService::~UPnPRegistrarService() {
                        }

                        UPnPDevice* UPnPRegistrarService::getDevice() {
                            return device;
                        }

                        UPnPService* UPnPRegistrarService::getService() {
                            return getDevice()->getUPnPService(SERVICE_TYPE);
                        }

                        void UPnPRegistrarService::initializeUPnPStateVariables() {
                            getService()->getUPnPStateVariable("AuthorizationGrantedUpdateID")->setValue(0);
                            getService()->getUPnPStateVariable("AuthorizationDeniedUpdateID")->setValue(0);
                            getService()->getUPnPStateVariable("ValidationSucceededUpdateID")->setValue(0);
                            getService()->getUPnPStateVariable("ValidationRevokedUpdateID")->setValue(0);
                        }

                        bool UPnPRegistrarService::actionRequest(UPnPAction *action) {
                            string actionName = action->getName();

                            if (actionName.compare("IsAuthorized") == 0) {
                                action->getUPnPArgument("Result")->setValue("1");
                                return true;
                            }

                            if (actionName.compare("RegisterDevice") == 0) {
                                action->getUPnPArgument("RegistrationRespMsg")->setValue(" ");
                                return true;
                            }

                            if (actionName.compare("IsValidated") == 0) {
                                action->getUPnPArgument("Result")->setValue("1");
                                return true;
                            }

                            return false;
                        }

                        bool UPnPRegistrarService::variableRequest(UPnPStateVariable *stateVar) {
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
