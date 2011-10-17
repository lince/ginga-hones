#include <stdio.h>
#include "HelloWorld.h"
#include "util/Utils.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace helloworld {

                        const char* HelloWorld::UUID_PATH = ".helloworld.uuid";
                        const char* HelloWorld::DEVICE_TYPE = "urn:schemas-upnp-org:device:HelloWorld:1";
                        const char* HelloWorld::HELLOWORLD_DEVICE_DESCRIPTION =
                                "<?xml version=\"1.0\" ?>\n"
                                "<root xmlns=\"urn:schemas-upnp-org:device-1-0\">\n"
                                " 	<specVersion>\n"
                                " 		<major>1</major>\n"
                                " 		<minor>0</minor>\n"
                                " 	</specVersion>\n"
                                " 	<device>\n"
                                " 		<deviceType>urn:schemas-upnp-org:device:HelloWorld:1</deviceType>\n"
                                " 		<friendlyName>Hello World</friendlyName>\n"
                                " 		<manufacturer>Lince</manufacturer>\n"
                                " 		<manufacturerURL>http://lince.dc.ufscar.br</manufacturerURL>\n"
                                " 		<modelDescription>Exemplo de serviço UPnP</modelDescription>\n"
                                " 		<modelName>HelloWorldApp</modelName>\n"
                                " 		<modelNumber>1.0</modelNumber>\n"
                                " 		<modelURL>http://lince.dc.ufscar.br</modelURL>\n"
                                " 		<serviceList>\n"
                                " 			<service>\n"
                                " 				<serviceType>urn:schemas-upnp-org:service:HelloWorldService:1</serviceType>\n"
                                " 				<serviceId>urn:schemas-upnp-org:serviceId:HelloWorldService:1</serviceId>\n"
                                " 				<SCPDURL>/service/helloworldservice/description.xml</SCPDURL>\n"
                                " 				<controlURL>/service/helloworldservice/control</controlURL>\n"
                                " 				<eventSubURL>/service/helloworldservice/eventSub</eventSubURL>\n"
                                " 			</service>\n"
                                " 		</serviceList>\n"
                                "	</device>\n"
                                "</root>\n";

                        HelloWorld::HelloWorld() : UPnPDevice() {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.helloworld.helloworld");
                            LoggerUtil_info(logger, "Entrando no construtor HelloWorld()!");

                            restoreUUID(string(Utils::getHomeDir()).append("/").append(UUID_PATH).c_str());

                            if (loadDescription(HELLOWORLD_DEVICE_DESCRIPTION) == false)
                                throw "Invalid description!";

                            service = new HelloWorldService(this);
                            LoggerUtil_info(logger, "Saindo do construtor HelloWorld()!");
                        }

                        HelloWorld::~HelloWorld() {
                            stop();
                            delete service;
                        }

                        HelloWorldService* HelloWorld::getHelloWorldService() {
                            return service;
                        }

                        bool HelloWorld::start() {
                            bool result = UPnPDevice::start();
                            // tarefas adicionais
                            return result;
                        }

                        bool HelloWorld::stop() {
                            return UPnPDevice::stop();
                        }

                    }
                }
            }
        }
    }
}

using namespace br::ufscar::lince::ginga::hones::helloworld;

int main(int argc, char* argv[]){

    HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.helloworld");

    LoggerUtil::configure();

    LoggerUtil_info(logger, "Entrando na aplicacao.");

    HelloWorld* app = new HelloWorld();
    app->start();

    int ch;

    do {
        ch = getchar();
        ch = toupper(ch);
        switch (ch) {
            case 'Q':
                break;
            default:
                LoggerUtil_info(logger, "'q' : Quit\n");
        }
    } while (ch != 'Q');

    delete app;

    LoggerUtil_info(logger, "Saindo da aplicacao.");
    
    return 0;
}
