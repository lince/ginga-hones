#include "honesconfig.h"

#include <string.h>
#include <stdlib.h>
#include <upnp/ixml.h>
#include "upnp/UPnPService.h"
#include "upnp/UPnPStateVariable.h"
#include "upnp/UPnPArgument.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        const string UPnPArgument::ELEMENT_NAME = "argument";
                        const string UPnPArgument::IN_DIRECTION = "in";
                        const string UPnPArgument::OUT_DIRECTION = "out";
                        const string UPnPArgument::NAME = "name";
                        const string UPnPArgument::DIRECTION = "direction";
                        const string UPnPArgument::RELATED_STATE_VARIABLE = "relatedStateVariable";

                        UPnPArgument::UPnPArgument() {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpargument");
                            LoggerUtil_info(logger, "Entrando no construtor UPnPArgument()!");

                            setUPnPService(NULL);
                            IXML_Node* node = (IXML_Node*) malloc(sizeof(IXML_Node));
                            memset(node, 0, sizeof(IXML_Node));
                            setNode(node);
                        }

                        UPnPArgument::UPnPArgument(NodePtr node) {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpargument");
                            LoggerUtil_info(logger, "Entrando no construtor UPnPArgument(node)!");

                            setUPnPService(NULL);
                            setNode(node);
                        }

                        UPnPArgument::UPnPArgument(const char* name, const char* value) {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.upnp.upnpargument");
                            LoggerUtil_info(logger, "Entrando no construtor UPnPArgument(name, value)!");

                            setUPnPService(NULL);
                            IXML_Node* node = (IXML_Node*) malloc(sizeof(IXML_Node));
                            memset(node, 0, sizeof(IXML_Node));
                            setNode(node);
                            setName(name);
                            setValue(value);
                        }

                        UPnPArgument::~UPnPArgument() {
                            if (node != NULL){
                                ixmlNode_free((IXML_Node*)node);
                                node = NULL;
                            }
                        }

                        NodePtr UPnPArgument::getNode() {
                            return node;
                        }

                        UPnPService* UPnPArgument::getUPnPService() {
                            return service;
                        }

                        void UPnPArgument::setUPnPService(UPnPService* service) {
                            this->service = service;
                        }

                        bool UPnPArgument::isUPnPArgumentNode(NodePtr node) {
                            return (ELEMENT_NAME.compare( ((IXML_Node*)node)->nodeName) == 0);
                        }

                        void UPnPArgument::setName(const char* value) {
                            LoggerUtil_info(logger, "Entrando no metodo setName(value)");

                            Utils::setNode(getNode(), NAME, value);
                        }

                        const char* UPnPArgument::getName() {
                            return Utils::getNodeValue(getNode(), NAME);
                        }

                        void UPnPArgument::setDirection(const char* value) {
                            Utils::setNode(getNode(), DIRECTION, value);
                        }

                        const char* UPnPArgument::getDirection() {
                            return Utils::getNodeValue(getNode(), DIRECTION);
                        }

                        bool UPnPArgument::isInDirection() {
                            if (IN_DIRECTION.compare(getDirection()) == 0)
                                return true;
                            else
                                return false;
                        }

                        bool UPnPArgument::isOutDirection() {
                            return !isInDirection();
                        }

                        void UPnPArgument::setUPnPRelatedStateVariableName(const char* value) {
                            Utils::setNode(getNode(), RELATED_STATE_VARIABLE, value);
                        }

                        const char* UPnPArgument::getUPnPRelatedStateVariableName() {
                            return Utils::getNodeValue(getNode(), RELATED_STATE_VARIABLE);
                        }

                        UPnPStateVariable* UPnPArgument::getUPnPRelatedStateVariable() {
                            if (getUPnPService() == NULL)
                                return NULL;
                            else
                                return getUPnPService()->getUPnPStateVariable(getUPnPRelatedStateVariableName());
                        }

                        void UPnPArgument::setValue(const char *value) {
                            LoggerUtil_info(logger, "Entrando no metodo setValue(value)");

                            argValue = value;
                        }

                        void UPnPArgument::setValue(int value) {
                            setValue(Utils::integerToString(value));
                        }

                        const char* UPnPArgument::getValue() {
                            if (argValue.length() > 0)
                                return argValue.c_str();
                            else
                                return Utils::getNodeValue(getNode());
                        }

                        int UPnPArgument::getIntegerValue() {
                            return Utils::stringToInteger(getValue());
                        }

                        void UPnPArgument::setNode(NodePtr node) {
                            this->node = node;
                        }

                    }
                }
            }
        }
    }
}
