#include <stdio.h>
#include "mediaserver/TVMediaServer.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
using namespace std;
using namespace br::ufscar::lince::ginga::hones::mediaserver;

HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones");

int main(int argc, char* argv[]){
    int result = 0;

    try{

        LoggerUtil::configure();

        LoggerUtil_info(logger, "Entrando na aplicacao.");

	TVMediaServer* server = new TVMediaServer();

	LoggerUtil_info(logger, "TVMediaServer instanciado: " << (server != NULL));

        server->getContentDirectory()->addContentDirectory("/misc/media");

        server->start();

        int ch;

    	do
	{
		ch = getchar();
		ch = toupper( ch );
		switch (ch) {
		case 'Q':
			break;
		default :
                    LoggerUtil_info(logger, "'q' : Quit");
		}
	} while( ch != 'Q');
        
	server->stop();

	LoggerUtil_info(logger, "Saindo da aplicacao.");

    } catch(...) {
        result = -1;
    }

    return result;
}
