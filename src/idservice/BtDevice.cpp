#include "honesconfig.h"

#include "idservice/BtDevice.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace idservice {

                        BtDevice::BtDevice() {
                        }

                        BtDevice::BtDevice(BtDevice* device) {
                            setFriendlyName(device->getFriendlyName());
                            setUDN(device->getUDN());
                        }

                        BtDevice::~BtDevice() {
                        }

                        bool BtDevice::isBtDevice(const char* name) {
                            return ((udn.compare(name) == 0) || (friendlyName.compare(name) == 0));
                        }

                        void BtDevice::setFriendlyName(const char* name) {
                            friendlyName = name;
                        }

                        const char* BtDevice::getFriendlyName() {
                            return friendlyName.c_str();
                        }

                        void BtDevice::setUDN(const char* udn) {
                            this->udn = udn;
                        }

                        const char* BtDevice::getUDN() {
                            return udn.c_str();
                        }

                    }
                }
            }
        }
    }
}
