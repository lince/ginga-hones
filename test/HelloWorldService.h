#ifndef _HELLOWORLDSERVICE_H
#define	_HELLOWORLDSERVICE_H

#include <string>
#include "UPnPDevice.h"
#include "UPnPActionListener.h"
#include "UPnPQueryListener.h"
#include "UPnPService.h"
#include "UPnPAction.h"
#include "UPnPArgument.h"
#include "UPnPStateVariable.h"
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

                        class HelloWorld;

                        /**
                         * Esta classe trata o serviço HelloWorldService.
                         * Recebe as chamadas de métodos (ações), faz algum tratamento e devolve o
                         * resultado para o dispositivo que o invocou.
                         */

                        class HelloWorldService : public UPnPActionListener, public UPnPQueryListener {
                        private:
                            /** string com a identificação do tipo do serviço */
                            static const char* HELLOWORLD_SERVICE_TYPE;
                            /** string (XML) com a descrição das funcionalidades do serviço */
                            static const char* HELLOWORLD_SERVICE_DESCRIPTION;
                            /** instância da classe que representa o dispositivo */
                            HelloWorld* device;
                            HLoggerPtr logger;

                        public:

                            /**
                             * Construtor.
                             */
                            HelloWorldService(HelloWorld* instance);

                            /**
                             * Destrutor.
                             */
                            ~HelloWorldService();

                            /**
                             * Inicializa as variáveis de estado necessárias ao funcionamento do serviço.
                             */
                            void initializeUPnPStateVariables();

                            /**
                             * Retorna a instância do serviço HelloWorld.
                             * @return serviço HelloWorld.
                             */
                            UPnPService* getService();

                            /**
                             * Recebe solicitações de ações a serem executadas.
                             * @param action ação a ser executada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionRequest(UPnPAction *action);

                            /**
                             * Envia o estado atual de uma variável do serviço.
                             * @param stateVar variável solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool variableRequest(UPnPStateVariable *stateVar);

                            /**
                             * Recebe uma mensagem (texto) de um dispositivo remoto.
                             * @param action ação a ser executada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionSetMessage(UPnPAction *action);

                            /**
                             * Envia a última/atual mensagem (texto) recebida para um dispositivo remoto.
                             * @param action ação a ser executada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetMessage(UPnPAction *action);

                            /**
                             * Recebe um arquivo (binário) de um dispositivo remoto.
                             * O problema de se transferir dados binários está relacionado ao tempo de transmissão
                             * e ao tamanho do arquivo. O UPnP define o tempo máximo de 30 segundos para retorno de uma
                             * chamada de método/ação, desta forma, se o arquivo não puder ser transferido pela banda
                             * disponível nesse tempo, ocorrerá erro.
                             * A melhor forma seria transferir o caminho (URI) do arquivo e internamente, em outra thread,
                             * baixar o arquivo, ou transferir o IP e a porta e fazer a transferência do arquivo via socket.
                             * @param action ação a ser executada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionSetFile(UPnPAction *action);

                        };

                    }
                }
            }
        }
    }
}

#endif	/* _HELLOWORLDSERVICE_H */

