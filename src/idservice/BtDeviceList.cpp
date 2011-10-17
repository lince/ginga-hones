#include "honesconfig.h"

#include <unistd.h>
#include <string>
#include "idservice/BtDeviceList.h"

using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace idservice {

                        BtDeviceList::BtDeviceList() {
                        }

                        BtDeviceList::~BtDeviceList(){
                            clear();
                        }

                        void BtDeviceList::clear() {
                            int size = Vector::size();
                            for (int i = 0; i < size; i++) {
                                BtDevice* value = getBtDevice(i);
                                if (value != NULL) {
                                    delete value;
                                    value = NULL;
                                }
                            }
                            Vector::clear();
                        }

                        BtDevice* BtDeviceList::getBtDevice(int i) {
                            return (BtDevice*) Vector::get(i);
                        }

                        BtDevice* BtDeviceList::getBtDevice(const char* name) {
                            BtDevice* device = getBtDeviceByUDN(name);

                            if ((device == NULL) && (name != NULL)) {
                                string devName = name;
                                int size = Vector::size();
                                for (int i = 0; i < size; i++) {
                                    device = getBtDevice(i);
                                    if (devName.compare(device->getFriendlyName()) == 0)
                                        return device;
                                }
                            }

                            return device;
                        }

                        BtDevice* BtDeviceList::getBtDeviceByUDN(const char* udn) {
                            if (udn != NULL) {
                                string devUDN = udn;
                                int size = Vector::size();
                                for (int i = 0; i < size; i++) {
                                    BtDevice* device = getBtDevice(i);
                                    if (devUDN.compare(device->getUDN()) == 0)
                                        return device;
                                }
                            }

                            return NULL;
                        }

                    }
                }
            }
        }
    }
}
