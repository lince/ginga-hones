#include "honesconfig.h"

#include <upnp/ixml.h>
#include "upnp/UPnPStateVariable.h"
#include "upnp/UPnPServiceStateTable.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        const string UPnPServiceStateTable::ELEMENT_NAME = "serviceStateTable";

                        UPnPServiceStateTable::UPnPServiceStateTable() {
                        }

                        UPnPServiceStateTable::~UPnPServiceStateTable() {
                            clear();
                        }

                        void UPnPServiceStateTable::clear() {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPStateVariable* value = getUPnPStateVariable(i);
                                if (value != NULL) {
                                    delete value;
                                    value = NULL;
                                }
                            }
                            Vector::clear();
                        }

                        bool UPnPServiceStateTable::isUPnPServiceStateTableNode(NodePtr node) {
                            return (ELEMENT_NAME.compare( ((IXML_Node*)node)->nodeName) == 0);
                        }

                        UPnPStateVariable* UPnPServiceStateTable::getUPnPStateVariable(int i) {
                            return (UPnPStateVariable*) get(i);
                        }

                        UPnPStateVariable* UPnPServiceStateTable::getUPnPStateVariable(const char* name) {
                            if (name != NULL) {
                                string nameStr = name;
                                int size = Vector::size();
                                for (int i = 0; i < size; i++) {
                                    UPnPStateVariable* arg = getUPnPStateVariable(i);
                                    if (nameStr.compare(arg->getName()) == 0) {
                                        return arg;
                                    }
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
