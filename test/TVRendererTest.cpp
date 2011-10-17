#include <stdio.h>
#include "mediarenderer/TVRenderer.h"
#include "ui/GraphicsHandler.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
using namespace br::ufscar::lince::ginga::hones::ui;
using namespace std;
using namespace br::ufscar::lince::ginga::hones::mediarenderer;

HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones");


int main(int argc, char* argv[]){
    
    int result = 0;
    
    try{

        LoggerUtil::configure();
        
        LoggerUtil_info(logger, "Entrando na aplicação.");

	int ch;

        GraphicsHandler::initialize();

	TVRenderer *renderer = new TVRenderer();

	LoggerUtil_info(logger, "TVRenderer instanciado: " << (renderer != NULL));

	renderer->start();

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

	delete renderer;
        
        GraphicsHandler::release();

	LoggerUtil_info(logger, "Saindo da aplicação.");
    
    } catch(...) {
        result = -1;
    }
    
    return result;
}
