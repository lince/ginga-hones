#include "honesconfig.h"

#include <string.h>
#include <stdlib.h>
#include <upnp/upnp.h>
#include <upnp/ixml.h>
#include "upnp/UPnPService.h"
#include "upnp/UPnPQueryListener.h"
#include "upnp/UPnPQueryRequest.h"
#include "upnp/UPnPQueryResponse.h"
#include "upnp/UPnPAllowedValue.h"
#include "upnp/UPnPStateVariable.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        const string UPnPStateVariable::ELEMENT_NAME = "stateVariable";
                        const string UPnPStateVariable::NAME = "name";
                        const string UPnPStateVariable::DATATYPE = "dataType";
                        const string UPnPStateVariable::SENDEVENTS = "sendEvents";
                        const string UPnPStateVariable::YES = "yes";
                        const string UPnPStateVariable::NO = "no";

                        UPnPStateVariable::UPnPStateVariable() {
                            varValue = "";
                            setUPnPService(NULL);
                            IXML_Node* node = (IXML_Node*) malloc(sizeof(IXML_Node));
                            memset(node, 0, sizeof(IXML_Node));
                            setNode(node);
                            allowedValueRange = NULL;
                            initializeUPnPAllowedValueList();
                            initializeUPnPAllowedValueRange();
                        }

                        UPnPStateVariable::UPnPStateVariable(NodePtr node) {
                            varValue = "";
                            setUPnPService(NULL);
                            setNode(node);
                            allowedValueRange = NULL;
                            initializeUPnPAllowedValueList();
                            initializeUPnPAllowedValueRange();
                        }

                        UPnPStateVariable::~UPnPStateVariable() {
                            if (queryResponse != NULL)
                                delete queryResponse;
                            allowedValueList.clear();
                            if(allowedValueRange != NULL)
                                delete allowedValueRange;
                            if (node != NULL){
                                ixmlNode_free((IXML_Node*)node);
                                node = NULL;
                            }
                        }

                        UPnPService* UPnPStateVariable::getUPnPService() {
                            return service;
                        }

                        void UPnPStateVariable::setUPnPService(UPnPService* service) {
                            this->service = service;
                        }

                        NodePtr UPnPStateVariable::getNode() {
                            return node;
                        }

                        bool UPnPStateVariable::isUPnPStateVariableNode(NodePtr node) {
                            return (ELEMENT_NAME.compare( ((IXML_Node*)node)->nodeName) == 0);
                        }

                        UPnPAllowedValueList* UPnPStateVariable::getUPnPAllowedValueList() {
                            return &allowedValueList;
                        }

                        bool UPnPStateVariable::hasUPnPAllowedValueList() {
                            return (allowedValueList.size() > 0);
                        }

                        UPnPAllowedValueRange* UPnPStateVariable::getUPnPAllowedValueRange() {
                            if ((allowedValueRange != NULL) && (allowedValueRange->hasValueRange()))
                                return allowedValueRange;
                            else
                                return NULL;
                        }

                        bool UPnPStateVariable::hasUPnPAllowedValueRange() {
                            return (getUPnPAllowedValueRange() != NULL);
                        }

                        void UPnPStateVariable::setName(const char* value) {
                            Utils::setNode(getNode(), NAME, value);
                        }

                        const char* UPnPStateVariable::getName() {
                            return Utils::getNodeValue(getNode(), NAME);
                        }

                        void UPnPStateVariable::setDataType(const char* value) {
                            Utils::setNode(getNode(), DATATYPE, value);
                        }

                        const char* UPnPStateVariable::getDataType() {
                            return Utils::getNodeValue(getNode(), DATATYPE);
                        }

                        void UPnPStateVariable::setSendEvents(bool state) {
                            Utils::setAttribute(getNode(), SENDEVENTS, state == true ? YES.c_str() : NO.c_str());
                        }

                        bool UPnPStateVariable::isSendEvents() {
                            const char* events = Utils::getAttributeValue(getNode(), SENDEVENTS);
                            return Utils::equalsIgnoreCase(YES.c_str(), events);
                        }

                        void UPnPStateVariable::setValue(const char* value) {
                            string current = getValue();
                            if ((value != NULL) && (current.compare(value) != 0)) {
                                varValue = value;

                                UPnPService* serv = getUPnPService();
                                if ((serv != NULL) && (isSendEvents())) {
                                    serv->sendEventMessage(this);
                                }
                            }
                        }

                        void UPnPStateVariable::setValue(int value) {
                            setValue(Utils::integerToString(value));
                        }

                        void UPnPStateVariable::setValue(long value) {
                            setValue(Utils::longToString(value));
                        }

                        const char* UPnPStateVariable::getValue() {
                            if (varValue.length() > 0)
                                return varValue.c_str();
                            else
                                return Utils::getNodeValue(getNode());
                        }

                        void UPnPStateVariable::setUPnPStateVariable(UPnPStateVariable* var) {
                            setName(var->getName());
                            setValue(var->getValue());
                            setDataType(var->getDataType());
                            setSendEvents(var->isSendEvents());
                        }

                        UPnPQueryListener* UPnPStateVariable::getUPnPQueryListener() {
                            return queryListener;
                        }

                        void UPnPStateVariable::setUPnPQueryListener(UPnPQueryListener* listener) {
                            queryListener = listener;
                        }

                        bool UPnPStateVariable::executeUPnPQueryListener(UPnPQueryRequest* req) {
                            UPnPQueryListener* listener = getUPnPQueryListener();
                            if (listener == NULL) {
                                return false;
                            } else {
                                UPnPQueryResponse res;
                                UPnPStateVariable var;
                                var.setUPnPStateVariable(this);
                                var.setValue("");
                                var.setStatus(UPNP_E_SUCCESS);
                                if (listener->variableRequest(&var) == true) {
                                    res.setUPnPStateVariable(&var);
                                } else {
                                    UPnPStatus* status = var.getStatus();
                                    res.setFaultResponse(status->getCode(), status->getDescription());
                                }
                                req->send(&res);
                                return true;
                            }
                        }

                        UPnPQueryResponse* UPnPStateVariable::getUPnPQueryResponse() {
                            return queryResponse;
                        }

                        void UPnPStateVariable::setUPnPQueryResponse(UPnPQueryResponse *res) {
                            if (queryResponse != NULL)
                                delete queryResponse;
                            queryResponse = res;
                        }

                        UPnPStatus* UPnPStateVariable::getUPnPQueryStatus() {
                            return getUPnPQueryResponse()->getStatus();
                        }

                        bool UPnPStateVariable::sendUPnPQueryAction() {
                            UPnPQueryRequest req;
                            req.setRequest(this);
                            UPnPQueryResponse* res = new UPnPQueryResponse();
                            req.send(res);
                            setUPnPQueryResponse(res);
                            setStatus(res->getStatus()->getCode());
                            if (res->isSuccessful()) {
                                setValue(res->getValueReceived());
                                return true;
                            } else {
                                return false;
                            }
                        }

                        void UPnPStateVariable::setStatus(int code, const char *desc) {
                            status.setCode(code);
                            status.setDescription(desc);
                        }

                        void UPnPStateVariable::setStatus(int code) {
                            setStatus(code, UPnPStatus::codeToString(code));
                        }

                        UPnPStatus* UPnPStateVariable::getStatus() {
                            return &status;
                        }

                        void UPnPStateVariable::setNode(NodePtr node) {
                            this->node = node;
                        }

                        void UPnPStateVariable::initializeUPnPAllowedValueList() {
                            allowedValueList.clear();
                            IXML_Node* allowedValueListNode = ((IXML_Node*)getNode())->firstChild;
                            while (allowedValueListNode != NULL) {
                                if (UPnPAllowedValueList::isUPnPAllowedValueListNode(allowedValueListNode)) {
                                    IXML_Node* allowedValueNode = allowedValueListNode->firstChild;
                                    while (allowedValueNode != NULL) {
                                        if (UPnPAllowedValue::isUPnPAllowedValueNode(allowedValueNode)) {
                                            UPnPAllowedValue* allowedValue = new UPnPAllowedValue(allowedValueNode);
                                            allowedValueList.add(allowedValue);
                                        }
                                        allowedValueNode = allowedValueNode->nextSibling;
                                    }
                                    return;
                                }
                                allowedValueListNode = allowedValueListNode->nextSibling;
                            }
                        }

                        void UPnPStateVariable::initializeUPnPAllowedValueRange() {
                            IXML_Node* allowedValueRangeNode = ((IXML_Node*)getNode())->firstChild;
                            while (allowedValueRangeNode != NULL) {
                                if (UPnPAllowedValueRange::isUPnPAllowedValueRangeNode(allowedValueRangeNode)) {
                                    if(allowedValueRange != NULL)
                                        delete allowedValueRange;
                                    allowedValueRange = new UPnPAllowedValueRange(allowedValueRangeNode);
                                    return;
                                }
                                allowedValueRangeNode = allowedValueRangeNode->nextSibling;
                            }
                        }

                    }
                }
            }
        }
    }
}
