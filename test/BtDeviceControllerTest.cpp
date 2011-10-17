#include <stdio.h>
#include <string>
#include "idservice/BtDevice.h"
#include "idservice/BtDeviceList.h"
#include "idservice/BtDeviceController.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
using namespace std;
using namespace br::ufscar::lince::ginga::hones::idservice;

HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones");

int main(int argc, char* argv[]){
    int result = 0;

    try{

        LoggerUtil::configure();

        LoggerUtil_info(logger, "Entrando na aplicacao.");

	BtDeviceController* ctrl = BtDeviceController::getInstance();

	LoggerUtil_info(logger, "BtDeviceController instanciado: " << (ctrl != NULL));

        ctrl->start();

        int ch;

    	do
	{
		ch = getchar();
		ch = toupper( ch );
		switch (ch) {
                case 'P':
                {
                    BtDeviceList* list = ctrl->getBtDeviceList();
                    int size = list->size();
                    LoggerUtil_debug(logger, "Tamanho da lista de dispositivos: " << size);
                    for(int i=0; i < size; i++){
                        LoggerUtil_debug(logger, "Dispositivo: " << list->getBtDevice(i)->getFriendlyName() << " UDN: " << list->getBtDevice(i)->getUDN());
                    }
                    break;
                }
		case 'Q':
                    break;
                default:
                    LoggerUtil_info(logger, "\n'p' : Print\n'q' : Quit\n");
		}
	} while( ch != 'Q');

	ctrl->stop();

	LoggerUtil_info(logger, "Saindo da aplicacao.");

    } catch(...) {
        result = -1;
    }

    return result;
}
