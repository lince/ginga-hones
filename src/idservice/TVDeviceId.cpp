#include "honesconfig.h"

#include "idservice/TVDeviceId.h"
#include "util/Utils.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;

using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace idservice {

                        const char* TVDeviceId::TVDeviceId_DEVICE_DESCRIPTION = "<?xml version=\"1.0\" ?>\n"
                                "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\n"
                                " 	<specVersion>\n"
                                " 		<major>1</major>\n"
                                " 		<minor>0</minor>\n"
                                " 	</specVersion>\n"
                                " 	<device>\n"
                                " 		<deviceType>urn:schemas-upnp-org:device:tvdeviceid:1</deviceType>\n"
                                " 		<friendlyName>Lince TV</friendlyName>\n"
                                " 		<manufacturer>Lince</manufacturer>\n"
                                " 		<manufacturerURL>http://lince.dc.ufscar.br</manufacturerURL>\n"
                                " 		<modelDescription>UPnP TV Id Service</modelDescription>\n"
                                " 		<modelName>TV Id Service</modelName>\n"
                                " 		<modelNumber>1.0</modelNumber>\n"
                                " 		<modelURL>http://lince.dc.ufscar.br</modelURL>\n"
                                " 		<serviceList>\n"
                                " 			<service>\n"
                                " 				<serviceType>urn:schemas-upnp-org:service:idservice:1</serviceType>\n"
                                " 				<serviceId>urn:schemas-upnp-org:serviceId:idservice:1</serviceId>\n"
                                " 				<SCPDURL>/service/idservice/description.xml</SCPDURL>\n"
                                " 				<controlURL>/service/idservice/control</controlURL>\n"
                                " 				<eventSubURL>/service/idservice/eventSub</eventSubURL>\n"
                                " 			</service>\n"
                                " 		</serviceList>\n"
                                "	</device>\n"
                                "</root>\n";

                        const char* TVDeviceId::TVDeviceId_SERVICE_DESCRIPTION = 
                                "<?xml version=\"1.0\"?>\n"
                                "<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\" >\n"
                                " 	<specVersion>\n"
                                " 		<major>1</major>\n"
                                " 		<minor>0</minor>\n"
                                " 	</specVersion>\n"
                                " 	<actionList>\n"
                                " 		<action>\n"
                                " 			<name>GetDevicesIds</name>\n"
                                " 			<argumentList>\n"
                                " 				<argument>\n"
                                " 					<name>Result</name>\n"
                                " 					<relatedStateVariable>DevicesIds</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 			</argumentList>\n"
                                " 		</action>\n"
                                " 	</actionList>\n"
                                " 	<serviceStateTable>\n"
                                " 		<stateVariable sendEvents=\"yes\">\n"
                                " 			<name>DevicesIds</name>\n"
                                " 			<dataType>string</dataType>\n"
                                " 		</stateVariable>\n"
                                " 	</serviceStateTable>\n"
                                "</scpd>\n";

                        TVDeviceId::TVDeviceId() : UPnPDevice() {
                            restoreUUID(string(Utils::getHomeDir()).append("/").append(".tvdeviceid.uuid").c_str());

                            if (loadDescription(TVDeviceId_DEVICE_DESCRIPTION) == false)
                                throw "Invalid description!";

                            if (getService()->loadSCPD(TVDeviceId_SERVICE_DESCRIPTION) == false)
                                throw "Invalid description!";

                            setUPnPActionListener(this);
                            setUPnPQueryListener(this);

                            upnpControl = UPnPDeviceController::getInstance();
                            upnpControl->addUPnPDeviceListener(this);
                            upnpControl->addUPnPEventListener(this);
                            btControl = BtDeviceController::getInstance();
                            btControl->addBtDeviceListener(this);

                            upnpControl->start();
                            btControl->start();
                        }

                        TVDeviceId::~TVDeviceId() {
                            stop();
                        }

                        UPnPService* TVDeviceId::getService() {
                            return getUPnPService("urn:schemas-upnp-org:service:idservice:1");
                        }

                        bool TVDeviceId::addBtDeviceListener(BtDeviceListener* deviceListener) {
                            btControl->addBtDeviceListener(deviceListener);
                        }

                        bool TVDeviceId::removeBtDeviceListener(BtDeviceListener* deviceListener) {
                            btControl->removeBtDeviceListener(deviceListener);
                        }

                        bool TVDeviceId::addUPnPDeviceListener(UPnPDeviceListener* deviceListener) {
                            upnpControl->addUPnPDeviceListener(deviceListener);
                        }

                        bool TVDeviceId::removeUPnPDeviceListener(UPnPDeviceListener* deviceListener) {
                            upnpControl->removeUPnPDeviceListener(deviceListener);
                        }

                        bool TVDeviceId::actionRequest(UPnPAction *action) {
                            const char* actionName = action->getName();

                            if (strcmp(actionName, "GetDevicesIds") == 0) {

                                action->getUPnPArgument("Result")->setValue(getService()->getUPnPStateVariable("DevicesIds")->getValue());

                                return true;
                            }

                            return false;
                        }

                        bool TVDeviceId::variableRequest(UPnPStateVariable *stateVar) {
                            UPnPStateVariable* serviceStateVar = getService()->getUPnPStateVariable(stateVar->getName());
                            if (serviceStateVar == NULL)
                                return false;

                            stateVar->setValue(serviceStateVar->getValue());

                            return true;
                        }

                        void TVDeviceId::deviceAdded(BtDevice* device) {
                            
                            UPnPStateVariable* variable = getService()->getUPnPStateVariable("DevicesIds");

                            string devices = variable->getValue();
                            devices.append("name=");
                            devices.append(device->getFriendlyName());
                            devices.append(" uuid=");
                            devices.append(device->getUDN());
                            devices.append(" network=");
                            devices.append("bluetooth");
                            devices.append(" host-uuid=");
                            devices.append(this->getUDN());
                            devices.append("\n");

                            variable->setValue(devices.c_str());
                            Utils::updateLastChangeStateVariable(getService(), variable);
                        }

                        void TVDeviceId::deviceRemoved(BtDevice* device) {
                            UPnPStateVariable* variable = getService()->getUPnPStateVariable("DevicesIds");

                            string devices = variable->getValue();

                            int line = devices.find(device->getUDN());
                            if (line != string::npos) {
                                string first = devices.substr(0, line);
                                string last = devices.substr(line);

                                int first_pos = first.find_last_of("\n");
                                if ((first_pos == string::npos) || (first_pos == 0)) {
                                    first = "";
                                } else {
                                    first = first.substr(0, first_pos);
                                }

                                int last_pos = last.find("\n");
                                if (last_pos == string::npos) {
                                    last = "";
                                } else {
                                    last = last.substr(last_pos);
                                }

                                devices = first + last;

                                variable->setValue(devices.c_str());
                                Utils::updateLastChangeStateVariable(getService(), variable);
                            }
                        }

                        void TVDeviceId::deviceAdded(UPnPDevice* device) {
                            if((device->isUPnPDevice("urn:schemas-upnp-org:device:tvdeviceid:1") == true) && (strcmp(device->getUDN(),this->getUDN()) != 0)){
                                UPnPService* serv = device->getUPnPService("urn:schemas-upnp-org:service:idservice:1");
                                upnpControl->subscribe(serv);
                            }

                            UPnPStateVariable* variable = getService()->getUPnPStateVariable("DevicesIds");

                            string devices = variable->getValue();
                            devices.append("name=");
                            devices.append(device->getFriendlyName());
                            devices.append(" uuid=");
                            devices.append(device->getUDN());
                            devices.append(" network=");
                            devices.append("upnp");
                            devices.append(" host-uuid=");
                            devices.append(this->getUDN());
                            devices.append("\n");

                            variable->setValue(devices.c_str());
                            Utils::updateLastChangeStateVariable(getService(), variable);
                        }

                        void TVDeviceId::deviceRemoved(UPnPDevice* device) {
                            UPnPStateVariable* variable = getService()->getUPnPStateVariable("DevicesIds");

                            string devices = variable->getValue();

                            int line = devices.find(device->getUDN());
                            if (line != string::npos) {
                                string first = devices.substr(0, line);
                                string last = devices.substr(line);

                                int first_pos = first.find_last_of("\n");
                                if ((first_pos == string::npos) || (first_pos == 0)) {
                                    first = "";
                                } else {
                                    first = first.substr(0, first_pos);
                                }

                                int last_pos = last.find("\n");
                                if (last_pos == string::npos) {
                                    last = "";
                                } else {
                                    last = last.substr(last_pos);
                                }

                                devices = first + last;

                                variable->setValue(devices.c_str());
                                Utils::updateLastChangeStateVariable(getService(), variable);
                            }
                        }

                        void TVDeviceId::eventReceived(UPnPDevice* device, UPnPService* service, const char* sid, const char* variableName, const char* newVariableValue){
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.idservice.tvdeviceid");
                            LoggerUtil_info(logger, "Entrando no metodo eventReceived!");
                            LoggerUtil_debug(logger, "\n\nvariableName: " << variableName << " variableValue: " << newVariableValue << "\n");
                            LoggerUtil_info(logger, "Saindo do metodo eventReceived!");
                        }

                    }
                }
            }
        }
    }
}
