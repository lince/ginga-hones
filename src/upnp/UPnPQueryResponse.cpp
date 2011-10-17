#include "honesconfig.h"

#include <string.h>
#include "upnp/UPnPStateVariable.h"
#include "upnp/UPnPQueryResponse.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        UPnPQueryResponse::UPnPQueryResponse() {
                            var = NULL;
                            valueReceived = NULL;
                        }

                        UPnPQueryResponse::~UPnPQueryResponse() {
                            if (valueReceived != NULL)
                                delete[] valueReceived;
                        }

                        void UPnPQueryResponse::setUPnPStateVariable(UPnPStateVariable* var) {
                            this->var = var;
                        }

                        UPnPStateVariable* UPnPQueryResponse::getUPnPStateVariable() {
                            return var;
                        }

                        void UPnPQueryResponse::setFaultResponse(int errCode, const char* description) {
                            status.setCode(errCode);
                            status.setDescription(description);
                        }

                        UPnPStatus* UPnPQueryResponse::getStatus() {
                            return &status;
                        }

                        bool UPnPQueryResponse::isSuccessful() {
                            return (valueReceived != NULL);
                        }

                        void UPnPQueryResponse::setValueReceived(const char* value) {
                            if(value != NULL)
                                valueReceived = strdup(value);
                        }

                        const char* UPnPQueryResponse::getValueReceived() {
                            return valueReceived;
                        }

                    }
                }
            }
        }
    }
}
