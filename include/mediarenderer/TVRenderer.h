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

#ifndef _TVRENDERER_H
#define	_TVRENDERER_H

#include <string>
#include <sstream>
#include "TVRConnectionManager.h"
#include "TVRAVTransport.h"
#include "TVRRenderingControl.h"
#include "PlayerHandler.h"
#include "UPnPService.h"
#include "UPnPDevice.h"

using namespace std;
using namespace br::ufscar::lince::ginga::hones::upnp;
using namespace br::ufscar::lince::ginga::hones::util;
using namespace br::ufscar::lince::ginga::hones::ui;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediarenderer {

                        class TVRenderer : public UPnPDevice {
                        private:
                            TVRConnectionManager *conMan;
                            TVRRenderingControl *renCtl;
                            TVRAVTransport *avTransp;
                            PlayerHandler *player;

                        public:
                            static const char *DEVICE_TYPE;
                            static const char *UUID_PATH;
                            static const char *DESCRIPTION;


                            TVRenderer();

                            ~TVRenderer();

                            /**
                             * Retorna a instância do serviço gerenciador de conexão.
                             * @return serviço gerenciador de conexão.
                             */
                            TVRConnectionManager* getConnectionManager();

                            /**
                             * Retorna a instância do serviço gerenciador de apresentação.
                             * @return serviço gerenciador de apresentação.
                             */
                            TVRRenderingControl* getRenderingControl();

                            /**
                             * Retorna a instância do serviço gerenciador de transferência de mídia.
                             * @return serviço gerenciador de transferência de mídia.
                             */
                            TVRAVTransport* getAVTransport();

                            /**
                             * Retorna a instância do manipulador de <em>players</em>.
                             * @return manipulador de <em>players</em>.
                             */
                            PlayerHandler* getPlayerHandler();

                            /**
                             * Inicia o Serviço de Apresentação.
                             * @return true, caso obtena sucesso na operação, ou false, caso contrário.
                             */
                            bool start();

                            /**
                             * Pára o Serviço de Apresentação.
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

#endif	/* _TVRENDERER_H */

