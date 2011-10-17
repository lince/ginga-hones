/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware
Ginga (Ginga-NCL).

Copyright (C) 2009 UFSCar/Lince, Todos os Direitos Reservados.

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

Para maiores informacoes:
lince@dc.ufscar.br
http://www.ncl.org.br
http://www.ginga.org.br
http://lince.dc.ufscar.br
******************************************************************************
This file is part of the declarative environment of middleware Ginga (Ginga-NCL)

Copyright (C) 2009 UFSCar/Lince, Todos os Direitos Reservados.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
lince@dc.ufscar.br
http://www.ncl.org.br
http://www.ginga.org.br
http://lince.dc.ufscar.br
*******************************************************************************/

#ifndef _UPNPDEVICECONTROLLER_H
#define	_UPNPDEVICECONTROLLER_H

#include <pthread.h>
#include <string>
#include "UPnPDevice.h"
#include "UPnPService.h"
#include "UPnPActionResponse.h"
#include "UPnPQueryResponse.h"
#include "UPnPAction.h"
#include "UPnPDeviceList.h"
#include "UPnPDeviceListener.h"
#include "UPnPEventListener.h"
#include "Vector.h"
#include "Utils.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
using namespace br::ufscar::lince::ginga::hones::util;
using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        class UPnPStateVariable;
                        
                        class UPnPDeviceController {
                        private:
                            static UPnPDeviceController* instance;
                            bool running;
                            UpnpClient_HandlePtr client_handle;
                            UPnPDeviceList deviceList;
                            Vector deviceListeners;
                            Vector eventListeners;
                            int searchTime;
                            int subscriptionTimeOut;
                            pthread_mutex_t mutex;
                            HLoggerPtr logger;

                        public:

                            /**
                             * Retorna a instância do controlador de dispositivos.
                             * @return instância do controlador de dispositivos.
                             */
                            static UPnPDeviceController* getInstance();

                            /**
                             * Destrói a instância do controlador de dispositivos.
                             */
                            static void deleteInstance();

                            /**
                             * Verifica se o controlador de dispositivos está ativo.
                             * @return true, caso o controlador de dispositivos esteja ativo, ou false, caso contrário.
                             */
                            bool isRunning();

                            /**
                             * Ativa o controlador de dispositivos.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool start();

                            /**
                             * Pára o controlador de dispositivos.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool stop();

                            /**
                             * Retorna o identificador/manipulador do controlador de dispositivos.
                             * @return identificador do controlador de dispositivos.
                             */
                            UpnpClient_HandlePtr getUPnPClientHandle();

                            /**
                             * Adiciona um dispositivo a partir da URL do documento que o descreve.
                             * @param url URL do documento que descreve o dispositivo.
                             */
                            void addUPnPDevice(const char* url);

                            /**
                             * Adiciona um dispositivo a lista dos dispositivos controlados.
                             * @param device dispositivo.
                             */
                            void addUPnPDevice(UPnPDevice* device);

                            /**
                             * Remove um dispositivo da lista dos dispositivos controlados.
                             * @param device dispositivo.
                             */
                            void removeUPnPDevice(UPnPDevice* device);

                            /**
                             * Remove um dispositivo da lista dos dispositivos controlados pelo identificador, nome amigável ou tipo do dispositivo.
                             * @param name identificador, nome amigável ou tipo do dispositivo.
                             */
                            void removeUPnPDevice(const char* name);

                            /**
                             * Retorna a lista dos dispositivos controlados.
                             * @return lista dos dispositivos controlados.
                             */
                            UPnPDeviceList* getUPnPDeviceList();

                            /**
                             * Retorna um dispositivo da lista dos dispositivos controlados pelo identificador, nome amigável ou tipo do dispositivo.
                             * @param name identificador, nome amigável ou tipo do dispositivo.
                             * @return dispositivo.
                             */
                            UPnPDevice* getUPnPDevice(const char* name);

                            /**
                             * Verifica se um dispositivo faz parte da lista dos dispositivos controlados pelo identificador, nome amigável ou tipo do dispositivo.
                             * @param name identificador, nome amigável ou tipo do dispositivo.
                             * @return true, caso o dispositivo faça parte da lista, ou false, caso contrário.
                             */
                            bool hasUPnPDevice(const char* name);

                            /**
                             * Configura o tempo de busca.
                             * @param searchTime tempo que deve demorar uma busca por dispositivos em segundos.
                             */
                            void setSearchTime(int searchTime);

                            /**
                             * Retorna o tempo de busca.
                             * @return tempo que deve demorar uma busca por dispositivos em segundos.
                             */
                            int getSearchTime();

                            /**
                             * Configura o tempo de expiração das subscrições.
                             * @param timeOut tempo de expiração das subscrições.
                             */
                            void setSubscriptionTimeOut(int timeOut);

                            /**
                             * Retorna o tempo de expiração das subscrições.
                             * @return tempo de expiração das subscrições.
                             */
                            int getSubscriptionTimeOut();

                            /**
                             * Realiza busca por tipo de dispositivo com tempo para resposta.
                             * @param deviceType tipo de dispositivo buscado.
                             * @param searchTime tempo que deve demorar a busca em segundos.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool search(const char* deviceType, int searchTime);

                            /**
                             * Realiza busca por tipo de dispositivo.
                             * @param deviceType tipo de dispositivo buscado.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool search(const char* deviceType);

                            /**
                             * Realiza busca por todo tipo de dispositivo.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool searchAll();

                            /**
                             * Realiza busca por dispositivos principais.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool searchRootDevices();

                            /**
                             * Inscreve o controlador de dispositivos no serviço.
                             * @param service serviço que notificará o controlador de dispositivos quando ocorrer mudanças em suas variáveis de estado.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool subscribe(UPnPService* service);

                            /**
                             * Verifica se o controlador de dispositivos está inscrito em um serviço.
                             * @param service serviço.
                             * @return true, caso o controlador de dispositivos esteja inscrito no serviço, ou false, caso contrário.
                             */
                            bool isSubscribed(UPnPService* service);

                            /**
                             * Desfaz a subscrição do controlador de dispositivos em um serviço.
                             * @param service serviço.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool unsubscribe(UPnPService* service);

                            /**
                             * Desfaz a subscrição do controlador de dispositivos em todos os serviços de um dispositivo.
                             * @param device dispositivo.
                             */
                            void unsubscribe(UPnPDevice* device);

                            /**
                             * Desfaz a subscrição do controlador de dispositivos em todos os serviços de todos os dispositivos.
                             */
                            void unsubscribe();

                            /**
                             * Renova as subscrições do controlador de dispositivos em todos os serviços de um dispositivo.
                             * @param device dispositivo.
                             */
                            void renewSubscriptions(UPnPDevice* device);

                            /**
                             * Renova todas as subscrições do controlador de dispositivos em todos os serviços de todos os dispositivos.
                             */
                            void renewSubscriptions();

                            /**
                             * Atualiza o valor de variáveis de estado de um dispositivo.
                             * @param device dipositivo.
                             * @param sid identificador da subscrição.
                             * @param changedVariables variáveis que sofreram alteração em seus valores.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool updateUPnPStateVariables(UPnPDevice* device, const char* sid, DocumentPtr changedVariables);

                            /**
                             * Adiciona um <em>listener</em> de dispositivo ao controlador.
                             * @param deviceListener <em>listener</em> de dispositivo.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool addUPnPDeviceListener(UPnPDeviceListener* deviceListener);

                            /**
                             * Remove um <em>listener</em> de dispositivo do controlador.
                             * @param deviceListener <em>listener</em> de dispositivo.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool removeUPnPDeviceListener(UPnPDeviceListener* deviceListener);

                            /**
                             * Adiciona um <em>listener</em> de evento ao controlador.
                             * @param eventListener <em>listener</em> de evento.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool addUPnPEventListener(UPnPEventListener* eventListener);

                            /**
                             * Remove um <em>listener</em> de evento do controlador.
                             * @param eventListener <em>listener</em> de evento.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool removeUPnPEventListener(UPnPEventListener* eventListener);

                            /**
                             * Envia a solicitação de uma ação para um dispositivo remoto.
                             * @param action ação solicitada.
                             * @param res resposta da solicitação da ação.
                             */
                            void sendUPnPAction(UPnPAction* action, UPnPActionResponse* res);

                            /**
                             * Envia a solicitação da variável de estado.
                             * @param var variável de estado a ser consultada.
                             * @param res retorno da consulta à variável de estado.
                             */
                            void sendUPnPQueryRequest(UPnPStateVariable* var, UPnPQueryResponse* res);

                        private:

                            UPnPDeviceController();

                            ~UPnPDeviceController();
                            
                            /**
                             * Configura o estado de execução do controlador de dispositivo.
                             * @param running true, o controlador de dispositivo está ativo, ou false, caso contrário.
                             */
                            void setRunning(bool running);

                        };

                    }
                }
            }
        }
    }
}

#endif	/* _UPNPDEVICECONTROLLER_H */

