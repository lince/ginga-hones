#include "honesconfig.h"

#include <upnp/ixml.h>
#include "upnp/UPnPAction.h"
#include "upnp/UPnPActionList.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        const string UPnPActionList::ELEMENT_NAME = "actionList";

                        UPnPActionList::UPnPActionList() {
                        }

                        UPnPActionList::~UPnPActionList() {
                            clear();
                        }

                        void UPnPActionList::clear() {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPAction* value = getUPnPAction(i);
                                if (value != NULL) {
                                    delete value;
                                    value = NULL;
                                }
                            }
                            Vector::clear();
                        }

                        bool UPnPActionList::isUPnPActionListNode(NodePtr node) {
                            return (ELEMENT_NAME.compare( ((IXML_Node*)node)->nodeName) == 0);
                        }

                        UPnPAction* UPnPActionList::getUPnPAction(int i) {
                            return (UPnPAction*) get(i);
                        }

                        UPnPAction* UPnPActionList::getUPnPAction(const char* name) {
                            if (name != NULL) {
                                string argName = name;
                                int size = Vector::size();
                                for (int i = 0; i < size; i++) {
                                    UPnPAction* arg = getUPnPAction(i);
                                    if (argName.compare(arg->getName()) == 0) {
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
