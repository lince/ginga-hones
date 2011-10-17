#include "honesconfig.h"

#include <unistd.h>
#ifdef HAVE_BLUETOOTH
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#endif
#include "idservice/BtDeviceController.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace idservice {

                        BtDeviceController* BtDeviceController::instance = NULL;

                        BtDeviceController::BtDeviceController(){
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.idservice.btdevicecontroller");
                            LoggerUtil_info(logger, "Entrando no construtor BtDeviceController()!");

                            setRunning(false);
                        }

                        BtDeviceController::~BtDeviceController(){
                        }

                        BtDeviceController* BtDeviceController::getInstance(){
                            if(instance == NULL){
                                instance = new BtDeviceController();
                            }
                            return instance;
                        }

                        void BtDeviceController::deleteInstance(){
                            if(instance != NULL){
                                delete instance;
                                instance = NULL;
                            }
                        }

                        void BtDeviceController::setRunning(bool running){
                            this->running = running;
                        }

                        bool BtDeviceController::isRunning(){
                            return running;
                        }

                        void* BtDeviceController::run(void* param){
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.idservice.btdevicecontroller");
                            LoggerUtil_info(logger, "Entrando no metodo BtDeviceController::run(param)");
#ifdef HAVE_BLUETOOTH
                            BtDeviceController* devControl = (BtDeviceController*) param;

                            int max_rsp = 255;
                            int len = 6;
                            char addr[19] = {0};
                            char name[248] = {0};

                            LoggerUtil_debug(logger, "Inicializando o Bluetooth...");

                            int dev_id = hci_get_route(NULL);
                            int sock = hci_open_dev(dev_id);
                            if ((dev_id < 0) || (sock < 0)){
                                LoggerUtil_debug(logger, "Bluetooth indisponível!");
                                return NULL;
                            }

                            LoggerUtil_debug(logger, "Bluetooth ligado!");

                            inquiry_info* ii = (inquiry_info*) malloc(max_rsp * sizeof(inquiry_info));
                            BtDeviceList* currentList = devControl->getBtDeviceList();
                            BtDeviceList newList;
                            bool removeAction = false;

                            while (devControl->isRunning() == true) {
                                pthread_testcancel();

                                int num_rsp = hci_inquiry(dev_id, len, max_rsp, NULL, &ii, IREQ_CACHE_FLUSH);
                                
                                for (int i=0; i < num_rsp; i++) {
                                    ba2str(&(ii + i)->bdaddr, addr);
                                    memset(name, 0, sizeof (name));
                                    if (hci_read_remote_name(sock, &(ii + i)->bdaddr, sizeof (name), name, 0) < 0)
                                        strcpy(name, "[desconhecido]");

                                    if (newList.getBtDeviceByUDN(addr) == NULL){
                                        BtDevice* device = new BtDevice();
                                        device->setFriendlyName(name);
                                        device->setUDN(addr);
                                        newList.add(device);
                                    }
                                }

                                int listSize = newList.size();
                                for(int i = 0; i < listSize; i++){
                                    BtDevice* device = newList.getBtDevice(i);
                                    if (currentList->getBtDeviceByUDN(device->getUDN()) == NULL){
                                        devControl->addBtDevice(new BtDevice(device));
                                    }
                                }

                                if (removeAction == true) {
                                    listSize = currentList->size();
                                    int index = 0;
                                    while (index < listSize) {
                                        BtDevice* device = currentList->getBtDevice(index);
                                        if (newList.getBtDeviceByUDN(device->getUDN()) == NULL) {
                                            devControl->removeBtDevice(device);
                                            delete device;
                                            listSize = currentList->size();
                                            index = -1;
                                        }
                                        index++;
                                    }

                                    newList.clear();
                                    removeAction = false;
                                } else {
                                    removeAction = true;
                                }
                            }

                            newList.clear();
                            free(ii);
                            close(sock);
#endif

                            return NULL;
                        }

                        bool BtDeviceController::start(){
                            stop();

                            setRunning(true);

                            if (pthread_create(&thread, NULL, run, this) != 0) {
                                setRunning(false);
                            }
                            pthread_detach(thread);

                            return isRunning();
                        }

                        bool BtDeviceController::stop(){
                            if(isRunning() == true){
                                pthread_cancel(thread);
                                pthread_detach(thread);
                                setRunning(false);
                                deviceList.clear();
                                return true;
                            } else {
                                return false;
                            }
                        }

                        void BtDeviceController::addBtDevice(BtDevice* device){
                            if (device != NULL) {
                                deviceList.add(device);
                                LoggerUtil_debug(logger, "Dispositivo adicionado: " << device->getFriendlyName());

                                int deviceListenersSize = deviceListeners.size();
                                for (int i = 0; i < deviceListenersSize; i++) {
                                    BtDeviceListener* dListener = (BtDeviceListener*) deviceListeners.get(i);
                                    dListener->deviceAdded(device);
                                }
                            }
                        }

                        void BtDeviceController::removeBtDevice(BtDevice* device){
                            if (device != NULL) {
                                int deviceListenersSize = deviceListeners.size();
                                for (int i = 0; i < deviceListenersSize; i++) {
                                    BtDeviceListener* dListener = (BtDeviceListener*) deviceListeners.get(i);
                                    dListener->deviceRemoved(device);
                                }

                                deviceList.remove(device);
                                LoggerUtil_debug(logger, "Dispositivo removido: " << device->getFriendlyName());
                            }
                        }

                        void BtDeviceController::removeBtDevice(const char* name){
                            removeBtDevice(deviceList.getBtDevice(name));
                        }

                        BtDeviceList* BtDeviceController::getBtDeviceList(){
                            return &deviceList;
                        }

                        BtDevice* BtDeviceController::getBtDevice(const char* name){
                            return deviceList.getBtDevice(name);
                        }

                        bool BtDeviceController::hasBtDevice(const char* name){
                            return (getBtDevice(name) != NULL);
                        }

                        bool BtDeviceController::addBtDeviceListener(BtDeviceListener* deviceListener){
                            return deviceListeners.add(deviceListener);
                        }

                        bool BtDeviceController::removeBtDeviceListener(BtDeviceListener* deviceListener){
                            return deviceListeners.remove(deviceListener);
                        }

                    }
                }
            }
        }
    }
}

