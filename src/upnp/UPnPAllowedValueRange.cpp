#include "honesconfig.h"

#include <stdlib.h>
#include <string.h>
#include <upnp/ixml.h>
#include "upnp/UPnPAllowedValueRange.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        const string UPnPAllowedValueRange::ELEMENT_NAME = "allowedValueRange";
                        const string UPnPAllowedValueRange::MAXIMUM = "maximum";
                        const string UPnPAllowedValueRange::MINIMUM = "minimum";
                        const string UPnPAllowedValueRange::STEP = "step";

                        UPnPAllowedValueRange::UPnPAllowedValueRange() {
                            IXML_Node* node = (IXML_Node*) malloc(sizeof(IXML_Node));
                            memset(node, 0, sizeof(IXML_Node));
                            setNode(node);
                        }

                        UPnPAllowedValueRange::UPnPAllowedValueRange(NodePtr node) {
                            setNode(node);
                        }

                        UPnPAllowedValueRange::~UPnPAllowedValueRange() {
                            if (node != NULL){
                                ixmlNode_free((IXML_Node*)node);
                                node = NULL;
                            }
                        }

                        void UPnPAllowedValueRange::setNode(NodePtr node) {
                            this->node = node;
                        }

                        NodePtr UPnPAllowedValueRange::getNode() {
                            return node;
                        }

                        bool UPnPAllowedValueRange::isUPnPAllowedValueRangeNode(NodePtr node) {
                            return (ELEMENT_NAME.compare( ((IXML_Node*)node)->nodeName) == 0);
                        }

                        void UPnPAllowedValueRange::setMinimumValue(const char *value) {
                            Utils::setNode(getNode(), MINIMUM, value);
                        }

                        const char* UPnPAllowedValueRange::getMinimumValue() {
                            return Utils::getNodeValue(getNode(), MINIMUM);
                        }

                        void UPnPAllowedValueRange::setMaximumValue(const char *value) {
                            Utils::setNode(getNode(), MAXIMUM, value);
                        }

                        const char* UPnPAllowedValueRange::getMaximumValue() {
                            return Utils::getNodeValue(getNode(), MAXIMUM);
                        }

                        void UPnPAllowedValueRange::setStepValue(const char *value) {
                            Utils::setNode(getNode(), STEP, value);
                        }

                        const char* UPnPAllowedValueRange::getStepValue() {
                            return Utils::getNodeValue(getNode(), STEP);
                        }

                        bool UPnPAllowedValueRange::hasValueRange() {
                            return (getNode() != NULL);
                        }

                    }
                }
            }
        }
    }
}
