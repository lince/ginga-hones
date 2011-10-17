#include <stdio.h>
#include "mediacenter/TVMediaCenter.h"
#include "mediarenderer/TVRenderer.h"
#include "ui/GraphicsHandler.h"
#include <ginga/linceutil/LoggerUtil.h>
#include "util/Utils.h"
using namespace br::ufscar::lince::util;
using namespace std;
using namespace br::ufscar::lince::ginga::hones::ui;
using namespace br::ufscar::lince::ginga::hones::util;
using namespace br::ufscar::lince::ginga::hones::mediacenter;
using namespace br::ufscar::lince::ginga::hones::mediarenderer;

HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.mediacenter");

int main(int argc, char* argv[]){
    int result = 0;

    try{

        LoggerUtil::configure();

        LoggerUtil_info(logger, "Entrando na aplicacao.");

        GraphicsHandler::initialize();

	TVMediaCenter* mediaCenter = new TVMediaCenter();

	LoggerUtil_info(logger, "TV Media Center instanciado!");

        TVRenderer* renderer = new TVRenderer();

	LoggerUtil_info(logger, "TVRenderer instanciado!");

	renderer->start();

        mediaCenter->start();

        int ch;

    	do
	{
		ch = getchar();
		ch = toupper( ch );
		switch (ch) {
                case 'S':
                {
                    mediaCenter->stop();
                    break;
                }
		case 'Q':
                    break;
                default:
                    LoggerUtil_info(logger, "'q' : Quit\n");
		}
	} while( ch != 'Q');

        delete mediaCenter;
        delete renderer;

        GraphicsHandler::release();

	LoggerUtil_info(logger, "Saindo da aplicacao.");

    } catch(...) {
        result = -1;
    }

    return result;
}
