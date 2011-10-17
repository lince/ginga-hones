#include "honesconfig.h"

#include <string.h>
#include <stdlib.h>
#include <upnp/ixml.h>
#include "upnp/UPnPAllowedValue.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        const string UPnPAllowedValue::ELEMENT_NAME = "allowedValue";

                        UPnPAllowedValue::UPnPAllowedValue() {
                            IXML_Node* node = (IXML_Node*) malloc(sizeof(IXML_Node));
                            memset(node, 0, sizeof(IXML_Node));
                            setNode(node);
                        }

                        UPnPAllowedValue::UPnPAllowedValue(NodePtr node) {
                            setNode(node);
                        }

                        UPnPAllowedValue::~UPnPAllowedValue(){
                            if (node != NULL){
                                ixmlNode_free((IXML_Node*)node);
                                node = NULL;
                            }
                        }

                        void UPnPAllowedValue::setNode(NodePtr node) {
                            this->node = node;
                        }

                        NodePtr UPnPAllowedValue::getNode() {
                            return node;
                        }

                        bool UPnPAllowedValue::isUPnPAllowedValueNode(NodePtr node) {
                            return (ELEMENT_NAME.compare( ((IXML_Node*)node)->nodeName) == 0);
                        }

                        void UPnPAllowedValue::setValue(const char *value) {
                            Utils::setNodeValue(getNode(), value);
                        }

                        const char* UPnPAllowedValue::getValue() {
                            return Utils::getNodeValue(getNode());
                        }

                    }
                }
            }
        }
    }
}
