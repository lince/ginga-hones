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

#ifndef _TVCONTROL_H
#define	_TVCONTROL_H

#include <string>
#include "UPnPDevice.h"
#include "UPnPActionListener.h"
#include "UPnPQueryListener.h"
#include "UPnPService.h"
#include "UPnPAction.h"
#include "UPnPArgument.h"
#include "UPnPStateVariable.h"
#include "TVControlHandler.h"

using namespace std;
using namespace br::ufscar::lince::ginga::hones::upnp;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace tvcontrol {

                        class TVControl : public UPnPDevice, public UPnPActionListener, public UPnPQueryListener {
                        private:
                            static const char *TVCONTROL_DEVICE_DESCRIPTION;
                            static const char *TVCONTROL_SERVICE_DESCRIPTION;
                            TVControlHandler* control;

                        public:

                            /**
                             * Construtor.
                             */
                            TVControl();

                            /**
                             * Destrutor.
                             */
                            ~TVControl();

                            /**
                             * Retorna a instância do serviço de controle das funcionalidades básicas da TV.
                             * @return serviço de controle das funcionalidades básicas da TV.
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
                        };

                    }
                }
            }
        }
    }
}

#endif	/* _TVCONTROL_H */

