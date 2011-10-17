#include "honesconfig.h"

#include <upnp/ixml.h>
#include "upnp/UPnPAllowedValueList.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        const string UPnPAllowedValueList::ELEMENT_NAME = "allowedValueList";

                        UPnPAllowedValueList::UPnPAllowedValueList() {
                        }

                        UPnPAllowedValueList::~UPnPAllowedValueList() {
                            clear();
                        }

                        void UPnPAllowedValueList::clear() {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPAllowedValue* value = getUPnPAllowedValue(i);
                                if (value != NULL) {
                                    delete value;
                                    value = NULL;
                                }
                            }
                            Vector::clear();
                        }

                        bool UPnPAllowedValueList::isUPnPAllowedValueListNode(NodePtr node) {
                            return (ELEMENT_NAME.compare( ((IXML_Node*)node)->nodeName) == 0);
                        }

                        UPnPAllowedValue* UPnPAllowedValueList::getUPnPAllowedValue(int i) {
                            return (UPnPAllowedValue*) get(i);
                        }

                    }
                }
            }
        }
    }
}
