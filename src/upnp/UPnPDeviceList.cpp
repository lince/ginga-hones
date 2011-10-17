#include "honesconfig.h"

#include <upnp/ixml.h>
#include "upnp/UPnPDevice.h"
#include "upnp/UPnPService.h"
#include "upnp/UPnPStateVariable.h"
#include "upnp/UPnPAction.h"
#include "upnp/UPnPActionListener.h"
#include "upnp/UPnPQueryListener.h"
#include "upnp/UPnPDeviceList.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        const string UPnPDeviceList::ELEMENT_NAME = "deviceList";

                        UPnPDeviceList::UPnPDeviceList() {
                        }

                        UPnPDeviceList::~UPnPDeviceList() {
                            clear();
                        }

                        void UPnPDeviceList::clear() {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPDevice* value = getUPnPDevice(i);
                                if (value != NULL) {
                                    delete value;
                                    value = NULL;
                                }
                            }
                            Vector::clear();
                        }

                        bool UPnPDeviceList::isUPnPDeviceListNode(NodePtr node) {
                            return (ELEMENT_NAME.compare( ((IXML_Node*)node)->nodeName) == 0);
                        }

                        UPnPDevice* UPnPDeviceList::getUPnPDevice(int i) {
                            return (UPnPDevice*) Vector::get(i);
                        }

                        UPnPDevice* UPnPDeviceList::getUPnPDevice(const char* name) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPDevice* dev = getUPnPDevice(i);
                                if (dev->isUPnPDevice(name))
                                    return dev;
                                UPnPDevice* embedded = dev->getUPnPDevice(name);
                                if (embedded != NULL)
                                    return embedded;
                            }
                            return NULL;
                        }

                        UPnPDevice* UPnPDeviceList::getUPnPDeviceByDescriptionURI(const char* uri) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPDevice* dev = getUPnPDevice(i);
                                if (dev->isDescriptionURI(uri))
                                    return dev;
                                UPnPDevice* embedded = dev->getUPnPDeviceByDescriptionURI(uri);
                                if (embedded != NULL)
                                    return embedded;
                            }
                            return NULL;
                        }

                        UPnPService* UPnPDeviceList::getUPnPService(const char* name) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPDevice* dev = getUPnPDevice(i);
                                UPnPService* serv = dev->getUPnPService(name);
                                if (serv != NULL)
                                    return serv;
                            }
                            return NULL;
                        }

                        UPnPService* UPnPDeviceList::getUPnPServiceBySCPDURL(const char* url) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPDevice* dev = getUPnPDevice(i);
                                UPnPService* serv = dev->getUPnPServiceBySCPDURL(url);
                                if (serv != NULL)
                                    return serv;
                            }
                            return NULL;
                        }

                        UPnPService* UPnPDeviceList::getUPnPServiceByControlURL(const char* url) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPDevice* dev = getUPnPDevice(i);
                                UPnPService* serv = dev->getUPnPServiceByControlURL(url);
                                if (serv != NULL)
                                    return serv;
                            }
                            return NULL;
                        }

                        UPnPService* UPnPDeviceList::getUPnPServiceByEventSubURL(const char* url) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPDevice* dev = getUPnPDevice(i);
                                UPnPService* serv = dev->getUPnPServiceByEventSubURL(url);
                                if (serv != NULL)
                                    return serv;
                            }
                            return NULL;
                        }

                        UPnPStateVariable* UPnPDeviceList::getUPnPStateVariable(const char* name) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPDevice* dev = getUPnPDevice(i);
                                UPnPStateVariable* var = dev->getUPnPStateVariable(name);
                                if (var != NULL)
                                    return var;
                            }
                            return NULL;
                        }

                        UPnPAction* UPnPDeviceList::getUPnPAction(const char* name) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPDevice* dev = getUPnPDevice(i);
                                UPnPAction* var = dev->getUPnPAction(name);
                                if (var != NULL)
                                    return var;
                            }
                            return NULL;
                        }

                        void UPnPDeviceList::setUPnPActionListener(UPnPActionListener* listener, bool embeddedDevices) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPDevice* dev = getUPnPDevice(i);
                                dev->setUPnPActionListener(listener, embeddedDevices);
                            }
                        }

                        void UPnPDeviceList::setUPnPQueryListener(UPnPQueryListener* listener, bool embeddedDevices) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPDevice* dev = getUPnPDevice(i);
                                dev->setUPnPQueryListener(listener, embeddedDevices);
                            }
                        }

                    }
                }
            }
        }
    }
}
