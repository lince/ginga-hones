#include "honesconfig.h"

#include <upnp/ixml.h>
#include "upnp/UPnPService.h"
#include "upnp/UPnPStateVariable.h"
#include "upnp/UPnPAction.h"
#include "upnp/UPnPActionList.h"
#include "upnp/UPnPActionListener.h"
#include "upnp/UPnPQueryListener.h"
#include "upnp/UPnPServiceList.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        const string UPnPServiceList::ELEMENT_NAME = "serviceList";

                        UPnPServiceList::UPnPServiceList() {
                        }

                        UPnPServiceList::~UPnPServiceList() {
                            clear();
                        }

                        void UPnPServiceList::clear() {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPService* value = getUPnPService(i);
                                if (value != NULL) {
                                    delete get(i);
                                    value = NULL;
                                }
                            }
                            Vector::clear();
                        }

                        bool UPnPServiceList::isUPnPServiceListNode(NodePtr node) {
                            return (ELEMENT_NAME.compare( ((IXML_Node*)node)->nodeName) == 0);
                        }

                        UPnPService* UPnPServiceList::getUPnPService(int i) {
                            return (UPnPService*) get(i);
                        }

                        UPnPService* UPnPServiceList::getUPnPService(const char* name) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPService* serv = getUPnPService(i);
                                if (serv->isUPnPService(name))
                                    return serv;
                            }
                            return NULL;
                        }

                        UPnPService* UPnPServiceList::getUPnPServiceBySCPDURL(const char* url) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPService* serv = getUPnPService(i);
                                if (serv->isSCPDURL(url))
                                    return serv;
                            }
                            return NULL;
                        }

                        UPnPService* UPnPServiceList::getUPnPServiceByControlURL(const char* url) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPService* serv = getUPnPService(i);
                                if (serv->isControlURL(url))
                                    return serv;
                            }
                            return NULL;
                        }

                        UPnPService* UPnPServiceList::getUPnPServiceByEventSubURL(const char* url) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPService* serv = getUPnPService(i);
                                if (serv->isEventSubURL(url))
                                    return serv;
                            }
                            return NULL;
                        }

                        UPnPStateVariable* UPnPServiceList::getUPnPStateVariable(const char* name) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPService* serv = getUPnPService(i);
                                UPnPStateVariable* var = serv->getUPnPStateVariable(name);
                                if (var != NULL)
                                    return var;
                            }
                            return NULL;
                        }

                        UPnPAction* UPnPServiceList::getUPnPAction(const char* name) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPService* serv = getUPnPService(i);
                                UPnPActionList* list = serv->getUPnPActions();
                                UPnPAction* var = list->getUPnPAction(name);
                                if (var != NULL)
                                    return var;
                            }
                            return NULL;
                        }

                        void UPnPServiceList::setUPnPActionListener(UPnPActionListener* listener) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPService* serv = getUPnPService(i);
                                serv->setUPnPActionListener(listener);
                            }
                        }

                        void UPnPServiceList::setUPnPQueryListener(UPnPQueryListener* listener) {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPService* serv = getUPnPService(i);
                                serv->setUPnPQueryListener(listener);
                            }
                        }

                    }
                }
            }
        }
    }
}
