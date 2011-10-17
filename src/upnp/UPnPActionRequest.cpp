#include "honesconfig.h"

#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <upnp/upnp.h>
#include <upnp/ActionRequest.h>
#include "upnp/UPnPActionResponse.h"
#include "upnp/UPnPArgumentList.h"
#include "upnp/UPnPAction.h"
#include "upnp/UPnPStatus.h"
#include "upnp/UPnPService.h"
#include "upnp/UPnPArgument.h"
#include "upnp/UPnPDeviceController.h"
#include "upnp/UPnPActionRequest.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        UPnPActionRequest::UPnPActionRequest() {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpactionrequest");
                            LoggerUtil_info(logger, "Entrando no construtor UPnPActionRequest()!");

                            actionRequest = NULL;
                            action = NULL;
                        }

                        UPnPActionRequest::UPnPActionRequest(UpnpActionRequestPtr actionRequest) {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpactionrequest");
                            LoggerUtil_info(logger, "Entrando no construtor UPnPActionRequest(actionRequest)!");

                            this->actionRequest = actionRequest;
                            action = NULL;
                        }

                        void UPnPActionRequest::setInputUPnPArgumentValues(UPnPAction* act) {
                            if ((actionRequest != NULL) && (act != NULL)) {
                                IXML_Document* xml = UpnpActionRequest_get_ActionRequest((UpnpActionRequest*)actionRequest);
                                int inSize = act->getInputUPnPArguments()->size();
                                for (int k = 0; k < inSize; k++) {
                                    IXML_Element* elem = ixmlDocument_getElementById(xml, act->getInputUPnPArguments()->getUPnPArgument(k)->getName());
                                    if (elem != NULL) {
                                        act->getInputUPnPArguments()->getUPnPArgument(k)->setValue(Utils::getNodeValue((IXML_Node*) elem));
                                    }
                                }
                            }
                        }

                        void UPnPActionRequest::setRequest(UPnPAction* action) {
                            this->action = action;
                        }

                        void UPnPActionRequest::send(UPnPActionResponse* res) {
                            LoggerUtil_info(logger, "Entrando no metodo send(res)");

                            if (actionRequest != NULL) {
                                LoggerUtil_debug(logger, "actionRequest != NULL \n Action:\n" << ixmlDocumenttoString(UpnpActionRequest_get_ActionRequest((UpnpActionRequest*) actionRequest)));

                                if (res->getUPnPAction() != NULL) {
                                    LoggerUtil_debug(logger, "res->getUPnPAction() != NULL");

                                    const char* serviceType = res->getUPnPAction()->getUPnPService()->getServiceType();
                                    const char* actionName = UpnpString_get_String(UpnpActionRequest_get_ActionName((UpnpActionRequest*) actionRequest));
                                    IXML_Document* result = NULL;

                                    UPnPArgumentList* list = res->getUPnPAction()->getOutputUPnPArguments();

                                    int size = list->size();

                                    for (int i = 0; i < size; i++)
                                        Utils::addArgumentToUPnPAction(true, (DocumentPtr*) &result, actionName, serviceType, list->getUPnPArgument(i)->getName(), list->getUPnPArgument(i)->getValue());

                                    if (result == NULL)
                                        Utils::addArgumentToUPnPAction(true, (DocumentPtr*) & result, actionName, serviceType, NULL, NULL);

                                    LoggerUtil_debug(logger, "action->name: " << actionName << " result:\n" << ixmlDocumenttoString(result));

                                    UpnpActionRequest_set_ActionResult((UpnpActionRequest*) actionRequest, result);
                                    UpnpActionRequest_set_ErrCode((UpnpActionRequest*) actionRequest, UPNP_E_SUCCESS);
                                }
                            } else if (action != NULL) {
                                LoggerUtil_debug(logger, "action != NULL");

                                UPnPDeviceController* deviceCtrl = UPnPDeviceController::getInstance();
                                if (deviceCtrl->isRunning() == false)
                                    deviceCtrl->start();

                                deviceCtrl->sendUPnPAction(action, res);
                            }
                        }

                        const char* UPnPActionRequest::getControlHost() {
                            if (actionRequest != NULL){
                                sockaddr* sock = UpnpActionRequest_get_CtrlPtIPAddr((UpnpActionRequest*)actionRequest);
                                if( sock->sa_family == AF_INET ){
                                    char ip[INET_ADDRSTRLEN];
                                    inet_ntop(AF_INET, &(((sockaddr_in*)sock)->sin_addr), ip, INET_ADDRSTRLEN);
                                    return strdup(ip);
                                } else if (sock->sa_family == AF_INET6 ){
                                    char ip[INET6_ADDRSTRLEN];
                                    inet_ntop(AF_INET6, &(((sockaddr_in6*)sock)->sin6_addr), ip, INET6_ADDRSTRLEN);
                                    return strdup(ip);
                                }
                            }
                            return NULL;
                        }

                    }
                }
            }
        }
    }
}
