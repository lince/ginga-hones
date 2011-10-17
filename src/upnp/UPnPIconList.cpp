#include "honesconfig.h"

#include <upnp/ixml.h>
#include "upnp/UPnPIcon.h"
#include "upnp/UPnPIconList.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        const string UPnPIconList::ELEMENT_NAME = "iconList";

                        UPnPIconList::UPnPIconList() {
                        }

                        UPnPIconList::~UPnPIconList() {
                            clear();
                        }

                        void UPnPIconList::clear() {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPIcon* value = getUPnPIcon(i);
                                if (value != NULL) {
                                    delete value;
                                    value = NULL;
                                }
                            }
                            Vector::clear();
                        }

                        bool UPnPIconList::isUPnPIconListNode(NodePtr node) {
                            return (ELEMENT_NAME.compare( ((IXML_Node*)node)->nodeName) == 0);
                        }

                        UPnPIcon* UPnPIconList::getUPnPIcon(int i) {
                            return (UPnPIcon*) get(i);
                        }

                    }
                }
            }
        }
    }
}
