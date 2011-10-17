#include "honesconfig.h"

#include <upnp/ixml.h>
#include <upnp/upnp.h>
#include <upnp/StateVarRequest.h>
#include "upnp/UPnPQueryResponse.h"
#include "upnp/UPnPStateVariable.h"
#include "upnp/UPnPService.h"
#include "upnp/UPnPDeviceController.h"
#include "upnp/UPnPQueryRequest.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        UPnPQueryRequest::UPnPQueryRequest() {
                            queryRequest = NULL;
                            var = NULL;
                        }

                        UPnPQueryRequest::UPnPQueryRequest(UpnpStateVarRequestPtr queryRequest) {
                            this->queryRequest = queryRequest;
                            var = NULL;
                        }

                        void UPnPQueryRequest::setRequest(UPnPStateVariable* var) {
                            this->var = var;
                        }

                        void UPnPQueryRequest::send(UPnPQueryResponse* res) {
                            if (queryRequest != NULL) {
                                if (res->getUPnPStateVariable() != NULL) {
                                    UPnPStateVariable* varOut = res->getUPnPStateVariable();
                                    UpnpStateVarRequest_set_CurrentVal((UpnpStateVarRequest*) queryRequest, ixmlCloneDOMString(varOut->getValue()));
                                    UpnpStateVarRequest_set_ErrCode((UpnpStateVarRequest*) queryRequest, UPNP_E_SUCCESS);
                                }
                            } else if (var != NULL) {
                                UPnPDeviceController* deviceCtrl = UPnPDeviceController::getInstance();

                                if (deviceCtrl->isRunning() == false)
                                    deviceCtrl->start();

                                deviceCtrl->sendUPnPQueryRequest(var, res);
                            }
                        }

                    }
                }
            }
        }
    }
}
