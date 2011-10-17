#include "honesconfig.h"

#include <upnp/ixml.h>
#include "upnp/UPnPArgument.h"
#include "upnp/UPnPArgumentList.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        const string UPnPArgumentList::ELEMENT_NAME = "argumentList";

                        UPnPArgumentList::UPnPArgumentList() {
                        }

                        UPnPArgumentList::~UPnPArgumentList() {
                            clear();
                        }

                        void UPnPArgumentList::clear() {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                UPnPArgument* value = getUPnPArgument(i);
                                if (value != NULL) {
                                    delete value;
                                    value = NULL;
                                }
                            }
                            Vector::clear();
                        }

                        bool UPnPArgumentList::isUPnPArgumentListNode(NodePtr node) {
                            return (ELEMENT_NAME.compare( ((IXML_Node*)node)->nodeName) == 0);
                        }

                        UPnPArgument* UPnPArgumentList::getUPnPArgument(int i) {
                            return (UPnPArgument*) get(i);
                        }

                        UPnPArgument* UPnPArgumentList::getUPnPArgument(const char* name) {
                            if (name != NULL) {
                                string argName = name;
                                int size = Vector::size();
                                for (int i = 0; i < size; i++) {
                                    UPnPArgument* arg = getUPnPArgument(i);
                                    if (argName.compare(arg->getName()) == 0) {
                                        return arg;
                                    }
                                }
                            }
                            return NULL;
                        }

                        void UPnPArgumentList::setUPnPArgumentListValues(UPnPArgumentList* list) {
                            int size = list->size();
                            for (int i = 0; i < size; i++) {
                                UPnPArgument* in = list->getUPnPArgument(i);
                                UPnPArgument* arg = getUPnPArgument(in->getName());
                                if (arg != NULL) {
                                    arg->setValue(in->getValue());
                                }
                            }
                        }

                    }
                }
            }
        }
    }
}
