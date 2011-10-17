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

#ifndef _TVDEVICEID_H
#define	_TVDEVICEID_H

#include <string>
#include <string.h>
#include "UPnPDevice.h"
#include "UPnPActionListener.h"
#include "UPnPEventListener.h"
#include "UPnPQueryListener.h"
#include "UPnPDeviceListener.h"
#include "UPnPService.h"
#include "UPnPAction.h"
#include "UPnPArgument.h"
#include "UPnPStateVariable.h"
#include "UPnPDeviceController.h"
#include "BtDeviceController.h"
#include "BtDeviceListener.h"

using namespace std;
using namespace br::ufscar::lince::ginga::hones::upnp;
using namespace br::ufscar::lince::ginga::hones::idservice;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace idservice {
                    
                    class TVDeviceId : public UPnPDevice, public UPnPActionListener, public UPnPQueryListener, public UPnPEventListener, public UPnPDeviceListener, public BtDeviceListener {
                    private:
                        static const char *TVDeviceId_DEVICE_DESCRIPTION;
                        static const char *TVDeviceId_SERVICE_DESCRIPTION;
                        UPnPDeviceController *upnpControl;
                        BtDeviceController *btControl;

                    public:
                            /**
                             * Construtor.
                             */
                        TVDeviceId();

                            /**
                             * Destrutor.
                             */
                        ~TVDeviceId();

                            /**
                             * Retorna a instância do serviço.
                             * @return serviço.
                             */
                            UPnPService* getService();

                            /**
                             * Adiciona um <em>listener</em> de dispositivo ao controlador.
                             * @param deviceListener <em>listener</em> de dispositivo.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool addBtDeviceListener(BtDeviceListener* deviceListener);

                            /**
                             * Remove um <em>listener</em> de dispositivo do controlador.
                             * @param deviceListener <em>listener</em> de dispositivo.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool removeBtDeviceListener(BtDeviceListener* deviceListener);

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
                             * Novo dispositivo encontrado e/ou adicionado.
                             * @param device novo dispositivo.
                             */
                            void deviceAdded(BtDevice* device);

                            /**
                             * Dispositivo desligado ou removido.
                             * @param device dispositivo.
                             */
                            void deviceRemoved(BtDevice* device);

                            /**
                             * Novo dispositivo encontrado e/ou adicionado.
                             * @param device novo dispositivo.
                             */
                            void deviceAdded(UPnPDevice* device);

                            /**
                             * Dispositivo desligado ou removido.
                             * @param device dispositivo.
                             */
                            void deviceRemoved(UPnPDevice* device);

                            /**
                             * Evento de notificação recebido.
                             * @param device dispositivo que sofreu alguma alteração.
                             * @param service serviço que sofreu alguma alteração.
                             * @param sid identificador da subscrição.
                             * @param variableName nome da variável de estado que foi alterada.
                             * @param newVariableValue novo valor da variável de estado.
                             */
                            void eventReceived(UPnPDevice* device, UPnPService* service, const char* sid, const char* variableName, const char* newVariableValue);
                        
                    };
                    
                    }
                }
            }
        }
    }
}

#endif	/* _TVDEVICEID_H */

