#include "honesconfig.h"

#include <upnp/ixml.h>
#include <upnp/upnp.h>
#include "upnp/UPnPService.h"
#include "upnp/UPnPArgumentList.h"
#include "upnp/UPnPArgument.h"
#include "upnp/UPnPActionListener.h"
#include "upnp/UPnPActionResponse.h"
#include "upnp/UPnPActionRequest.h"
#include "upnp/UPnPAction.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {
                        const string UPnPAction::ELEMENT_NAME = "action";
                        const string UPnPAction::NAME = "name";

                        UPnPAction::UPnPAction(UPnPService* service, NodePtr node) {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpaction");
                            LoggerUtil_info(logger, "Entrando no construtor UPnPAction(service, node)!");

                            setUPnPService(service);
                            setNode(node);
                            initializeUPnPArgumentList();
                        }

                        UPnPAction::~UPnPAction() {
                            LoggerUtil_info(logger, "Entrando no destrutor ~UPnPAction()!");
                            
                            delete arguments;
                        }

                        UPnPService* UPnPAction::getUPnPService() {
                            return service;
                        }

                        void UPnPAction::setUPnPService(UPnPService* service) {
                            this->service = service;
                        }

                        NodePtr UPnPAction::getNode() {
                            return node;
                        }

                        bool UPnPAction::isUPnPActionNode(NodePtr node) {
                            return (ELEMENT_NAME.compare( ((IXML_Node*)node)->nodeName) == 0);
                        }

                        void UPnPAction::setName(const char* value) {
                            Utils::setNode(getNode(), NAME, value);
                        }

                        const char* UPnPAction::getName() {
                            return Utils::getNodeValue(getNode(), NAME);
                        }

                        UPnPArgumentList* UPnPAction::getUPnPArguments() {
                            return arguments;
                        }

                        UPnPArgumentList* UPnPAction::getInputUPnPArguments() {
                            return argumentsIn;
                        }

                        UPnPArgumentList* UPnPAction::getOutputUPnPArguments() {
                            return argumentsOut;
                        }

                        UPnPArgument* UPnPAction::getUPnPArgument(const char* name) {
                            return getUPnPArguments()->getUPnPArgument(name);
                        }

                        void UPnPAction::setUPnPArgumentValues(UPnPArgumentList* list) {
                            getUPnPArguments()->setUPnPArgumentListValues(list);
                        }

                        void UPnPAction::setUPnPArgumentValue(const char* name, const char* value) {
                            UPnPArgument* arg = getUPnPArgument(name);
                            if (arg != NULL) {
                                arg->setValue(value);
                            }
                        }

                        void UPnPAction::setUPnPArgumentValue(const char* name, int value) {
                            UPnPArgument* arg = getUPnPArgument(name);
                            if (arg != NULL) {
                                arg->setValue(value);
                            }
                        }

                        const char* UPnPAction::getUPnPArgumentValue(const char* name) {
                            UPnPArgument* arg = getUPnPArgument(name);
                            if (arg != NULL) {
                                return arg->getValue();
                            } else {
                                return "";
                            }
                        }

                        int UPnPAction::getUPnPArgumentIntegerValue(const char* name) {
                            UPnPArgument* arg = getUPnPArgument(name);
                            if (arg != NULL) {
                                return arg->getIntegerValue();
                            } else {
                                return 0;
                            }
                        }

                        UPnPActionListener* UPnPAction::getUPnPActionListener() {
                            return actionListener;
                        }

                        void UPnPAction::setUPnPActionListener(UPnPActionListener* listener) {
                            this->actionListener = listener;
                        }

                        bool UPnPAction::executeUPnPActionListener(UPnPActionRequest* req) {
                            UPnPActionListener* listener = getUPnPActionListener();
                            if (listener != NULL) {
                                UPnPActionResponse res;
                                setStatus(UPNP_E_SUCCESS);
                                clearOutputUPnPArgumentValues();
                                req->setInputUPnPArgumentValues(this);
                                setControlHost(req->getControlHost());
                                if (listener->actionRequest(this) == true) {
                                    res.setUPnPAction(this);
                                } else {
                                    res.setFaultResponse(getStatus()->getCode(), getStatus()->getDescription());
                                }
                                req->send(&res);
                                return true;
                            } else {
                                return false;
                            }
                        }

                        bool UPnPAction::sendUPnPAction() {
                            LoggerUtil_info(logger, "Entrando no metodo sendUPnPAction()");
                            
                            UPnPArgumentList* args = getUPnPArguments();
                            UPnPActionRequest req;
                            req.setRequest(this);
                            UPnPActionResponse res;
                            req.send(&res);
                            setStatus(res.getStatus()->getCode());
                            if (res.isSuccessful()) {
                                args->setUPnPArgumentListValues(res.getResponse());
                                return true;
                            } else {
                                return false;
                            }
                        }

                        void UPnPAction::setStatus(int code, const char *desc) {
                            status.setCode(code);
                            status.setDescription(desc);
                        }

                        void UPnPAction::setStatus(int code) {
                            setStatus(code, UPnPStatus::codeToString(code));
                        }

                        UPnPStatus* UPnPAction::getStatus() {
                            return &status;
                        }

                        void UPnPAction::setControlHost(const char* ip){
                            if(ip != NULL)
                                controlHost = ip;
                        }

                        const char* UPnPAction::getControlHost(){
                            return controlHost.c_str();
                        }

                        void UPnPAction::setNode(NodePtr node) {
                            this->node = node;
                        }

                        void UPnPAction::initializeUPnPArgumentList() {
                            arguments = new UPnPArgumentList();
                            argumentsIn = new UPnPArgumentList();
                            argumentsOut = new UPnPArgumentList();

                            IXML_Node* argumentListNode = ((IXML_Node*)getNode())->firstChild;
                            while (argumentListNode != NULL) {
                                if (UPnPArgumentList::isUPnPArgumentListNode(argumentListNode)) {
                                    IXML_Node* argumentNode = argumentListNode->firstChild;
                                    while (argumentNode != NULL) {
                                        if (UPnPArgument::isUPnPArgumentNode(argumentNode)) {
                                            UPnPArgument* arg = new UPnPArgument(argumentNode);
                                            arg->setUPnPService(getUPnPService());
                                            arguments->add(arg);
                                            if (arg->isInDirection())
                                                argumentsIn->add(arg);
                                            else
                                                argumentsOut->add(arg);
                                        }
                                        argumentNode = argumentNode->nextSibling;
                                    }
                                    return;
                                }
                                argumentListNode = argumentListNode->nextSibling;
                            }
                        }

                        void UPnPAction::clearOutputUPnPArgumentValues() {
                            UPnPArgumentList* list = getOutputUPnPArguments();
                            int size = list->size();
                            for (int i = 0; i < size; i++) {
                                UPnPArgument* arg = list->getUPnPArgument(i);
                                if (arg != NULL) {
                                    arg->setValue("");
                                }
                            }
                        }

                    }
                }
            }
        }
    }
}
