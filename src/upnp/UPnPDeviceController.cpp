#include "honesconfig.h"

#include <string.h>
#include <upnp/upnp.h>
#include <upnp/ixml.h>
#include "upnp/UPnPStateVariable.h"
#include "upnp/UPnPServiceList.h"
#include "upnp/UPnPArgument.h"
#include "upnp/UPnPArgumentList.h"
#include "upnp/UPnPServiceStateTable.h"
#include "upnp/UPnPDeviceController.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        UPnPDeviceController* UPnPDeviceController::instance = NULL;

                        UPnPDeviceController::UPnPDeviceController() {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpdevicecontroller");
                            LoggerUtil_info(logger, "Entrando no construtor UPnPDeviceController()!");

                            client_handle = new int;
                            *client_handle = -1;
                            setRunning(false);
                            searchTime = 3;
                            subscriptionTimeOut = 1801;
                        }

                        UPnPDeviceController::~UPnPDeviceController() {
                            LoggerUtil_info(logger, "Entrando no destrutor ~UPnPDeviceController()!");
                            if (client_handle != NULL){
                                delete client_handle;
                                client_handle = NULL;
                            }
                        }

                        UPnPDeviceController* UPnPDeviceController::getInstance() {
                            if(instance == NULL){
                                instance = new UPnPDeviceController();
                            }
                            return instance;
                        }

                        void UPnPDeviceController::deleteInstance() {
                            if(instance != NULL){
                                delete instance;
                                instance = NULL;
                            }
                        }

                        void UPnPDeviceController::setRunning(bool running) {
                            LoggerUtil_info(logger, "Entrando no metodo setRunning");
                            
                            this->running = running;
                        }
                        
                        bool UPnPDeviceController::isRunning() {
                            LoggerUtil_info(logger, "Entrando no metodo isRunning");

                            return running;
                        }

                        static int clientCallbackEventHandler(Upnp_EventType EventType, void* Event, void* Cookie) {
                            UPnPDeviceController* deviceCtrl = UPnPDeviceController::getInstance();
                            switch (EventType) {
                                /*
                                 * Novo dispositivo na rede ou resultado de busca.
                                 */
                                case UPNP_DISCOVERY_ADVERTISEMENT_ALIVE:
                                case UPNP_DISCOVERY_SEARCH_RESULT:
                                {
                                    UpnpDiscovery* dEvent = (UpnpDiscovery*) Event;

                                    deviceCtrl->addUPnPDevice(UpnpString_get_String(UpnpDiscovery_get_Location(dEvent)));

                                    break;
                                }

                                /*
                                 * Terminou o tempo de busca.
                                 */
                                case UPNP_DISCOVERY_SEARCH_TIMEOUT:
                                    break;

                                /*
                                 * Dispositivo saiu da rede.
                                 */
                                case UPNP_DISCOVERY_ADVERTISEMENT_BYEBYE:
                                {
                                    UpnpDiscovery* dEvent = (UpnpDiscovery*) Event;

                                    deviceCtrl->removeUPnPDevice(UpnpString_get_String(UpnpDiscovery_get_DeviceID(dEvent)));
                                    
                                    break;
                                }

                                /*
                                 * Executou uma ação em um dispositivo.
                                 * Nenhum processamento aqui.
                                 * O tratamento dos resultados ocorre na classe UPnPActionRequest.
                                 */
                                case UPNP_CONTROL_ACTION_COMPLETE:
                                    break;

                                /*
                                 * Terminou a solicitação de uma variável de estado.
                                 * Nenhum processamento aqui.
                                 * O tratamento dos resultados ocorre na classe UPnPQueryRequest.
                                 */
                                case UPNP_CONTROL_GET_VAR_COMPLETE:
                                    break;

                                /*
                                 * Recebeu uma notificação de alteração de uma variável de estado.
                                 */
                                case UPNP_EVENT_RECEIVED:
                                {
                                    UpnpEvent* eEvent = (UpnpEvent*) Event;
                                    IXML_Document* xml = UpnpEvent_get_ChangedVariables(eEvent);
                                    const char* sid = UpnpString_get_String(UpnpEvent_get_SID(eEvent));
                                    
                                    UPnPDeviceList* devList = deviceCtrl->getUPnPDeviceList();
                                    int devListSize = devList->size();
                                    for(int i=0; i < devListSize; i++){
                                        UPnPDevice* device = devList->getUPnPDevice(i);
                                        if (deviceCtrl->updateUPnPStateVariables(device, sid, xml) == true)
                                            i = devListSize;
                                    }

                                    break;
                                }

                                /*
                                 * Terminou o processo de cancelamento de subscrição.
                                 */
                                case UPNP_EVENT_UNSUBSCRIBE_COMPLETE:
                                {
                                    UpnpEventSubscribe* esEvent = (UpnpEventSubscribe*) Event;

                                    if (UpnpEventSubscribe_get_ErrCode(esEvent) == UPNP_E_SUCCESS) {
                                        UPnPService* service = deviceCtrl->getUPnPDeviceList()->getUPnPServiceByEventSubURL(UpnpString_get_String(UpnpEventSubscribe_get_PublisherUrl(esEvent)));
                                        if(service != NULL)
                                            service->setSubscriptionID(NULL);
                                    }

                                    break;
                                }

                                /*
                                 * Terminou o processo de subscrição.
                                 */
                                case UPNP_EVENT_SUBSCRIBE_COMPLETE:
                                case UPNP_EVENT_RENEWAL_COMPLETE:
                                {
                                    UpnpEventSubscribe* esEvent = (UpnpEventSubscribe*) Event;

                                    if (UpnpEventSubscribe_get_ErrCode(esEvent) == UPNP_E_SUCCESS) {
                                        UPnPService* service = deviceCtrl->getUPnPDeviceList()->getUPnPServiceByEventSubURL(UpnpString_get_String(UpnpEventSubscribe_get_PublisherUrl(esEvent)));
                                        if(service != NULL)
                                            service->setSubscriptionID(strdup(UpnpString_get_String(UpnpEventSubscribe_get_SID(esEvent))));
                                    }

                                    break;
                                }

                                /*
                                 * Falha na subscrição, tentar novamente.
                                 */
                                case UPNP_EVENT_AUTORENEWAL_FAILED:
                                case UPNP_EVENT_SUBSCRIPTION_EXPIRED:
                                {
                                    int TimeOut = deviceCtrl->getSubscriptionTimeOut();
                                    Upnp_SID newSID;

                                    UpnpEventSubscribe* esEvent = (UpnpEventSubscribe*) Event;

                                    if (UpnpSubscribe(*((UpnpClient_Handle*) Cookie), UpnpString_get_String(UpnpEventSubscribe_get_PublisherUrl(esEvent)), &TimeOut, newSID) == UPNP_E_SUCCESS) {
                                        UPnPService* service = deviceCtrl->getUPnPDeviceList()->getUPnPServiceByEventSubURL(UpnpString_get_String(UpnpEventSubscribe_get_PublisherUrl(esEvent)));
                                        if(service != NULL)
                                            service->setSubscriptionID(strdup(newSID));
                                    }

                                    break;
                                }

                                /*
                                 * Ignora esses casos, pois não é um dispositivo.
                                 */
                                case UPNP_EVENT_SUBSCRIPTION_REQUEST:
                                case UPNP_CONTROL_GET_VAR_REQUEST:
                                case UPNP_CONTROL_ACTION_REQUEST:
                                    break;

                            }
                            return 0;
                        }

                        bool UPnPDeviceController::start() {
                            LoggerUtil_info(logger, "Entrando no metodo start");

                            stop();

                            pthread_mutex_init(&mutex, NULL);

                            Utils::initializeUPnP();

                            if (UpnpRegisterClient(clientCallbackEventHandler, client_handle, client_handle) != UPNP_E_SUCCESS) {
                                return false;
                            }
                            
                            setRunning(true);

                            return true;
                        }

                        bool UPnPDeviceController::stop() {
                            LoggerUtil_info(logger, "Entrando no metodo stop");

                            if(isRunning() == true){
                                unsubscribe();
                                UpnpUnRegisterClient(*client_handle);
                                setRunning(false);
                                deviceList.clear();
                                pthread_mutex_destroy(&mutex);
                                if (UPnPDevice::getUPnPLocalRootDeviceInstances()->size() == 0)
                                    Utils::stopUPnP();
                                return true;
                            } else {
                                return false;
                            }
                        }

                        UpnpClient_HandlePtr UPnPDeviceController::getUPnPClientHandle(){
                            return client_handle;
                        }

                        void UPnPDeviceController::addUPnPDevice(const char* url){
                            LoggerUtil_info(logger, "Entrando no metodo addUPnPDevice(url)");
                            LoggerUtil_debug(logger, "url: " << url);

                            IXML_Document* xml = NULL;
                            if (UpnpDownloadXmlDoc(url, &xml) == UPNP_E_SUCCESS){
                                string location = "http://";
                                location.append(Utils::getHostURL(url));
                                location.append(":");
                                location.append(Utils::integerToString(Utils::getPortURL(url)));
                                UPnPDevice* dev = new UPnPDevice(ixmlNode_cloneNode(xml->n.firstChild, true), location.c_str());
                                if (hasUPnPDevice(dev->getUDN()) == false) {
                                    LoggerUtil_debug(logger, "Adicionando novo dispositivo: " << dev->getFriendlyName() << " ip: " << dev->getLocation());
                                    addUPnPDevice(dev);
                                } else {
                                    LoggerUtil_debug(logger, "Atualizando dispositivo: " << dev->getFriendlyName() << " ip: " << dev->getLocation());
                                    UPnPDevice* localDev = getUPnPDevice(dev->getUDN());
                                    localDev->setLocation(dev->getLocation());
                                    //localDev->loadDescription(ixmlNode_cloneNode((IXML_Node*)dev->getRootNode(), true));
                                    delete dev;
                                }
                                ixmlDocument_free(xml);
                            }
                        }

                        void UPnPDeviceController::addUPnPDevice(UPnPDevice* device){
                            LoggerUtil_info(logger, "Entrando no metodo addUPnPDevice(device)");

                            //pthread_mutex_lock(&mutex);
                            if (device != NULL) {
                                deviceList.add(device);

                                if (device->isUPnPRootDevice() == true) {
                                    int deviceListenersSize = deviceListeners.size();
                                    for (int i = 0; i < deviceListenersSize; i++) {
                                        UPnPDeviceListener* dListener = (UPnPDeviceListener*) deviceListeners.get(i);
                                        dListener->deviceAdded(device);
                                    }
                                }
                            }
                            //pthread_mutex_unlock(&mutex);
                        }

                        void UPnPDeviceController::removeUPnPDevice(UPnPDevice* device){
                            LoggerUtil_info(logger, "Entrando no metodo removeUPnPDevice(device)");

                            //pthread_mutex_lock(&mutex);
                            if (device != NULL) {
                                LoggerUtil_debug(logger, "Removendo dispositivo: " << device->getFriendlyName() << " ip: " << device->getLocation());

                                if (device->isUPnPRootDevice() == true) {
                                    int deviceListenersSize = deviceListeners.size();
                                    for (int i = 0; i < deviceListenersSize; i++) {
                                        UPnPDeviceListener* dListener = (UPnPDeviceListener*) deviceListeners.get(i);
                                        dListener->deviceRemoved(device);
                                    }
                                }

                                deviceList.remove(device);
                            }
                            //pthread_mutex_unlock(&mutex);
                        }

                        void UPnPDeviceController::removeUPnPDevice(const char* name){
                            LoggerUtil_info(logger, "Entrando no metodo removeUPnPDevice(name)");

                            removeUPnPDevice(getUPnPDevice(name));
                        }

                        UPnPDeviceList* UPnPDeviceController::getUPnPDeviceList() {
                            LoggerUtil_info(logger, "Entrando no metodo getUPnPDeviceList");

                            return &deviceList;
                        }

                        UPnPDevice* UPnPDeviceController::getUPnPDevice(const char* name) {
                            LoggerUtil_info(logger, "Entrando no metodo getUPnPDevice(name)");

                            return deviceList.getUPnPDevice(name);
                        }

                        bool UPnPDeviceController::hasUPnPDevice(const char* name) {
                            LoggerUtil_info(logger, "Entrando no metodo hasUPnPDevice(name)");

                            return (getUPnPDevice(name) != NULL);
                        }

                        void UPnPDeviceController::setSearchTime(int searchTime){
                            this->searchTime = searchTime;
                        }

                        int UPnPDeviceController::getSearchTime(){
                            return searchTime;
                        }

                        void UPnPDeviceController::setSubscriptionTimeOut(int timeOut){
                            subscriptionTimeOut = timeOut;
                        }

                        int UPnPDeviceController::getSubscriptionTimeOut(){
                            return subscriptionTimeOut;
                        }

                        bool UPnPDeviceController::search(const char* deviceType, int searchTime){
                            LoggerUtil_info(logger, "Entrando no metodo search(deviceType, searchTime)");

                            bool result = (UpnpSearchAsync(*client_handle, searchTime, deviceType, NULL) == UPNP_E_SUCCESS);
                            int count = searchTime * 10;
                            while((result == true) && (count > 0)){
                                Utils::wait(100);
                                count--;
                            }
                            return result;
                        }

                        bool UPnPDeviceController::search(const char* deviceType){
                            LoggerUtil_info(logger, "Entrando no metodo search(deviceType)");

                            return search(deviceType, getSearchTime());
                        }

                        bool UPnPDeviceController::searchAll(){
                            LoggerUtil_info(logger, "Entrando no metodo searchAll");
                            
                            return search("ssdp:all", getSearchTime());
                        }

                        bool UPnPDeviceController::searchRootDevices() {
                            LoggerUtil_info(logger, "Entrando no metodo searchRootDevices");
                            
                            return search("upnp:rootdevice", getSearchTime());
                        }

                        bool UPnPDeviceController::subscribe(UPnPService* service){
                            LoggerUtil_info(logger, "Entrando no metodo subscribe(service)");

                            int timeOut = getSubscriptionTimeOut();
                            if (service->isSubscribed() == true){
                                if (UpnpRenewSubscription(*client_handle, &timeOut, service->getSubscriptionID()) == UPNP_E_SUCCESS)
                                    return true;
                            } else {
                                Upnp_SID newSID;
                                string newurl = service->getEventSubURL();
                                if(Utils::isAbsoluteURL(newurl) == false){
                                    newurl = service->getUPnPDevice()->getLocation();
                                    newurl.append(Utils::getRelativeURL(service->getEventSubURL()));
                                }
                                if (UpnpSubscribe(*client_handle, newurl.c_str(), &timeOut, newSID) == UPNP_E_SUCCESS){
                                    service->setSubscriptionID(strdup(newSID));
                                    return true;
                                }
                            }
                            return false;
                        }

                        bool UPnPDeviceController::isSubscribed(UPnPService* service){
                            LoggerUtil_info(logger, "Entrando no metodo isSubscribed(service)");

                            return service->isSubscribed();
                        }

                        bool UPnPDeviceController::unsubscribe(UPnPService* service){
                            LoggerUtil_info(logger, "Entrando no metodo unsubscribe(service)");

                            if (service->isSubscribed() == true){
                                if (UpnpUnSubscribe(*client_handle, service->getSubscriptionID()) == UPNP_E_SUCCESS){
                                    service->setSubscriptionID(NULL);
                                    return true;
                                }
                            }
                            return false;
                        }

                        void UPnPDeviceController::unsubscribe(UPnPDevice* device){
                            LoggerUtil_info(logger, "Entrando no metodo unsubscribe(device)");

                            UPnPServiceList* list = device->getUPnPServices();
                            int size = list->size();
                            for(int i=0; i < size; i++){
                                UPnPService* serv = list->getUPnPService(i);
                                if(serv->isSubscribed() == true){
                                    unsubscribe(serv);
                                }
                            }

                            UPnPDeviceList* list2 = device->getEmbeddedDevices();
                            size = list2->size();
                            for(int i=0; i < size; i++){
                                unsubscribe(list2->getUPnPDevice(i));
                            }
                        }

                        void UPnPDeviceController::unsubscribe(){
                            LoggerUtil_info(logger, "Entrando no metodo unsubscribe");

                            UPnPDeviceList* list = getUPnPDeviceList();
                            int size = list->size();
                            for(int i=0; i < size; i++){
                                unsubscribe(list->getUPnPDevice(i));
                            }
                        }

                        void UPnPDeviceController::renewSubscriptions(UPnPDevice* device){
                            LoggerUtil_info(logger, "Entrando no metodo renewSubscriptions(device)");

                            UPnPServiceList* list = device->getUPnPServices();
                            int size = list->size();
                            for(int i=0; i < size; i++){
                                UPnPService* serv = list->getUPnPService(i);
                                if(serv->isSubscribed() == true){
                                    subscribe(serv);
                                }
                            }

                            UPnPDeviceList* list2 = device->getEmbeddedDevices();
                            size = list2->size();
                            for(int i=0; i < size; i++){
                                renewSubscriptions(list2->getUPnPDevice(i));
                            }
                        }

                        void UPnPDeviceController::renewSubscriptions(){
                            LoggerUtil_info(logger, "Entrando no metodo renewSubscriptions");

                            UPnPDeviceList* list = getUPnPDeviceList();
                            int size = list->size();
                            for(int i=0; i < size; i++){
                                renewSubscriptions(list->getUPnPDevice(i));
                            }
                        }

                        bool UPnPDeviceController::updateUPnPStateVariables(UPnPDevice* device, const char* sid, DocumentPtr changedVariables){
                            LoggerUtil_info(logger, "Entrando no metodo updateUPnPStateVariables(device, sid, changedVariables)");

                            IXML_Document* xml = (IXML_Document*) changedVariables;
                            UPnPServiceList* servList = device->getUPnPServices();

                            int servListSize = servList->size();
                            for (int k = 0; k < servListSize; k++) {
                                UPnPService* service = servList->getUPnPService(k);
                                if (service->isSubscribed() == true) {
                                    if (strcmp(service->getSubscriptionID(), sid) == 0) {
                                        UPnPServiceStateTable* stateTable = service->getUPnPServiceStateTable();
                                        int stateTableSize = stateTable->size();
                                        for (int j = 0; j < stateTableSize; j++) {
                                            UPnPStateVariable* var = stateTable->getUPnPStateVariable(j);
                                            IXML_Element* elem = ixmlDocument_getElementById(xml, var->getName());
                                            if (elem != NULL) {
                                                int eventListenersSize = eventListeners.size();
                                                for(int e=0; e < eventListenersSize; e++){
                                                    UPnPEventListener* eListener = (UPnPEventListener*) eventListeners.get(e);
                                                    eListener->eventReceived(device, service, sid, var->getName(), Utils::getNodeValue((IXML_Node*) elem));
                                                }
                                                var->setValue(Utils::getNodeValue((IXML_Node*) elem));
                                            }
                                        }
                                        return true;
                                    }
                                }
                            }

                            UPnPDeviceList* embDevList = device->getEmbeddedDevices();
                            int embDevListSize = embDevList->size();
                            for (int y = 0; y < embDevListSize; y++) {
                                if (updateUPnPStateVariables(embDevList->getUPnPDevice(y), sid, changedVariables) == true)
                                    return true;
                            }

                            return false;
                        }

                        bool UPnPDeviceController::addUPnPDeviceListener(UPnPDeviceListener* deviceListener){
                            LoggerUtil_info(logger, "Entrando no metodo addUPnPDeviceListener(deviceListener)");

                            return deviceListeners.add(deviceListener);
                        }

                        bool UPnPDeviceController::removeUPnPDeviceListener(UPnPDeviceListener* deviceListener){
                            LoggerUtil_info(logger, "Entrando no metodo removeUPnPDeviceListener(deviceListener)");

                            return deviceListeners.remove(deviceListener);
                        }

                        bool UPnPDeviceController::addUPnPEventListener(UPnPEventListener* eventListener){
                            LoggerUtil_info(logger, "Entrando no metodo addUPnPEventListener(eventListener)");

                            return eventListeners.add(eventListener);
                        }

                        bool UPnPDeviceController::removeUPnPEventListener(UPnPEventListener* eventListener){
                            LoggerUtil_info(logger, "Entrando no metodo removeUPnPEventListener(eventListener)");

                            return eventListeners.remove(eventListener);
                        }
                        
                        void UPnPDeviceController::sendUPnPAction(UPnPAction* action, UPnPActionResponse* res) {
                            IXML_Document* actionNode = NULL;
                            IXML_Document* respNode = NULL;
                            int size = action->getInputUPnPArguments()->size();
                            if (size == 0) {
                                Utils::addArgumentToUPnPAction(false, (DocumentPtr*) & actionNode, action->getName(), action->getUPnPService()->getServiceType(), NULL, NULL);
                            } else {
                                for (int i = 0; i < size; i++) {
                                    Utils::addArgumentToUPnPAction(false, (DocumentPtr*) & actionNode, action->getName(), action->getUPnPService()->getServiceType(), action->getInputUPnPArguments()->getUPnPArgument(i)->getName(), action->getInputUPnPArguments()->getUPnPArgument(i)->getValue());
                                }
                            }

                            int retCode;

                            string newurl = action->getUPnPService()->getControlURL();
                            if (Utils::isAbsoluteURL(newurl) == false) {
                                newurl = action->getUPnPService()->getUPnPDevice()->getLocation();
                                newurl.append(Utils::getRelativeURL(action->getUPnPService()->getControlURL()));
                            }

                            LoggerUtil_debug(logger, "ActionName: " << action->getName() << " ControlURL: " << newurl.c_str() << "\nServiceType: " << action->getUPnPService()->getServiceType());

                            if ((retCode = UpnpSendAction(*client_handle, newurl.c_str(), action->getUPnPService()->getServiceType(), NULL, actionNode, &respNode)) == UPNP_E_SUCCESS) {
                                UPnPArgumentList* list = new UPnPArgumentList();
                                int outSize = action->getOutputUPnPArguments()->size();
                                for (int k = 0; k < outSize; k++) {
                                    const char* argumentName = action->getOutputUPnPArguments()->getUPnPArgument(k)->getName();
                                    IXML_Element* elem = ixmlDocument_getElementById(respNode, argumentName);
                                    if (elem != NULL) {
                                        const char* argumentValue = Utils::getNodeValue(elem);
                                        list->add(new UPnPArgument(argumentName, argumentValue));
                                        LoggerUtil_debug(logger, "ArgumentName: " << argumentName << " ArgumentValue: " << argumentValue);
                                    }
                                }
                                res->setResponse(list);
                            } else {
                                LoggerUtil_debug(logger, "retCode: " << UPnPStatus::codeToString(retCode));

                                if(retCode == UPNP_E_INVALID_HANDLE)
                                    UpnpRegisterClient(clientCallbackEventHandler, client_handle, client_handle);

                                res->setFaultResponse(retCode, UPnPStatus::codeToString(retCode));
                            }

                            if (actionNode != NULL)
                                ixmlDocument_free(actionNode);
                            if (respNode != NULL)
                                ixmlDocument_free(respNode);
                        }

                        void UPnPDeviceController::sendUPnPQueryRequest(UPnPStateVariable* var, UPnPQueryResponse* res){
                            char* value = NULL;

                            int retCode;
                            string newurl = var->getUPnPService()->getControlURL();
                            if (Utils::isAbsoluteURL(newurl) == false) {
                                newurl = var->getUPnPService()->getUPnPDevice()->getLocation();
                                newurl.append(Utils::getRelativeURL(var->getUPnPService()->getControlURL()));
                            }
                            if ((retCode = UpnpGetServiceVarStatus(*client_handle, newurl.c_str(), var->getName(), &value)) == UPNP_E_SUCCESS) {
                                res->setValueReceived(value);
                            } else {
                                if(retCode == UPNP_E_INVALID_HANDLE)
                                    UpnpRegisterClient(clientCallbackEventHandler, client_handle, client_handle);
                                
                                res->setFaultResponse(retCode, UPnPStatus::codeToString(retCode));
                            }

                            if (value != NULL)
                                ixmlFreeDOMString(value);
                        }

                    }
                }
            }
        }
    }
}
