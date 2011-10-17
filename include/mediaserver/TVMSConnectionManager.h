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

#ifndef _TVMSCONNECTIONMANAGER_H
#define	_TVMSCONNECTIONMANAGER_H

#include "UPnPActionListener.h"
#include "UPnPQueryListener.h"
#include "UPnPService.h"
#include "UPnPAction.h"
#include "UPnPArgument.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
using namespace std;
using namespace br::ufscar::lince::ginga::hones::upnp;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediaserver {

                        class TVMediaServer;

                        class TVMSConnectionManager : public UPnPActionListener, public UPnPQueryListener {
                        private:
                            TVMediaServer* tvServer;
                            HLoggerPtr logger;

                        public:

                            static const char *SERVICE_TYPE;

                            static const char *GET_PROTOCOL_INFO;
                            static const char *SOURCE;
                            static const char *SINK;
                            static const char *SOURCE_PROTOCOL_INFO;
                            static const char *SINK_PROTOCOL_INFO;
                            static const char *CURRENT_CONNECTION_IDS;

                            static const char *PEER_CONNECTION_MANAGER;
                            static const char *PEER_CONNECTION_ID;
                            static const char *DIRECTION;
                            static const char *AV_TRNSPORT_ID;
                            static const char *RCS_ID;

                            static const char *GET_CURRENT_CONNECTION_IDS;
                            static const char *CONNECTION_IDS;

                            static const char *GET_CURRENT_CONNECTION_INFO;
                            static const char *PROTOCOL_INFO;
                            static const char *STATUS;
                            static const char *OUTPUT;
                            static const char *OK;

                            static const char *SCPD;


                            TVMSConnectionManager(TVMediaServer* tvs);
                            ~TVMSConnectionManager();

                            /**
                             * Inicializa as variáveis de estado necessárias ao funcionamento do serviço gerenciador de conexão.
                             */
                            void initializeUPnPStateVariables();

                            /**
                             * Retorna a instância do servidor de mídias.
                             * @return servidor de mídias.
                             */
                            TVMediaServer* getMediaServer();

                            /**
                             * Retorna a instância do serviço gerenciador de conexão.
                             * @return serviço gerenciador de conexão.
                             */
                            UPnPService* getService();

                            /**
                             * Recebe solicitações de ações a serem executadas.
                             * @param action ação a ser executada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionRequest(UPnPAction *action);

                            /**
                             * Envia informações sobre os protocolos suportados.
                             * Os protocolos são enviados em uma lista separados por vírgula no argumento <em>Source</em> ou <em>Sink</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - Source
                             *      - Sink
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetProtocolInfo(UPnPAction *action);

                            /**
                             * Envia informações sobre uma conexão ativa do servidor de mídias, tais como protocolo e direção.
                             *
                             * Argumentos de entrada da ação:
                             *      - ConnectionID
                             *
                             * Argumentos de saída da ação:
                             *      - RcsID
                             *      - AVTransportId
                             *      - ProtocolInfo
                             *      - PeerConnectionManager
                             *      - PeerConnectionId
                             *      - Direction
                             *      - Status
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetCurrentConnectionInfo(UPnPAction *action);

                            /**
                             * Envia os identificadores das conexões ativas.
                             * Os identificadores são enviados em uma lista separados por vírgula no argumento <em>ConnectionIDs</em>.
                             *
                             * Argumentos de saída da ação:
                             *      - ConnectionIDs
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetCurrentConnectionIDs(UPnPAction *action);

                            /**
                             * Envia o estado atual de uma variável do serviço.
                             * @param stateVar variável solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool variableRequest(UPnPStateVariable *stateVar);

                        private:
                            /**
                             * Configura a instância do servidor de mídias.
                             * @param tvs nova instância.
                             */
                            void setMediaServer(TVMediaServer* tvs);

                        };

                    }
                }
            }
        }
    }
}

#endif	/* _TVMSCONNECTIONMANAGER_H */

