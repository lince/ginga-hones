#include "honesconfig.h"

#include "upnp/UPnPAction.h"
#include "upnp/UPnPArgumentList.h"
#include "upnp/UPnPActionResponse.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        UPnPActionResponse::UPnPActionResponse() {
                            action = NULL;
                            response = NULL;
                        }

                        UPnPActionResponse::~UPnPActionResponse() {
                            if(response != NULL)
                                delete response;
                        }

                        void UPnPActionResponse::setUPnPAction(UPnPAction* action) {
                            this->action = action;
                        }

                        UPnPAction* UPnPActionResponse::getUPnPAction() {
                            return action;
                        }

                        void UPnPActionResponse::setResponse(UPnPArgumentList* response) {
                            this->response = response;
                        }

                        UPnPArgumentList* UPnPActionResponse::getResponse() {
                            return response;
                        }

                        void UPnPActionResponse::setFaultResponse(int errCode, const char* description) {
                            status.setCode(errCode);
                            status.setDescription(description);
                        }

                        UPnPStatus* UPnPActionResponse::getStatus() {
                            return &status;
                        }

                        bool UPnPActionResponse::isSuccessful() {
                            return (response != NULL);
                        }
                    }
                }
            }
        }
    }
}
