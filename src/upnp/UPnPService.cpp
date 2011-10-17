#include "honesconfig.h"

#include <stdlib.h>
#include <upnp/ixml.h>
#include <upnp/upnp.h>
#include "upnp/UPnPAction.h"
#include "upnp/UPnPDevice.h"
#include "upnp/UPnPQueryListener.h"
#include "upnp/UPnPActionListener.h"
#include "upnp/UPnPService.h"
#include "util/Utils.h"
using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        const string UPnPService::ELEMENT_NAME = "service";
                        const string UPnPService::SERVICE_TYPE = "serviceType";
                        const string UPnPService::SERVICE_ID = "serviceId";
                        const string UPnPService::SCPD_URL = "SCPDURL";
                        const string UPnPService::CONTROL_URL = "controlURL";
                        const string UPnPService::EVENT_SUB_URL = "eventSubURL";
                        const string UPnPService::SCPD = "scpd";

                        UPnPService::UPnPService(NodePtr node) {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpservice");
                            LoggerUtil_info(logger, "Entrando no construtor UPnPService(node)!");

                            subscriptionID = NULL;
                            setUPnPDevice(NULL);
                            setSCPDNode(NULL);
                            setNode(node);
                            initializeUPnPActionList();
                            initializeUPnPServiceStateTable();

                            LoggerUtil_info(logger, "Saindo do construtor UPnPService(node)!");
                        }

                        UPnPService::UPnPService(NodePtr node, UPnPDevice* device){
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpservice");
                            LoggerUtil_info(logger, "Entrando no construtor UPnPService(node, device)!");

                            subscriptionID = NULL;
                            setUPnPDevice(device);
                            setSCPDNode(NULL);
                            setNode(node);
                            initializeUPnPActionList();
                            initializeUPnPServiceStateTable();

                            LoggerUtil_info(logger, "Saindo do construtor UPnPService(node, device)!");
                        }

                        UPnPService::~UPnPService() {
                            actions.clear();
                            stateVariableTable.clear();
                            if (SCPD_Node != NULL){
                                ixmlNode_free((IXML_Node*)SCPD_Node);
                                SCPD_Node = NULL;
                            }
                            if (node != NULL){
                                ixmlNode_free((IXML_Node*)node);
                                node = NULL;
                            }
                            if (subscriptionID != NULL){
                                delete[] subscriptionID;
                            }
                        }

                        NodePtr UPnPService::getNode() {
                            return node;
                        }

                        bool UPnPService::isUPnPServiceNode(NodePtr node) {
                            return (ELEMENT_NAME.compare( ((IXML_Node*)node)->nodeName) == 0);
                        }

                        void UPnPService::setUPnPDevice(UPnPDevice* device) {
                            this->device = device;
                        }

                        UPnPDevice* UPnPService::getUPnPDevice() {
                            return device;
                        }

                        UPnPDevice* UPnPService::getUPnPRootDevice() {
                            if (getUPnPDevice() != NULL)
                                return getUPnPDevice()->getUPnPRootDevice();
                            else
                                return NULL;
                        }

                        void UPnPService::setServiceType(const char* type) {
                            Utils::setNode(getNode(), SERVICE_TYPE, type);
                        }

                        const char* UPnPService::getServiceType() {
                            return Utils::getNodeValue(getNode(), SERVICE_TYPE);
                        }

                        void UPnPService::setServiceID(const char* id) {
                            Utils::setNode(getNode(), SERVICE_ID, id);
                        }

                        const char* UPnPService::getServiceID() {
                            return Utils::getNodeValue(getNode(), SERVICE_ID);
                        }

                        bool UPnPService::hasServiceID() {
                            if (getServiceID() != NULL) {
                                string sid = getServiceID();
                                if (sid.length() > 0)
                                    return true;
                            }
                            return false;
                        }

                        void UPnPService::setSCPDURL(const char* url) {
                            Utils::setNode(getNode(), SCPD_URL, url);
                        }

                        const char* UPnPService::getSCPDURL() {
                            return Utils::getNodeValue(getNode(), SCPD_URL);
                        }

                        bool UPnPService::isSCPDURL(const char* url) {
                            return Utils::equalsURL(getSCPDURL(), url);
                        }

                        const char* UPnPService::getSCPD() {
                            return ixmlPrintDocument((IXML_Document*) getSCPDNode());
                        }

                        bool UPnPService::loadSCPD(const char* desc) {
                            bool ret = false;
                            IXML_Document* xml = ixmlParseBuffer(desc);
                            if (xml != NULL) {
                                IXML_Node* scpdNode = xml->n.firstChild;
                                if ((scpdNode != NULL) && (SCPD.compare(scpdNode->nodeName) == 0)) {
                                    setSCPDNode(ixmlNode_cloneNode(scpdNode, true));
                                    initializeUPnPActionList();
                                    initializeUPnPServiceStateTable();
                                    ret = true;
                                }
                                ixmlDocument_free(xml);
                            }
                            return ret;
                        }

                        NodePtr UPnPService::getSCPDNode() {
                            LoggerUtil_info(logger, "Entrando no metodo getSCPDNode()!");

                            if (SCPD_Node != NULL) {
                                return SCPD_Node;
                            } else {
                                IXML_Document* xml;
                                if (Utils::isAbsoluteURL(getSCPDURL()) && (UpnpDownloadXmlDoc(getSCPDURL(), &xml) == UPNP_E_SUCCESS)) {
                                    setSCPDNode(ixmlNode_cloneNode(xml->n.firstChild, true));
                                    ixmlDocument_free(xml);
                                    if (SCPD_Node != NULL)
                                        return SCPD_Node;
                                }

                                UPnPDevice* root = getUPnPRootDevice();

                                if (root != NULL) {
                                    string urlbase;
                                    if (strlen(root->getURLBase()) > 0) {
                                        urlbase = "http://";
                                        urlbase.append(Utils::getHostURL(root->getURLBase()));
                                        urlbase.append(":");
                                        urlbase.append(Utils::integerToString(Utils::getPortURL(root->getURLBase())));
                                    } else if (strlen(root->getLocation()) > 0) {
                                        urlbase = "http://";
                                        urlbase.append(Utils::getHostURL(root->getLocation()));
                                        urlbase.append(":");
                                        urlbase.append(Utils::integerToString(Utils::getPortURL(root->getLocation())));
                                    }

                                    if (urlbase.length() > 0) {
                                        string newurl = urlbase;
                                        newurl.append(Utils::getRelativeURL(getSCPDURL()));
                                        if (UpnpDownloadXmlDoc(newurl.c_str(), &xml) == UPNP_E_SUCCESS) {
                                            setSCPDNode(ixmlNode_cloneNode(xml->n.firstChild, true));
                                            ixmlDocument_free(xml);
                                            return SCPD_Node;
                                        }
                                    }
                                }

                                return NULL;
                            }
                        }

                        void UPnPService::setControlURL(const char* url) {
                            Utils::setNode(getNode(), CONTROL_URL, url);
                        }

                        const char* UPnPService::getControlURL() {
                            return Utils::getNodeValue(getNode(), CONTROL_URL);
                        }

                        bool UPnPService::isControlURL(const char* url) {
                            return Utils::equalsURL(getControlURL(), url);
                        }

                        void UPnPService::setEventSubURL(const char* url) {
                            Utils::setNode(getNode(), EVENT_SUB_URL, url);
                        }

                        const char* UPnPService::getEventSubURL() {
                            return Utils::getNodeValue(getNode(), EVENT_SUB_URL);
                        }

                        bool UPnPService::isEventSubURL(const char* url) {
                            return Utils::equalsURL(getEventSubURL(), url);
                        }

                        UPnPActionList* UPnPService::getUPnPActions() {
                            return &actions;
                        }

                        UPnPAction* UPnPService::getUPnPAction(const char* name) {
                            return actions.getUPnPAction(name);
                        }

                        UPnPServiceStateTable* UPnPService::getUPnPServiceStateTable() {
                            return &stateVariableTable;
                        }

                        UPnPStateVariable* UPnPService::getUPnPStateVariable(const char* name) {
                            return stateVariableTable.getUPnPStateVariable(name);
                        }

                        bool UPnPService::hasUPnPStateVariable(const char* name) {
                            return (stateVariableTable.getUPnPStateVariable(name) != NULL);
                        }

                        bool UPnPService::isUPnPService(const char* name) {
                            return (Utils::stringEndsWith(name, getServiceType()) || Utils::stringEndsWith(name, getServiceID()));
                        }

                        void UPnPService::sendEventMessage(UPnPStateVariable* var) {
                            if (getUPnPDevice() != NULL) {
                                getUPnPDevice()->sendEventMessage(var);
                            }
                        }

                        void UPnPService::notifyAllUPnPStateVariables() {
                            UPnPServiceStateTable* list = getUPnPServiceStateTable();
                            int size = list->size();
                            for (int i = 0; i < size; i++) {
                                UPnPStateVariable* var = list->getUPnPStateVariable(i);
                                if (var->isSendEvents())
                                    sendEventMessage(var);
                            }
                        }

                        void UPnPService::setUPnPQueryListener(UPnPQueryListener* listener) {
                            UPnPServiceStateTable* list = getUPnPServiceStateTable();
                            int size = list->size();
                            for (int i = 0; i < size; i++) {
                                UPnPStateVariable* var = list->getUPnPStateVariable(i);
                                var->setUPnPQueryListener(listener);
                            }
                        }

                        void UPnPService::setUPnPActionListener(UPnPActionListener* listener) {
                            UPnPActionList* list = getUPnPActions();
                            int size = list->size();
                            for (int i = 0; i < size; i++) {
                                UPnPAction* action = list->getUPnPAction(i);
                                action->setUPnPActionListener(listener);
                            }
                        }

                        bool UPnPService::isSubscribed(){
                            return (getSubscriptionID() != NULL);
                        }

                        void UPnPService::setSubscriptionID(char* sid){
                            if(subscriptionID != NULL)
                                free(subscriptionID);
                            subscriptionID = sid;
                        }

                        char* UPnPService::getSubscriptionID(){
                            return subscriptionID;
                        }

                        void UPnPService::setNode(NodePtr node) {
                            this->node = node;
                        }

                        void UPnPService::setSCPDNode(NodePtr node) {
                            SCPD_Node = node;
                        }

                        void UPnPService::initializeUPnPActionList() {
                            LoggerUtil_info(logger, "Entrando no metodo initializeUPnPActionList()!");

                            actions.clear();
                            IXML_Node* scpdnode = (IXML_Node*) getSCPDNode();
                            if (scpdnode != NULL) {
                                IXML_Node* actionListNode = scpdnode->firstChild;
                                while (actionListNode != NULL) {
                                    if (UPnPActionList::isUPnPActionListNode(actionListNode)) {
                                        IXML_Node* actionNode = actionListNode->firstChild;
                                        while (actionNode != NULL) {
                                            if (UPnPAction::isUPnPActionNode(actionNode)) {
                                                UPnPAction* action = new UPnPAction(this, actionNode);
                                                actions.add(action);
                                            }
                                            actionNode = actionNode->nextSibling;
                                        }
                                        return;
                                    }
                                    actionListNode = actionListNode->nextSibling;
                                }
                            }
                        }

                        void UPnPService::initializeUPnPServiceStateTable() {
                            stateVariableTable.clear();
                            IXML_Node* scpdnode = (IXML_Node*) getSCPDNode();
                            if (scpdnode != NULL) {
                                IXML_Node* stateVariableTableNode = scpdnode->firstChild;
                                while (stateVariableTableNode != NULL) {
                                    if (UPnPServiceStateTable::isUPnPServiceStateTableNode(stateVariableTableNode)) {
                                        IXML_Node* stateVariableNode = stateVariableTableNode->firstChild;
                                        while (stateVariableNode != NULL) {
                                            if (UPnPStateVariable::isUPnPStateVariableNode(stateVariableNode)) {
                                                UPnPStateVariable* var = new UPnPStateVariable(stateVariableNode);
                                                var->setUPnPService(this);
                                                stateVariableTable.add(var);
                                            }
                                            stateVariableNode = stateVariableNode->nextSibling;
                                        }
                                        return;
                                    }
                                    stateVariableTableNode = stateVariableTableNode->nextSibling;
                                }
                            }
                        }

                    }
                }
            }
        }
    }
}
