#ifndef _HELLOWORLD_H
#define	_HELLOWORLD_H

#include <string>
#include "UPnPDevice.h"
#include "HelloWorldService.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
using namespace std;
using namespace br::ufscar::lince::ginga::hones::upnp;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace helloworld {

                        /**
                         * Esta classe representa um dispositivo, que recebeu o nome de HelloWorld,
                         * mas poderia ser TV, Computador, PVR, ou outra coisa.
                         * Cada dispositivo possui sua descrição e os serviços que disponibiliza.
                         * Neste exemplo, o dispositivo possui apenas um serviço (HelloWorldService) que é
                         * tratado pela classe de mesmo nome.
                         */

                        class HelloWorld : public UPnPDevice {
                        private:
                            /** instância da classe que trata o serviço HelloWorldService */
                            HelloWorldService* service;
                            /** descrição (XML) do dispositivo e quais serviços possui */
                            static const char* HELLOWORLD_DEVICE_DESCRIPTION;
                            /** local para armazenamento do UUID, para evitar que um novo seja gerado a cada execução da aplicação */
                            static const char* UUID_PATH;
                            /** string com a identificação do tipo do dispositivo */
                            static const char* DEVICE_TYPE;
                            HLoggerPtr logger;

                        public:

                            /**
                             * Construtor.
                             */
                            HelloWorld();

                            /**
                             * Destrutor.
                             */
                            ~HelloWorld();

                            /**
                             * Retorna a instância da classe que manipula o serviço HelloWorld.
                             * @return instância da classe que manipula o serviço HelloWorld.
                             */
                            HelloWorldService* getHelloWorldService();

                            /**
                             * Inicia os serviços.
                             * @return true, caso obtena sucesso na operação, ou false, caso contrário.
                             */
                            bool start();

                            /**
                             * Pára os serviços.
                             * @return true, caso obtena sucesso na operação, ou false, caso contrário.
                             */
                            bool stop();

                        };

                    }
                }
            }
        }
    }
}

#endif	/* _HELLOWORLD_H */

