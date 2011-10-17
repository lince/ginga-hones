#include "honesconfig.h"

#include <upnp/ixml.h>
#include "upnp/UPnPIcon.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        const string UPnPIcon::ELEMENT_NAME = "icon";
                        const string UPnPIcon::MIME_TYPE = "mimeType";
                        const string UPnPIcon::WIDTH = "width";
                        const string UPnPIcon::HEIGHT = "height";
                        const string UPnPIcon::DEPTH = "depth";
                        const string UPnPIcon::URL = "url";

                        UPnPIcon::UPnPIcon(NodePtr node) {
                            setNode(node);
                        }

                        UPnPIcon::~UPnPIcon() {
                        }

                        NodePtr UPnPIcon::getNode() {
                            return node;
                        }

                        bool UPnPIcon::isUPnPIconNode(NodePtr node) {
                            return (ELEMENT_NAME.compare( ((IXML_Node*)node)->nodeName) == 0);
                        }

                        void UPnPIcon::setMimeType(const char* value) {
                            Utils::setNode(getNode(), MIME_TYPE, value);
                        }

                        const char* UPnPIcon::getMimeType() {
                            return Utils::getNodeValue(getNode(), MIME_TYPE);
                        }

                        void UPnPIcon::setWidth(const char* value) {
                            Utils::setNode(getNode(), WIDTH, value);
                        }

                        const char* UPnPIcon::getWidth() {
                            return Utils::getNodeValue(getNode(), WIDTH);
                        }

                        void UPnPIcon::setHeight(const char* value) {
                            Utils::setNode(getNode(), HEIGHT, value);
                        }

                        const char* UPnPIcon::getHeight() {
                            return Utils::getNodeValue(getNode(), HEIGHT);
                        }

                        void UPnPIcon::setDepth(const char* value) {
                            Utils::setNode(getNode(), DEPTH, value);
                        }

                        const char* UPnPIcon::getDepth() {
                            return Utils::getNodeValue(getNode(), DEPTH);
                        }

                        void UPnPIcon::setURL(const char* value) {
                            Utils::setNode(getNode(), URL, value);
                        }

                        const char* UPnPIcon::getURL() {
                            return Utils::getNodeValue(getNode(), URL);
                        }

                        void UPnPIcon::setNode(NodePtr node) {
                            this->node = node;
                        }

                    }
                }
            }
        }
    }
}
