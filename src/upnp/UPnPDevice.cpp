#include "honesconfig.h"

#include <string.h>
#include <stdlib.h>
#include <upnp/ixml.h>
#include <upnp/upnp.h>
#include <upnp/ActionRequest.h>
#include <upnp/SubscriptionRequest.h>
#include <upnp/StateVarRequest.h>
#include <upnp/FileInfo.h>
#include <upnp/UpnpString.h>
#include "upnp/UPnPService.h"
#include "upnp/UPnPStateVariable.h"
#include "upnp/UPnPAction.h"
#include "upnp/UPnPIcon.h"
#include "upnp/UPnPQueryListener.h"
#include "upnp/UPnPQueryRequest.h"
#include "upnp/UPnPActionListener.h"
#include "upnp/UPnPActionRequest.h"
#include "upnp/UPnPArgumentList.h"
#include "upnp/UPnPHttpServer.h"
#include "upnp/UPnPDeviceController.h"
#include "upnp/UPnPDevice.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        const string UPnPDevice::ELEMENT_NAME = "device";
                        const string UPnPDevice::UPNP_ROOTDEVICE = "upnp:rootdevice";
                        const string UPnPDevice::DEFAULT_DESCRIPTION_URI = "/description.xml";
                        const string UPnPDevice::URLBASE = "URLBase";
                        const string UPnPDevice::DEVICE_TYPE = "deviceType";
                        const string UPnPDevice::FRIENDLY_NAME = "friendlyName";
                        const string UPnPDevice::MANUFACTURE = "manufacture";
                        const string UPnPDevice::MANUFACTURE_URL = "manufactureURL";
                        const string UPnPDevice::MODEL_DESCRIPTION = "modelDescription";
                        const string UPnPDevice::MODEL_NAME = "modelName";
                        const string UPnPDevice::MODEL_NUMBER = "modelNumber";
                        const string UPnPDevice::MODEL_URL = "modelURL";
                        const string UPnPDevice::SERIAL_NUMBER = "serialNumber";
                        const string UPnPDevice::UDN = "UDN";
                        const string UPnPDevice::UPC = "UPC";
                        const string UPnPDevice::PRESENTATION_URL = "presentationURL";
                        UPnPDeviceList* UPnPDevice::localRootDeviceInstances = NULL;

                        UPnPDevice::UPnPDevice() {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpdevice");
                            LoggerUtil_info(logger, "Entrando no construtor UPnPDevice()!");

                            device_handle = new int;
                            *device_handle = -1;
                            adsThread = NULL;
                            setNode(NULL);
                            setRootNode(NULL);
                            setUPnPRootDevice(NULL);
                            setUPnPParentDevice(NULL);
                            initializeLists();
                        }

                        UPnPDevice::UPnPDevice(NodePtr node) {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpdevice");
                            LoggerUtil_info(logger, "Entrando no construtor UPnPDevice(node)!");

                            device_handle = new int;
                            *device_handle = -1;
                            adsThread = NULL;
                            setNode(node);
                            setRootNode(NULL);
                            setUPnPRootDevice(NULL);
                            setUPnPParentDevice(NULL);
                            initializeLists();
                        }

                        UPnPDevice::UPnPDevice(NodePtr rootnode, const char* location){
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpdevice");
                            LoggerUtil_info(logger, "Entrando no construtor UPnPDevice(node, location)!");

                            device_handle = new int;
                            *device_handle = -1;
                            adsThread = NULL;
                            setLocation(location);
                            setNode(NULL);
                            setRootNode(NULL);
                            setUPnPRootDevice(NULL);
                            setUPnPParentDevice(NULL);
                            loadDescription(rootnode);
                        }

                        UPnPDevice::~UPnPDevice() {
                            LoggerUtil_info(logger, "Entrando no destrutor ~UPnPDevice()!");
                            if (isUPnPRootDevice()) {
                                stop();
                                if(rootnode != NULL){
                                    ixmlNode_free((IXML_Node*)rootnode);
                                    rootnode = NULL;
                                    node = NULL;
                                } else if(node != NULL){
                                    ixmlNode_free((IXML_Node*)node);
                                    node = NULL;
                                    LoggerUtil_debug(logger, "node = NULL");
                                }
                                if (getUPnPLocalRootDeviceInstances()->size() == 0) {
                                    delete localRootDeviceInstances;
                                    localRootDeviceInstances = NULL;
                                    UPnPHttpServer::getInstance()->stop();
                                    UPnPHttpServer::deleteInstance();
                                }
                            }

                            if (device_handle != NULL){
                                delete device_handle;
                                device_handle = NULL;
                                LoggerUtil_debug(logger, "device_handle = NULL");
                            }

                            embeddedDevices.clear();
                            LoggerUtil_debug(logger, "embeddedDevices.clear()");
                            services.clear();
                            LoggerUtil_debug(logger, "services.clear()");
                            icons.clear();
                            LoggerUtil_debug(logger, "icons.clear()");
                        }

                        NodePtr UPnPDevice::getNode() {
                            return node;
                        }

                        NodePtr UPnPDevice::getRootNode() {
                            return rootnode;
                        }

                        bool UPnPDevice::isUPnPDeviceNode(NodePtr node) {
                            return (ELEMENT_NAME.compare( ((IXML_Node*)node)->nodeName) == 0);
                        }

                        bool UPnPDevice::isUPnPRootDevice() {
                            return (getRootNode() != NULL);
                        }

                        UPnPDevice* UPnPDevice::getUPnPRootDevice() {
                            if (root != NULL) {
                                return root;
                            } else {
                                UPnPDevice* parent = this;
                                while (parent != NULL) {
                                    if (parent->isUPnPRootDevice()) {
                                        setUPnPRootDevice(parent);
                                        return parent;
                                    }
                                    parent = parent->getUPnPParentDevice();
                                }
                                return NULL;
                            }
                        }

                        UPnPDevice* UPnPDevice::getUPnPParentDevice() {
                            return parent;
                        }

                        void UPnPDevice::setUPnPParentDevice(UPnPDevice* parent) {
                            this->parent = parent;
                        }

                        void UPnPDevice::setDescriptionURI(const char* uri) {
                            descriptionURI = uri;
                        }

                        const char* UPnPDevice::getDescriptionURI() {
                            return descriptionURI.c_str();
                        }

                        bool UPnPDevice::isDescriptionURI(const char* uri) {
                            return (descriptionURI.compare(uri) == 0);
                        }

                        bool UPnPDevice::loadDescription(const char* desc) {
                            bool ret = false;
                            IXML_Document* xml = ixmlParseBuffer(desc);
                            if (xml != NULL) {
                                IXML_Node* rootNode = xml->n.firstChild;
                                if (rootNode != NULL) {
                                    ret = loadDescription(ixmlNode_cloneNode(rootNode, true));
                                }
                                ixmlDocument_free(xml);
                            }
                            return ret;
                        }

                        bool UPnPDevice::loadDescription(NodePtr rootNode) {
                            if(this->rootnode != NULL)
                                ixmlNode_free((IXML_Node*)this->rootnode);
                            
                            bool ret = false;
                            IXML_Node* deviceNode = NULL;
                            if (rootNode != NULL) {
                                deviceNode = ((IXML_Node*) rootNode)->firstChild;
                                while ((deviceNode != NULL) && (!ret)) {
                                    if (isUPnPDeviceNode(deviceNode)) {
                                        setNode(deviceNode);
                                        setRootNode(rootNode);
                                        setDescriptionURI(DEFAULT_DESCRIPTION_URI.c_str());
                                        if (hasUDN() == false) {
                                            string uuid = "uuid:";
                                            uuid.append(getUUID());
                                            setUDN(uuid.c_str());
                                        }
                                        initializeLists();
                                        ret = true;
                                    }
                                    deviceNode = deviceNode->nextSibling;
                                }
                                if (ret == false) {
                                    ixmlNode_free((IXML_Node*) rootNode);
                                }
                            }
                            return ret;
                        }

                        void UPnPDevice::setLocation(const char* location){
                            if(location != NULL)
                                this->location = location;
                        }

                        const char* UPnPDevice::getLocation() {
                            if ((location.length() <= 0) && (getUPnPRootDevice() != NULL) && (getUPnPRootDevice() != this)) {
                                string urlbase;
                                if (strlen(getUPnPRootDevice()->getURLBase()) > 0) {
                                    urlbase = "http://";
                                    urlbase.append(Utils::getHostURL(getUPnPRootDevice()->getURLBase()));
                                    urlbase.append(":");
                                    urlbase.append(Utils::integerToString(Utils::getPortURL(getUPnPRootDevice()->getURLBase())));
                                } else if (strlen(getUPnPRootDevice()->getLocation()) > 0) {
                                    urlbase = "http://";
                                    urlbase.append(Utils::getHostURL(getUPnPRootDevice()->getLocation()));
                                    urlbase.append(":");
                                    urlbase.append(Utils::integerToString(Utils::getPortURL(getUPnPRootDevice()->getLocation())));
                                }
                                setLocation(urlbase.c_str());
                            }
                            return location.c_str();
                        }

                        void UPnPDevice::setDeviceType(const char* type) {
                            Utils::setNode(getNode(), DEVICE_TYPE, type);
                        }

                        const char* UPnPDevice::getDeviceType() {
                            return Utils::getNodeValue(getNode(), DEVICE_TYPE);
                        }

                        bool UPnPDevice::isDeviceType(const char* type) {
                            if (type != NULL) {
                                string typeStr = type;
                                return (typeStr.compare(getDeviceType()) == 0);
                            } else {
                                return false;
                            }
                        }

                        void UPnPDevice::setFriendlyName(const char* name) {
                            Utils::setNode(getNode(), FRIENDLY_NAME, name);
                        }

                        const char* UPnPDevice::getFriendlyName() {
                            return Utils::getNodeValue(getNode(), FRIENDLY_NAME);
                        }

                        void UPnPDevice::setManufacture(const char* value) {
                            Utils::setNode(getNode(), MANUFACTURE, value);
                        }

                        const char* UPnPDevice::getManufacture() {
                            return Utils::getNodeValue(getNode(), MANUFACTURE);
                        }

                        void UPnPDevice::setManufactureURL(const char* url) {
                            Utils::setNode(getNode(), MANUFACTURE_URL, url);
                        }

                        const char* UPnPDevice::getManufactureURL() {
                            return Utils::getNodeValue(getNode(), MANUFACTURE_URL);
                        }

                        void UPnPDevice::setModelDescription(const char* desc) {
                            Utils::setNode(getNode(), MODEL_DESCRIPTION, desc);
                        }

                        const char* UPnPDevice::getModelDescription() {
                            return Utils::getNodeValue(getNode(), MODEL_DESCRIPTION);
                        }

                        void UPnPDevice::setModelName(const char* name) {
                            Utils::setNode(getNode(), MODEL_NAME, name);
                        }

                        const char* UPnPDevice::getModelName() {
                            return Utils::getNodeValue(getNode(), MODEL_NAME);
                        }

                        void UPnPDevice::setModelNumber(const char* number) {
                            Utils::setNode(getNode(), MODEL_NUMBER, number);
                        }

                        const char* UPnPDevice::getModelNumber() {
                            return Utils::getNodeValue(getNode(), MODEL_NUMBER);
                        }

                        void UPnPDevice::setModelURL(const char* url) {
                            Utils::setNode(getNode(), MODEL_URL, url);
                        }

                        const char* UPnPDevice::getModelURL() {
                            return Utils::getNodeValue(getNode(), MODEL_URL);
                        }

                        void UPnPDevice::setSerialNumber(const char* number) {
                            Utils::setNode(getNode(), SERIAL_NUMBER, number);
                        }

                        const char* UPnPDevice::getSerialNumber() {
                            return Utils::getNodeValue(getNode(), SERIAL_NUMBER);
                        }

                        void UPnPDevice::setUDN(const char* udn) {
                            Utils::setNode(getNode(), UDN, udn);
                        }

                        const char* UPnPDevice::getUDN() {
                            return Utils::getNodeValue(getNode(), UDN);
                        }

                        bool UPnPDevice::hasUDN() {
                            if (getUDN() != NULL)
                                return (string(getUDN()).length() > 0);
                            else
                                return false;
                        }

                        void UPnPDevice::setUPC(const char* upc) {
                            Utils::setNode(getNode(), UPC, upc);
                        }

                        const char* UPnPDevice::getUPC() {
                            return Utils::getNodeValue(getNode(), UPC);
                        }

                        void UPnPDevice::setPresentationURL(const char* url) {
                            Utils::setNode(getNode(), PRESENTATION_URL, url);
                        }

                        const char* UPnPDevice::getPresentationURL() {
                            return Utils::getNodeValue(getNode(), PRESENTATION_URL);
                        }

                        const char* UPnPDevice::getURLBase() {
                            if (isUPnPRootDevice()) {
                                return Utils::getNodeValue(getRootNode(), URLBASE);
                            } else {
                                return "";
                            }
                        }

                        UPnPDeviceList* UPnPDevice::getEmbeddedDevices() {
                            return &embeddedDevices;
                        }

                        bool UPnPDevice::isUPnPDevice(const char* name) {
                            if (name != NULL) {
                                string nameStr = name;
                                return ( Utils::stringEndsWith(name, getUDN()) ||
                                        (nameStr.compare(getFriendlyName()) == 0) ||
                                        Utils::stringEndsWith(name, getDeviceType()));
                            } else {
                                return false;
                            }
                        }

                        UPnPDevice* UPnPDevice::getUPnPDevice(const char* name) {
                            UPnPDeviceList* list = getEmbeddedDevices();
                            return list->getUPnPDevice(name);
                        }

                        UPnPDevice* UPnPDevice::getUPnPDeviceByDescriptionURI(const char* uri) {
                            UPnPDeviceList* list = getEmbeddedDevices();
                            return list->getUPnPDeviceByDescriptionURI(uri);
                        }

                        UPnPServiceList* UPnPDevice::getUPnPServices() {
                            return &services;
                        }

                        UPnPService* UPnPDevice::getUPnPService(const char* name) {
                            UPnPService* serv = NULL;
                            UPnPServiceList* servList = getUPnPServices();
                            serv = servList->getUPnPService(name);
                            if (serv == NULL) {
                                UPnPDeviceList* devList = getEmbeddedDevices();
                                serv = devList->getUPnPService(name);
                            }
                            return serv;
                        }

                        UPnPService* UPnPDevice::getUPnPServiceBySCPDURL(const char* url) {
                            UPnPService* serv = NULL;
                            UPnPServiceList* servList = getUPnPServices();
                            serv = servList->getUPnPServiceBySCPDURL(url);
                            if (serv == NULL) {
                                UPnPDeviceList* devList = getEmbeddedDevices();
                                serv = devList->getUPnPServiceBySCPDURL(url);
                            }
                            return serv;
                        }

                        UPnPService* UPnPDevice::getUPnPServiceByControlURL(const char* url) {
                            UPnPService* serv = NULL;
                            UPnPServiceList* servList = getUPnPServices();
                            serv = servList->getUPnPServiceByControlURL(url);
                            if (serv == NULL) {
                                UPnPDeviceList* devList = getEmbeddedDevices();
                                serv = devList->getUPnPServiceByControlURL(url);
                            }
                            return serv;
                        }

                        UPnPService* UPnPDevice::getUPnPServiceByEventSubURL(const char* url) {
                            UPnPService* serv = NULL;
                            UPnPServiceList* servList = getUPnPServices();
                            serv = servList->getUPnPServiceByEventSubURL(url);
                            if (serv == NULL) {
                                UPnPDeviceList* devList = getEmbeddedDevices();
                                serv = devList->getUPnPServiceByEventSubURL(url);
                            }
                            return serv;
                        }

                        UPnPStateVariable* UPnPDevice::getUPnPStateVariable(const char* servType, const char* name) {
                            UPnPService* serv = getUPnPService(servType);
                            if (serv != NULL)
                                return serv->getUPnPStateVariable(name);
                            else
                                return NULL;
                        }

                        UPnPStateVariable* UPnPDevice::getUPnPStateVariable(const char* name) {
                            UPnPStateVariable* var = NULL;
                            UPnPServiceList* servList = getUPnPServices();
                            var = servList->getUPnPStateVariable(name);
                            if (var == NULL) {
                                UPnPDeviceList* devList = getEmbeddedDevices();
                                var = devList->getUPnPStateVariable(name);
                            }
                            return var;
                        }

                        void UPnPDevice::sendEventMessage(UPnPStateVariable* var) {
                            if ((var != NULL) && (*device_handle >= 0)) {
                                IXML_Document* doc = NULL;

                                if (Utils::addArgumentToPropertySet((DocumentPtr*)&doc, var->getName(), var->getValue()) == true) {
                                    if ((getUDN() != NULL) && (var->getUPnPService() != NULL))
                                        UpnpNotifyExt(*device_handle, getUDN(), var->getUPnPService()->getServiceID(), doc);
                                }
                                if (doc != NULL)
                                    ixmlDocument_free(doc);
                            }
                        }

                        UPnPAction* UPnPDevice::getUPnPAction(const char* name) {
                            UPnPAction* action = NULL;
                            UPnPServiceList* servList = getUPnPServices();
                            action = servList->getUPnPAction(name);
                            if (action == NULL) {
                                UPnPDeviceList* devList = getEmbeddedDevices();
                                action = devList->getUPnPAction(name);
                            }
                            return action;
                        }

                        UPnPIconList* UPnPDevice::getUPnPIcons() {
                            return &icons;
                        }

                        UPnPIcon* UPnPDevice::getUPnPIcon(int n) {
                            return icons.getUPnPIcon(n);
                        }

                        static int deviceCallbackEventHandler(Upnp_EventType EventType, void* Event, void* Cookie) {
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpdevice");
                            LoggerUtil_info(logger, "Entrando no metodo deviceCallbackEventHandler");

                            switch (EventType) {
                                case UPNP_CONTROL_ACTION_REQUEST:
                                {
                                    UpnpActionRequest* actionReq = (UpnpActionRequest*) Event;
                                    UPnPDevice* dev = UPnPDevice::getUPnPLocalRootDeviceInstances()->getUPnPDevice(UpnpString_get_String(UpnpActionRequest_get_DevUDN(actionReq)));
                                    if (dev != NULL) {
                                        UPnPService* serv = dev->getUPnPService(UpnpString_get_String(UpnpActionRequest_get_ServiceID(actionReq)));
                                        if (serv != NULL) {
                                            UPnPAction* action = serv->getUPnPAction(UpnpString_get_String(UpnpActionRequest_get_ActionName(actionReq)));
                                            UPnPActionRequest req(actionReq);
                                            if (action != NULL) {
                                                if (action->executeUPnPActionListener(&req) == false){
                                                    if ((action->getStatus()->getCode() == UPNP_E_INVALID_ARGUMENT) || (action->getStatus()->getCode() == UPNP_SOAP_E_INVALID_ARGS) || (action->getStatus()->getCode() == UPNP_E_INVALID_PARAM)) {
                                                        UpnpActionRequest_set_ActionResult(actionReq, NULL);
                                                        UpnpString* errStr = UpnpString_new();
                                                        UpnpString_set_String(errStr, "Invalid Args");
                                                        UpnpActionRequest_set_ErrStr(actionReq, errStr);
                                                        UpnpActionRequest_set_ErrCode(actionReq, UPNP_SOAP_E_INVALID_ARGS);
                                                    } else if (action->getStatus()->getCode() == UPNP_SOAP_E_INVALID_INSTANCE_ID) {
                                                        UpnpActionRequest_set_ActionResult(actionReq, NULL);
                                                        UpnpString* errStr = UpnpString_new();
                                                        UpnpString_set_String(errStr, "Invalid InstanceID");
                                                        UpnpActionRequest_set_ErrStr(actionReq, errStr);
                                                        UpnpActionRequest_set_ErrCode(actionReq, UPNP_SOAP_E_INVALID_INSTANCE_ID);
                                                    } else {
                                                        UpnpActionRequest_set_ActionResult(actionReq, NULL);
                                                        UpnpString* errStr = UpnpString_new();
                                                        UpnpString_set_String(errStr, "Action Failed");
                                                        UpnpActionRequest_set_ErrStr(actionReq, errStr);
                                                        UpnpActionRequest_set_ErrCode(actionReq, UPNP_SOAP_E_ACTION_FAILED);
                                                    }
                                                }

                                                return 0;
                                            }
                                        }
                                    }
                                    UpnpActionRequest_set_ActionResult(actionReq, NULL);
                                    UpnpString* errStr = UpnpString_new();
                                    UpnpString_set_String(errStr, "Invalid Action");
                                    UpnpActionRequest_set_ErrStr(actionReq, errStr);
                                    UpnpActionRequest_set_ErrCode(actionReq, UPNP_SOAP_E_INVALID_ACTION);
                                
                                    break;
                                }

                                case UPNP_EVENT_SUBSCRIPTION_REQUEST:
                                {
                                    UpnpSubscriptionRequest* subscription = (UpnpSubscriptionRequest*) Event;
                                    UPnPDevice* dev = UPnPDevice::getUPnPLocalRootDeviceInstances()->getUPnPDevice(UpnpString_get_String(UpnpSubscriptionRequest_get_UDN(subscription)));
                                    if (dev != NULL) {
                                        UPnPService* serv = dev->getUPnPService(UpnpString_get_String(UpnpSubscriptionRequest_get_ServiceId(subscription)));
                                        if (serv != NULL) {
                                            IXML_Document* xml = NULL;
                                            UPnPServiceStateTable* table = serv->getUPnPServiceStateTable();
                                            int size = table->size();
                                            for (int i = 0; i < size; i++)
                                                Utils::addArgumentToPropertySet((DocumentPtr*)&xml, table->getUPnPStateVariable(i)->getName(), table->getUPnPStateVariable(i)->getValue());
                                            UpnpAcceptSubscriptionExt(*((UpnpDevice_Handle*) Cookie), UpnpString_get_String(UpnpSubscriptionRequest_get_UDN(subscription)), UpnpString_get_String(UpnpSubscriptionRequest_get_ServiceId(subscription)), xml, (char*) UpnpString_get_String(UpnpSubscriptionRequest_get_SID(subscription)));
                                            if (xml != NULL)
                                                ixmlDocument_free(xml);
                                        }
                                    }
                                
                                    break;
                                }
                                
                                case UPNP_CONTROL_GET_VAR_REQUEST:
                                {
                                    UpnpStateVarRequest* varReq = (UpnpStateVarRequest*) Event;
                                    UPnPDevice* dev = UPnPDevice::getUPnPLocalRootDeviceInstances()->getUPnPDevice(UpnpString_get_String(UpnpStateVarRequest_get_DevUDN(varReq)));
                                    if (dev != NULL) {
                                        UPnPService* serv = dev->getUPnPService(UpnpString_get_String(UpnpStateVarRequest_get_ServiceID(varReq)));
                                        if (serv != NULL) {
                                            UPnPStateVariable* var = serv->getUPnPStateVariable(UpnpString_get_String(UpnpStateVarRequest_get_StateVarName(varReq)));
                                            UPnPQueryRequest req(varReq);
                                            if (var != NULL) {
                                                if (var->executeUPnPQueryListener(&req) == false) {
                                                    UpnpString* errStr = UpnpString_new();
                                                    UpnpString_set_String(errStr, "Invalid Action");
                                                    UpnpStateVarRequest_set_ErrStr(varReq, errStr);
                                                    UpnpStateVarRequest_set_ErrCode(varReq, UPNP_SOAP_E_INVALID_ACTION);
                                                }

                                                return 0;
                                            }
                                        }
                                    }
                                    UpnpString* errStr = UpnpString_new();
                                    UpnpString_set_String(errStr, "Invalid Variable");
                                    UpnpStateVarRequest_set_ErrStr(varReq, errStr);
                                    UpnpStateVarRequest_set_ErrCode(varReq, UPNP_SOAP_E_INVALID_VAR);
                                
                                    break;
                                }
                            }

                            return 0;
                        }

                        static int executeHttpGetInfo(const char* filename, UpnpFileInfo* info) {
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpdevice");
                            LoggerUtil_info(logger, "Entrando no metodo executeHttpGetInfo");

                            if ((filename != NULL) && (info != NULL)) {
                                UPnPDevice* dev = UPnPDevice::getUPnPLocalRootDeviceInstances()->getUPnPDeviceByDescriptionURI(filename);
                                if (dev != NULL) {
                                    off_t size = strlen(dev->getDescription());
                                    UpnpFileInfo_set_FileLength(info, size);
                                    //UpnpFileInfo_set_LastModified(info, NULL);
                                    UpnpFileInfo_set_IsDirectory(info, 0);
                                    UpnpFileInfo_set_IsReadable(info, 1);
                                    UpnpFileInfo_set_ContentType(info, ixmlCloneDOMString("text/xml"));
                                    return 0;
                                } else {
                                    UPnPService* serv = UPnPDevice::getUPnPLocalRootDeviceInstances()->getUPnPServiceBySCPDURL(filename);
                                    if (serv != NULL) {
                                        off_t size = strlen(serv->getSCPD());
                                        UpnpFileInfo_set_FileLength(info, size);
                                        //UpnpFileInfo_set_LastModified(info, NULL);
                                        UpnpFileInfo_set_IsDirectory(info, 0);
                                        UpnpFileInfo_set_IsReadable(info, 1);
                                        UpnpFileInfo_set_ContentType(info, ixmlCloneDOMString("text/xml"));
                                        return 0;
                                    }
                                }
                            }
                            return -1;
                        }

                        static UpnpWebFileHandle executeHttpOpen(const char* filename, enum UpnpOpenFileMode mode) {
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpdevice");
                            LoggerUtil_info(logger, "Entrando no metodo executeHttpOpen");

                            if ((mode == UPNP_READ) && (filename != NULL)) {
                                UPnPDevice* dev = UPnPDevice::getUPnPLocalRootDeviceInstances()->getUPnPDeviceByDescriptionURI(filename);
                                if (dev != NULL) {
                                    UPnPHttpFileHandle* file = new UPnPHttpFileHandle;
                                    file->pos = 0;
                                    file->type = UPnPHttpFileHandle::FILE_MEMORY;
                                    file->memory.contents = strdup(dev->getDescription());
                                    file->memory.len = strlen(dev->getDescription());
                                    return ((UpnpWebFileHandle) file);
                                } else {
                                    UPnPService* serv = UPnPDevice::getUPnPLocalRootDeviceInstances()->getUPnPServiceBySCPDURL(filename);
                                    if (serv != NULL) {
                                        UPnPHttpFileHandle* file = new UPnPHttpFileHandle;
                                        file->pos = 0;
                                        file->type = UPnPHttpFileHandle::FILE_MEMORY;
                                        file->memory.contents = strdup(serv->getSCPD());
                                        file->memory.len = strlen(serv->getSCPD());
                                        return ((UpnpWebFileHandle) file);
                                    }
                                }
                            }

                            return NULL;
                        }

                        static int executeHttpRead(UpnpWebFileHandle fh, char* buf, size_t buflen) {
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpdevice");
                            LoggerUtil_info(logger, "Entrando no metodo executeHttpRead");

                            UPnPHttpFileHandle* file = (UPnPHttpFileHandle*) fh;
                            size_t length = -1;

                            if ((file != NULL) && (file->type == UPnPHttpFileHandle::FILE_MEMORY)) {
                                length = (buflen < (file->memory.len - file->pos)) ? buflen : (file->memory.len - file->pos);
                                if (length > 0) {
                                    memcpy(buf, file->memory.contents + file->pos, length);
                                    file->pos += length;
                                }
                            }

                            return length;
                        }

                        static int executeHttpWrite(UpnpWebFileHandle fh, char* buf, size_t buflen) {
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpdevice");
                            LoggerUtil_info(logger, "Entrando no metodo executeHttpWrite");

                            return 0;
                        }

                        static int executeHttpSeek(UpnpWebFileHandle fh, off_t offset, int origin) {
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpdevice");
                            LoggerUtil_info(logger, "Entrando no metodo executeHttpSeek");

                            UPnPHttpFileHandle* file = (UPnPHttpFileHandle*) fh;
                            if ((file != NULL) && (file->type == UPnPHttpFileHandle::FILE_MEMORY)) {
                                off_t newpos = (off_t) -1;

                                switch (origin) {
                                    case SEEK_SET:
                                        newpos = offset;
                                        break;
                                    case SEEK_CUR:
                                        newpos = file->pos + offset;
                                        break;
                                    case SEEK_END:
                                        newpos = file->memory.len + offset;
                                        break;
                                }
                                if ((newpos >= 0) && (newpos <= file->memory.len)) {
                                    file->pos = newpos;
                                    return 0;
                                }
                            }

                            return -1;
                        }

                        static int executeHttpClose(UpnpWebFileHandle fh) {
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpdevice");
                            LoggerUtil_info(logger, "Entrando no metodo executeHttpClose");

                            UPnPHttpFileHandle* file = (UPnPHttpFileHandle*) fh;
                            if (file != NULL) {
                                if (file->type == UPnPHttpFileHandle::FILE_MEMORY) {
                                    if (file->memory.contents != NULL)
                                        free(file->memory.contents);
                                    delete file;
                                    return 0;
                                }
                            }

                            return -1;
                        }

                        void* sendAdvertisements(void* param) {
                            UpnpDevice_Handle* devHandle = (UpnpDevice_Handle*) param;
                            
                            while(devHandle != NULL) {
                                pthread_testcancel();
                                Utils::wait(30000);
                                UpnpSendAdvertisement(*devHandle, 1800);
                            }

                            return NULL;
                        }

                        bool UPnPDevice::start() {
                            LoggerUtil_info(logger, "Entrando no metodo start()");

                            stop();

                            if (isUPnPRootDevice()) {
                                
                                setLocation(Utils::initializeUPnP());

                                UpnpVirtualDir_set_GetInfoCallback(executeHttpGetInfo);
                                UpnpVirtualDir_set_OpenCallback(executeHttpOpen);
                                UpnpVirtualDir_set_ReadCallback(executeHttpRead);
                                UpnpVirtualDir_set_WriteCallback(executeHttpWrite);
                                UpnpVirtualDir_set_SeekCallback(executeHttpSeek);
                                UpnpVirtualDir_set_CloseCallback(executeHttpClose);

                                if(getUPnPLocalRootDeviceInstances()->size() > 0){
                                    setLocation(getUPnPLocalRootDeviceInstances()->getUPnPDevice(0)->getLocation());
                                }
                                
                                getUPnPLocalRootDeviceInstances()->add(this);
                                
                                string baseURL = getLocation();
                                baseURL.append("/");
                                setURLBase(baseURL.c_str());

                                string descURL = getLocation();
                                descURL.append(getDescriptionURI());

                                int addressFamily = AF_INET;

                                if (Utils::isIPv6(Utils::getHostURL(getLocation())))
                                    addressFamily = AF_INET6;

                                LoggerUtil_debug(logger, "URL: " << descURL.c_str());

                                int code;
                                if ( (code = UpnpRegisterRootDevice3(descURL.c_str(), deviceCallbackEventHandler, device_handle, device_handle, addressFamily)) != UPNP_E_SUCCESS) {
                                    LoggerUtil_debug(logger, "UPnPStatus: " << UPnPStatus::codeToString(code));

                                    if (UpnpRegisterRootDevice2(UPNPREG_BUF_DESC, getDescription(), 0, 1, deviceCallbackEventHandler, device_handle, device_handle) != UPNP_E_SUCCESS)
                                        return false;
                                }

                                if ( (code = UpnpUnRegisterRootDevice(*device_handle)) != UPNP_E_SUCCESS) {
                                    LoggerUtil_debug(logger, "UPnPStatus: " << UPnPStatus::codeToString(code));

                                    return false;
                                }

                                if ( (code = UpnpRegisterRootDevice3(descURL.c_str(), deviceCallbackEventHandler, device_handle, device_handle, addressFamily)) != UPNP_E_SUCCESS) {
                                    LoggerUtil_debug(logger, "UPnPStatus: " << UPnPStatus::codeToString(code));

                                    if (UpnpRegisterRootDevice2(UPNPREG_BUF_DESC, getDescription(), 0, 1, deviceCallbackEventHandler, device_handle, device_handle) != UPNP_E_SUCCESS)
                                        return false;
                                }

                                if ( (code = UpnpSendAdvertisement(*device_handle, 1800)) != UPNP_E_SUCCESS) {
                                    LoggerUtil_debug(logger, "UPnPStatus: " << UPnPStatus::codeToString(code));

                                    return false;
                                }

                                if (adsThread == NULL) {
                                    adsThread = new pthread_t;
                                    pthread_create(adsThread, NULL, sendAdvertisements, device_handle);
                                    pthread_detach(*adsThread);
                                }
                            }

                            LoggerUtil_info(logger, "Saindo do metodo start()");

                            return true;
                        }

                        bool UPnPDevice::stop() {
                            LoggerUtil_info(logger, "Entrando no metodo stop()");

                            if (isUPnPRootDevice() && (getUPnPLocalRootDeviceInstances()->indexOf(this) != -1)) {
                                if (getUPnPLocalRootDeviceInstances()->size() == 1) {
                                    UpnpUnRegisterRootDevice(*device_handle);
                                    if (UPnPDeviceController::getInstance()->isRunning() == false){
                                        UPnPDeviceController::deleteInstance();
                                        Utils::stopUPnP();
                                    }
                                } else {
                                    UpnpUnRegisterRootDevice(*device_handle);
                                }
                                getUPnPLocalRootDeviceInstances()->remove(this);
                                if (adsThread != NULL) {
                                    pthread_cancel(*adsThread);
                                    pthread_detach(*adsThread);
                                    delete adsThread;
                                    adsThread = NULL;
                                }
                                return true;
                            } else {
                                LoggerUtil_info(logger, "Saindo do metodo stop()");

                                return false;
                            }
                        }

                        void UPnPDevice::setUPnPActionListener(UPnPActionListener* listener) {
                            UPnPServiceList* list = getUPnPServices();
                            list->setUPnPActionListener(listener);
                        }

                        void UPnPDevice::setUPnPQueryListener(UPnPQueryListener* listener) {
                            UPnPServiceList* list = getUPnPServices();
                            list->setUPnPQueryListener(listener);
                        }

                        void UPnPDevice::setUPnPActionListener(UPnPActionListener* listener, bool embeddedDevices) {
                            setUPnPActionListener(listener);
                            if (embeddedDevices) {
                                UPnPDeviceList* list = getEmbeddedDevices();
                                list->setUPnPActionListener(listener, true);
                            }
                        }

                        void UPnPDevice::setUPnPQueryListener(UPnPQueryListener* listener, bool embeddedDevices) {
                            setUPnPQueryListener(listener);
                            if (embeddedDevices) {
                                UPnPDeviceList* list = getEmbeddedDevices();
                                list->setUPnPQueryListener(listener, true);
                            }
                        }

                        UPnPDeviceList* UPnPDevice::getUPnPLocalRootDeviceInstances() {
                            if (localRootDeviceInstances == NULL) {
                                localRootDeviceInstances = new UPnPDeviceList();
                            }

                            return localRootDeviceInstances;
                        }

                        void UPnPDevice::setNode(NodePtr node) {
                            this->node = node;
                        }

                        void UPnPDevice::setRootNode(NodePtr node) {
                            rootnode = node;
                        }

                        void UPnPDevice::setUPnPRootDevice(UPnPDevice* root) {
                            this->root = root;
                        }

                        void UPnPDevice::setURLBase(const char* url) {
                            if (isUPnPRootDevice()) {
                                Utils::setNode(getRootNode(), URLBASE, url);
                            }
                        }

                        void UPnPDevice::restoreUUID(const char* path){
                            LoggerUtil_info(logger, "Entrando no metodo restoreUUID(path)");

                            string tmpUUID;
                            char* c = new char[2];
                            c[1] = '\0';
                            int configfd = open(path, O_RDONLY);
                            if (configfd != -1) {
                                while(read(configfd, c, 1) > 0){
                                    tmpUUID.append(c);
                                }
                                close(configfd);
                            }
                            delete[] c;

                            LoggerUtil_debug(logger, "tmpUUID: " << tmpUUID.c_str());

                            if(tmpUUID.length() > 0){
                                setUUID(tmpUUID.c_str());
                            } else {
                                configfd = open(path, O_CREAT|O_TRUNC|O_WRONLY, 0644);
                                if (configfd != -1){
                                    write(configfd, getUUID(), strlen(getUUID()));
                                    close(configfd);
                                }
                            }
                        }

                        void UPnPDevice::setUUID(const char* uuid) {
                            LoggerUtil_info(logger, "Entrando no metodo setUUID(uuid)");
                            LoggerUtil_debug(logger, "uuid: " << uuid);

                            this->uuid = uuid;
                        }
                        
                        const char* UPnPDevice::getUUID() {
                            LoggerUtil_info(logger, "Entrando no metodo getUUID()");
                            LoggerUtil_debug(logger, "uuid: " << uuid.c_str());

                            if(uuid.length() <= 0){
                                uuid = Utils::createRandomUUID();
                            }
                            return uuid.c_str();
                        }

                        const char* UPnPDevice::getDescription() {
                            if (isUPnPRootDevice()) {
                                return ixmlPrintDocument((IXML_Document*) getRootNode());
                            } else {
                                return "";
                            }
                        }

                        void UPnPDevice::initializeLists() {
                            initializeDeviceList();
                            initializeServiceList();
                            initializeIconList();
                        }

                        void UPnPDevice::initializeDeviceList() {
                            LoggerUtil_info(logger, "Entrando no metodo initializeDeviceList()");

                            embeddedDevices.clear();
                            IXML_Node* devnode = (IXML_Node*) getNode();
                            if (devnode != NULL) {
                                IXML_Node* devListNode = devnode->firstChild;
                                while (devListNode != NULL) {
                                    if (UPnPDeviceList::isUPnPDeviceListNode(devListNode)) {
                                        IXML_Node* embeddedNode = devListNode->firstChild;
                                        while (embeddedNode != NULL) {
                                            if (UPnPDevice::isUPnPDeviceNode(embeddedNode)) {
                                                UPnPDevice* embedded = new UPnPDevice(embeddedNode);
                                                embedded->setUPnPParentDevice(this);
                                                embeddedDevices.add(embedded);
                                            }
                                            embeddedNode = embeddedNode->nextSibling;
                                        }
                                        return;
                                    }
                                    devListNode = devListNode->nextSibling;
                                }
                            }
                        }

                        void UPnPDevice::initializeServiceList() {
                            LoggerUtil_info(logger, "Entrando no metodo initializeServiceList()");

                            services.clear();
                            IXML_Node* devnode = (IXML_Node*) getNode();
                            if (devnode != NULL) {
                                IXML_Node* servListNode = devnode->firstChild;
                                while (servListNode != NULL) {
                                    if (UPnPServiceList::isUPnPServiceListNode(servListNode)) {
                                        IXML_Node* servNode = servListNode->firstChild;
                                        while (servNode != NULL) {
                                            if (UPnPService::isUPnPServiceNode(servNode)) {
                                                UPnPService* serv = new UPnPService(servNode, this);
                                                services.add(serv);
                                            }
                                            servNode = servNode->nextSibling;
                                        }
                                        return;
                                    }
                                    servListNode = servListNode->nextSibling;
                                }
                            }
                        }

                        void UPnPDevice::initializeIconList() {
                            LoggerUtil_info(logger, "Entrando no metodo initializeIconList()");

                            icons.clear();
                            IXML_Node* devnode = (IXML_Node*) getNode();
                            if (devnode != NULL) {
                                IXML_Node* iconListNode = devnode->firstChild;
                                while (iconListNode != NULL) {
                                    if (UPnPIconList::isUPnPIconListNode(iconListNode)) {
                                        IXML_Node* iconNode = iconListNode->firstChild;
                                        while (iconNode != NULL) {
                                            if (UPnPIcon::isUPnPIconNode(iconNode)) {
                                                UPnPIcon* icon = new UPnPIcon(iconNode);
                                                icons.add(icon);
                                            }
                                            iconNode = iconNode->nextSibling;
                                        }
                                        return;
                                    }
                                    iconListNode = iconListNode->nextSibling;
                                }
                            }
                        }

                    }
                }
            }
        }
    }
}
