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

#ifndef _UPNPACTION_H
#define	_UPNPACTION_H

#include <string>
#include "Utils.h"
#include "UPnPStatus.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
using namespace std;
using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {
                        class UPnPService;
                        class UPnPArgumentList;
                        class UPnPArgument;
                        class UPnPActionListener;
                        class UPnPActionRequest;

                        class UPnPAction {
                        private:
                            NodePtr node;
                            UPnPService* service;
                            UPnPArgumentList* arguments;
                            UPnPArgumentList* argumentsIn;
                            UPnPArgumentList* argumentsOut;
                            UPnPStatus status;
                            UPnPActionListener* actionListener;
                            string controlHost;
                            HLoggerPtr logger;
                            static const string ELEMENT_NAME;
                            static const string NAME;

                        public:

                            UPnPAction(UPnPService* service, NodePtr node);
                            ~UPnPAction();

                            /**
                             * Retorna o serviço UPnP ao qual está associada a ação.
                             * @return serviço.
                             */
                            UPnPService* getUPnPService();

                            /**
                             * Associa a ação a um serviço UPnP.
                             * @param service serviço ao qual estará associada a ação.
                             */
                            void setUPnPService(UPnPService* service);

                            /**
                             * Diz se o nó XML passado como parâmetro descreve uma ação.
                             * @param node nó XML.
                             * @return true, caso o nó XML descreva uma ação, ou false, caso contrário.
                             */
                            static bool isUPnPActionNode(NodePtr node);

                            /**
                             * Configura o nome da ação.
                             * @param value nome da ação.
                             */
                            void setName(const char* value);

                            /**
                             * Retorna o nome da ação.
                             * @return nome da ação.
                             */
                            const char* getName();

                            /**
                             * Retorna todos (entrada e saída) os argumentos da ação.
                             * @return lista de argumentos.
                             */
                            UPnPArgumentList* getUPnPArguments();

                            /**
                             * Retorna os argumentos de entrada da ação.
                             * @return lista de argumentos.
                             */
                            UPnPArgumentList* getInputUPnPArguments();

                            /**
                             * Retorna os argumentos de saída da ação.
                             * @return lista de argumentos.
                             */
                            UPnPArgumentList* getOutputUPnPArguments();

                            /**
                             * Retorna um argumento da ação pelo seu nome.
                             * @param name nome do argumento.
                             * @return argumento, caso seja encontrado, ou NULL, caso contrário.
                             */
                            UPnPArgument* getUPnPArgument(const char* name);

                            /**
                             * Atribui novos valores aos argumentos da ação.
                             * @param list lista de argumentos com novos valores.
                             */
                            void setUPnPArgumentValues(UPnPArgumentList* list);

                            /**
                             * Atribui um novo valor a um determinado argumento da ação.
                             * @param name nome do argumento da ação.
                             * @param value novo valor.
                             */
                            void setUPnPArgumentValue(const char* name, const char* value);

                            /**
                             * Atribui um novo valor (inteiro) a um determinado argumento da ação.
                             * @param name nome do argumento da ação.
                             * @param value novo valor.
                             */
                            void setUPnPArgumentValue(const char* name, int value);

                            /**
                             * Retorna o valor de um argumento da ação.
                             * @param name nome do argumento.
                             */
                            const char* getUPnPArgumentValue(const char* name);

                            /**
                             * Retorna o valor (inteiro) de um argumento da ação.
                             * @param name nome do argumento.
                             */
                            int getUPnPArgumentIntegerValue(const char* name);

                            /**
                             * Retorna o <em>listener</em> da ação.
                             * @return <em>listener</em> da ação.
                             */
                            UPnPActionListener* getUPnPActionListener();

                            /**
                             * Configura o <em>listener</em> da ação.
                             * @param listener <em>listener</em> da ação.
                             */
                            void setUPnPActionListener(UPnPActionListener* listener);

                            /**
                             * Invoca o <em>listener</em> da ação.
                             * @param req ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool executeUPnPActionListener(UPnPActionRequest* req);

                            /**
                             * Envia a ação como uma solicitação para um dispositivo remoto.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool sendUPnPAction();

                            /**
                             * Configura o <em>status</em> da ação com um código e sua descrição.
                             * @param code código de <em>status</em>.
                             * @param desc descrição.
                             */
                            void setStatus(int code, const char *desc);

                            /**
                             * Configura o <em>status</em> da ação com um código de erro ou sucesso.
                             * @param code código de <em>status</em>.
                             */
                            void setStatus(int code);

                            /**
                             * Retorna o <em>status</em> da ação.
                             * @return <em>status</em> da ação.
                             */
                            UPnPStatus* getStatus();

                            /**
                             * Atribui o endereço IP do dispositivo que invocou a ação.
                             * @param ip endereço IP.
                             */
                            void setControlHost(const char* ip);

                            /**
                             * Retorna o endereço IP do dispositivo que invocou a ação.
                             * @return endereço IP.
                             */
                            const char* getControlHost();

                        private:

                            /**
                             * Atribui um nó XML descrevendo a ação.
                             * @param node nó XML.
                             */
                            void setNode(NodePtr node);

                            /**
                             * Retorna o nó XML que descreve a ação.
                             * @return nó XML.
                             */
                            NodePtr getNode();

                            /**
                             * Monta a lista de argumentos da ação a partir de sua descrição XML.
                             */
                            void initializeUPnPArgumentList();

                            /**
                             * Limpa os valores dos argumentos de saída da ação.
                             */
                            void clearOutputUPnPArgumentValues();

                        };
                    }
                }
            }
        }
    }
}

#endif	/* _UPNPACTION_H */

