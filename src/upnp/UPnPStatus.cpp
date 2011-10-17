#include "honesconfig.h"

#include <upnp/upnp.h>
#include "upnp/UPnPStatus.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        UPnPStatus::UPnPStatus() {
                            UPnPStatus(0, "");
                        }

                        UPnPStatus::UPnPStatus(int code, const char* desc) {
                            setCode(code);
                            setDescription(desc);
                        }

                        void UPnPStatus::setCode(int code) {
                            this->code = code;
                        }

                        int UPnPStatus::getCode() {
                            return code;
                        }

                        void UPnPStatus::setDescription(const char* desc) {
                            description = desc;
                        }

                        const char* UPnPStatus::getDescription() {
                            return description.c_str();
                        }

                        const char* UPnPStatus::codeToString(int code) {
                            switch (code) {
                                case UPNP_E_SUCCESS:
                                    return "SUCCESS";
                                    break;
                                case UPNP_E_INVALID_HANDLE:
                                    return "INVALID_HANDLE";
                                    break;
                                case UPNP_E_INVALID_PARAM:
                                    return "INVALID_PARAM";
                                    break;
                                case UPNP_E_OUTOF_HANDLE:
                                    return "OUTOF_HANDLE";
                                    break;
                                case UPNP_E_OUTOF_CONTEXT:
                                    return "OUTOF_CONTEXT";
                                    break;
                                case UPNP_E_OUTOF_MEMORY:
                                    return "OUTOF_MEMOR";
                                    break;
                                case UPNP_E_INIT:
                                    return "INIT";
                                    break;
                                case UPNP_E_BUFFER_TOO_SMALL:
                                    return "BUFFER_TOO_SMALL";
                                    break;
                                case UPNP_E_INVALID_DESC:
                                    return "INVALID_DESC";
                                    break;
                                case UPNP_E_INVALID_URL:
                                    return "INVALID_URL";
                                    break;
                                case UPNP_E_INVALID_SID:
                                    return "INVALID_SID";
                                    break;
                                case UPNP_E_INVALID_DEVICE:
                                    return "INVALID_DEVICE";
                                    break;
                                case UPNP_E_INVALID_SERVICE:
                                    return "INVALID_SERVICE";
                                    break;
                                case UPNP_E_BAD_RESPONSE:
                                    return "BAD_RESPONSE";
                                    break;
                                case UPNP_E_BAD_REQUEST:
                                    return "BAD_REQUEST";
                                    break;
                                case UPNP_E_INVALID_ACTION:
                                    return "INVALID_ACTION";
                                    break;
                                case UPNP_E_FINISH:
                                    return "FINISH";
                                    break;
                                case UPNP_E_INIT_FAILED:
                                    return "INIT_FAILED";
                                    break;
                                case UPNP_E_BAD_HTTPMSG:
                                    return "BAD_HTTPMSG";
                                    break;
                                case UPNP_E_NETWORK_ERROR:
                                    return "NETWORK_ERROR";
                                    break;
                                case UPNP_E_SOCKET_WRITE:
                                    return "SOCKET_WRITE";
                                    break;
                                case UPNP_E_SOCKET_READ:
                                    return "SOCKET_READ";
                                    break;
                                case UPNP_E_SOCKET_BIND:
                                    return "SOCKET_BIND";
                                    break;
                                case UPNP_E_SOCKET_CONNECT:
                                    return "SOCKET_CONNECT";
                                    break;
                                case UPNP_E_OUTOF_SOCKET:
                                    return "OUTOF_SOCKET";
                                    break;
                                case UPNP_E_LISTEN:
                                    return "LISTEN";
                                    break;
                                case UPNP_E_EVENT_PROTOCOL:
                                    return "EVENT_PROTOCOL";
                                    break;
                                case UPNP_E_SUBSCRIBE_UNACCEPTED:
                                    return "SUBSCRIBE_UNACCEPTED";
                                    break;
                                case UPNP_E_UNSUBSCRIBE_UNACCEPTED:
                                    return "UNSUBSCRIBE_UNACCEPTED";
                                    break;
                                case UPNP_E_NOTIFY_UNACCEPTED:
                                    return "NOTIFY_UNACCEPTED";
                                    break;
                                case UPNP_E_INTERNAL_ERROR:
                                    return "INTERNAL_ERROR";
                                    break;
                                case UPNP_E_INVALID_ARGUMENT:
                                    return "INVALID_ARGUMENT";
                                    break;
                                case UPNP_E_OUTOF_BOUNDS:
                                    return "OUTOF_BOUNDS";
                                    break;
                                case UPNP_SOAP_E_INVALID_ACTION:
                                    return "INVALID_ACTION";
                                    break;
                                case UPNP_SOAP_E_INVALID_ARGS:
                                    return "INVALID_ARGS";
                                    break;
                                case UPNP_SOAP_E_OUT_OF_SYNC:
                                    return "OUT_OF_SYNC";
                                    break;
                                case UPNP_SOAP_E_INVALID_VAR:
                                    return "INVALID_VAR";
                                    break;
                                case UPNP_SOAP_E_ACTION_FAILED:
                                    return "ACTION_FAILED";
                                    break;
                                default:
                                    return "";
                            }
                        }

                    }
                }
            }
        }
    }
}
