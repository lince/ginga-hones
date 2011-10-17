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

#ifndef _TVMEDIACENTER_H
#define	_TVMEDIACENTER_H

#include <string>
#include <set>
#include "PlayerHandler.h"
#include "Frame.h"
#include "Container.h"
#include "TextArea.h"
#include "SelectableTextArea.h"
#include "UPnPDeviceController.h"
#include "Utils.h"
#include "Vector.h"
#include <ginga/system/io/IInputManager.h>
#include <ginga/system/io/interface/input/IInputEventListener.h>
#include <ginga/system/io/interface/input/CodeMap.h>
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
using namespace std;
using namespace br::ufscar::lince::ginga::hones::ui;
using namespace br::ufscar::lince::ginga::hones::util;
using namespace br::ufscar::lince::ginga::hones::upnp;
using namespace ::br::pucrio::telemidia::ginga::core::system::io;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediacenter {

                        class TVMediaCenter : public IInputEventListener  {
                        private:
                            PlayerHandler* player;
                            HLoggerPtr logger;
                            Frame* initWindow;
                            Frame* serverWindow;
                            Frame* audioWindow;
                            Frame* videoWindow;
                            Frame* imageWindow;
                            Frame* mediaWindow;
                            Frame* clientWindow;
                            Frame* playbackWindow;
                            int playbackWindowVisibleCount;
                            string clientUDN;
                            string serverUDN;
                            string mediaClass;
                            string mediaURI;
                            bool hasBeenInit;
                            bool playbackRunning;
                            UPnPDeviceController* upnpCtrl;
                            Vector itemList;
                            int itemListIndex;
                            int startingIndex;
                            int totalMatches;
                            string upnpParentID;
                            static IInputManager* im;

                        public:

                            TVMediaCenter();

                            ~TVMediaCenter();

                            /**
                             * Retorna a instância do manipulador de <em>players</em>.
                             * @return manipulador de <em>players</em>.
                             */
                            PlayerHandler* getPlayerHandler();

                            /**
                             * Inicia o TV Media Center.
                             * @return true, caso obtena sucesso na operação, ou false, caso contrário.
                             */
                            bool start();

                            /**
                             * Pára o TV Media Center.
                             * @return true, caso obtena sucesso na operação, ou false, caso contrário.
                             */
                            bool stop();

                            /**
                             * Recebe eventos do usuário.
                             * @param ev evento.
                             * @return true, caso o evento seja processado com sucesso, ou false, caso contrário.
                             */
                            bool userEventReceived(IInputEvent* ev);

                            void playbackWindowThread();
                            
                        private:

                            /**
                             * Instancia a maioria dos componentes visuais.
                             */
                            void initComponents();

                            void showServerWindow();

                            void showClientWindow();

                            void showMediaWindow(const char* parent);

                            void clearItemList();

                            void showItemList(Container* container);

                            void playpause(const char* uri);

                            void backward();

                            void forward();

                            void getMetadata(string& metadata);

                        };

                    }
                }
            }
        }
    }
}

#endif	/* _TVMEDIACENTER_H */

