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

#ifndef _UPNPACTIONRESPONSE_H
#define	_UPNPACTIONRESPONSE_H

#include "UPnPStatus.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {
                        class UPnPAction;
                        class UPnPArgumentList;

                        class UPnPActionResponse {
                        private:
                            UPnPAction* action;
                            UPnPArgumentList* response;
                            UPnPStatus status;

                        public:
                            
                            UPnPActionResponse();
                            ~UPnPActionResponse();

                            /**
                             * Configura a resposta de uma ação.
                             * @param action ação com os dados da resposta.
                             */
                            void setUPnPAction(UPnPAction* action);

                            /**
                             * Retorna a ação com os dados da resposta.
                             * @return ação de resposta.
                             */
                            UPnPAction* getUPnPAction();

                            /**
                             * Configura a resposta de uma ação.
                             * @param response lista de argumentos com valores retornados do dispositivo remoto.
                             */
                            void setResponse(UPnPArgumentList* response);

                            /**
                             * Retorna uma lista de argumentos com valores dados pelo dispositivo remoto.
                             * @return lista de argumentos de resposta.
                             */
                            UPnPArgumentList* getResponse();

                            /**
                             * Configura uma falha na solicitação da ação.
                             * @param errCode código de erro.
                             * @param description descrição do erro.
                             */
                            void setFaultResponse(int errCode, const char* description);

                            /**
                             * Retorna o <em>status</em> da resposta de uma ação.
                             * @return <em>status</em> da resposta.
                             */
                            UPnPStatus* getStatus();

                            /**
                             * Diz se a ação solicitada foi executada com sucesso.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool isSuccessful();
                        };

                    }
                }
            }
        }
    }
}

#endif	/* _UPNPACTIONRESPONSE_H */

