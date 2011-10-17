#include <stdio.h>
#include <string>
#include "upnp/UPnPAction.h"
#include "upnp/UPnPArgument.h"
#include "upnp/UPnPEventListener.h"
#include "upnp/UPnPDevice.h"
#include "upnp/UPnPService.h"
#include "upnp/UPnPDeviceList.h"
#include "upnp/UPnPDeviceController.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
using namespace std;
using namespace br::ufscar::lince::ginga::hones::upnp;

HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones");

class ListenerTest : public UPnPEventListener {
public:
    void eventReceived(UPnPDevice* device, UPnPService* service, const char* sid, const char* variableName, const char* newVariableValue) {
        LoggerUtil_info(logger, "Entrando no metodo eventReceived!");
        LoggerUtil_debug(logger, "\n\nvariableName: " << variableName << " variableValue: " << newVariableValue << "\n");
        LoggerUtil_info(logger, "Saindo do metodo eventReceived!");
    }
};

int main(int argc, char* argv[]){
    int result = 0;

    try{

        LoggerUtil::configure();

        LoggerUtil_info(logger, "Entrando na aplicacao.");

	UPnPDeviceController* ctrl = UPnPDeviceController::getInstance();

	LoggerUtil_info(logger, "UPnPDeviceController instanciado: " << (ctrl != NULL));

        ctrl->start();
        
        ListenerTest listener;
        ctrl->addUPnPEventListener(&listener);

        int ch;

    	do
	{
		ch = getchar();
		ch = toupper( ch );
		switch (ch) {
                case 'P':
                {
                    UPnPDeviceList* list = ctrl->getUPnPDeviceList();
                    int size = list->size();
                    LoggerUtil_debug(logger, "Tamanho da lista de dispositivos: " << size);
                    for(int i=0; i < size; i++){
                        LoggerUtil_debug(logger, "Dispositivo: " << list->getUPnPDevice(i)->getFriendlyName() << " tipo: " << list->getUPnPDevice(i)->getDeviceType() << " ip: " << list->getUPnPDevice(i)->getLocation());
                    }
                    break;
                }
		case 'Q':
                    break;
                case 'S':
                {
                    UPnPDevice* dev = ctrl->getUPnPDevice("urn:schemas-upnp-org:device:DimmableLight:1");
                    if(dev != NULL){
                        UPnPService* serv = dev->getUPnPService("urn:schemas-upnp-org:service:SwitchPower:1");
                        if(ctrl->subscribe(serv) == true){
                            LoggerUtil_debug(logger, "\n\nsubscribed\n");
                        } else {
                            LoggerUtil_debug(logger, "\n\nnot subscribed\n");
                        }
                    }
                    break;
                }
                case 'T':
                {
                    UPnPDevice* dev = ctrl->getUPnPDevice("urn:schemas-upnp-org:device:DimmableLight:1");
                    if(dev != NULL){
                        UPnPService* serv = dev->getUPnPService("urn:schemas-upnp-org:service:SwitchPower:1");
                        UPnPAction* action = serv->getUPnPAction("GetTarget");
                        if(action->sendUPnPAction() == true){
                            LoggerUtil_debug(logger, "\n\nsendUPnPAction() == true\n");
                            string ret = action->getUPnPArgument("RetTargetValue")->getValue();
                            LoggerUtil_debug(logger, "\n\nreturn: " << ret.c_str());
                            UPnPAction* action2 = serv->getUPnPAction("SetTarget");
                            action2->getUPnPArgument("NewTargetValue")->setValue(ret.compare("1") == 0 ? 0 : 1);
                            action2->sendUPnPAction();
                        }
                    }
                    break;
                }
                case 'U':
                {
                    UPnPDevice* dev = ctrl->getUPnPDevice("urn:schemas-upnp-org:device:DimmableLight:1");
                    if(dev != NULL){
                        UPnPService* serv = dev->getUPnPService("urn:schemas-upnp-org:service:SwitchPower:1");
                        if(ctrl->unsubscribe(serv) == true){
                            LoggerUtil_debug(logger, "\n\nunsubscribed\n");
                        } else {
                            LoggerUtil_debug(logger, "\n\nnot unsubscribed\n");
                        }
                    }
                    break;
                }
                case 'V':
                {
                    UPnPDevice* dev = ctrl->getUPnPDevice("urn:schemas-upnp-org:device:DimmableLight:1");
                    if(dev != NULL){
                        UPnPStateVariable* var = dev->getUPnPStateVariable("Status");
                        LoggerUtil_debug(logger, "\n\nvarName: " << var->getName() << " varValue: " << var->getValue() << "\n");
                    }
                    break;
                }
                default:
                    LoggerUtil_info(logger, "\n'p' : Print\n'q' : Quit\n's' : Subscribe\n't' : Test action\n'u' : Unsubscribe\n'v' : Variable value");
		}
	} while( ch != 'Q');

	ctrl->stop();

	LoggerUtil_info(logger, "Saindo da aplicacao.");

    } catch(...) {
        result = -1;
    }

    return result;
}
