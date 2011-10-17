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

#ifndef _UPNPSTATEVARIABLE_H
#define	_UPNPSTATEVARIABLE_H

#include <string>
#include "UPnPAllowedValueList.h"
#include "UPnPAllowedValueRange.h"
#include "UPnPStatus.h"
#include "Utils.h"
using namespace std;
using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {
                        class UPnPService;
                        class UPnPQueryListener;
                        class UPnPQueryRequest;
                        class UPnPQueryResponse;

                        class UPnPStateVariable {
                        private:
                            NodePtr node;
                            UPnPService* service;
                            UPnPAllowedValueList allowedValueList;
                            UPnPAllowedValueRange* allowedValueRange;
                            UPnPQueryResponse* queryResponse;
                            UPnPQueryListener* queryListener;
                            UPnPStatus status;
                            string varValue;
                            static const string ELEMENT_NAME;
                            static const string NAME;
                            static const string DATATYPE;
                            static const string SENDEVENTS;
                            static const string YES;
                            static const string NO;

                        public:

                            UPnPStateVariable();
                            UPnPStateVariable(NodePtr node);
                            ~UPnPStateVariable();

                            /**
                             * Retorna a instância do serviço ao qual a variável de estado está associada.
                             * @return serviço.
                             */
                            UPnPService* getUPnPService();

                            /**
                             * Configura a instância do serviço ao qual a variável de estado está associada.
                             * @param service serviço.
                             */
                            void setUPnPService(UPnPService* service);

                            /**
                             * Diz se o nó XML passado como parâmetro descreve uma variável de estado UPnP.
                             * @param node nó XML.
                             * @return true, caso o nó XML descreva uma variável de estado, ou false, caso contrário.
                             */
                            static bool isUPnPStateVariableNode(NodePtr node);

                            /**
                             * Retorna a lista de valores permitidos à variável de estado.
                             * @return lista de valores permitidos.
                             */
                            UPnPAllowedValueList* getUPnPAllowedValueList();

                            /**
                             * Verifica se a variável de estado possui lista de valores permitidos.
                             * @return true, caso a variável de estado possua lista de valores permitidos, ou false, caso contrário.
                             */
                            bool hasUPnPAllowedValueList();

                            /**
                             * Retorna a faixa de valores permitidos à variável de estado.
                             * @return faixa de valores permitidos.
                             */
                            UPnPAllowedValueRange* getUPnPAllowedValueRange();

                            /**
                             * Verifica se a variável de estado possui faixa de valores permitidos.
                             * @return true, caso a variável de estado possua faixa de valores permitidos, ou false, caso contrário.
                             */
                            bool hasUPnPAllowedValueRange();

                            /**
                             * Configura o nome da variável de estado.
                             * @param value nome da variável de estado.
                             */
                            void setName(const char* value);

                            /**
                             * Retorna o nome da variável de estado.
                             * @return nome da variável de estado.
                             */
                            const char* getName();

                            /**
                             * Configura o tipo de dados aceito pela variável de estado.
                             * @param value tipo de dados aceito pela variável de estado.
                             */
                            void setDataType(const char* value);

                            /**
                             * Retorna o tipo de dados aceito pela variável de estado.
                             * @return tipo de dados aceito pela variável de estado.
                             */
                            const char* getDataType();

                            /**
                             * Configura a variável de estado para envio de eventos.
                             * @param state true, se a variável de estado deve notificar caso ocorra alguma alteração em seus atributos, ou false, caso contrário.
                             */
                            void setSendEvents(bool state);

                            /**
                             * Verifica se a variável de estado deve notificar (envio de eventos) caso ocorra alguma alteração em seus atributos.
                             * @return true, se a variável de estado deve notificar caso ocorra alguma alteração, ou false, caso contrário.
                             */
                            bool isSendEvents();

                            /**
                             * Configura o valor da variável de estado.
                             * @param value valor da variável de estado.
                             */
                            void setValue(const char* value);

                            /**
                             * Configura o valor (inteiro) da variável de estado.
                             * @param value valor da variável de estado.
                             */
                            void setValue(int value);

                            /**
                             * Configura o valor (<em>long</em>) da variável de estado.
                             * @param value valor da variável de estado.
                             */
                            void setValue(long value);

                            /**
                             * Retorna o valor da variável de estado.
                             * @return valor da variável de estado.
                             */
                            const char* getValue();

                            /**
                             * Copia os atributos da variável de estado passada como parâmetro.
                             * @param var variável de estado a ser copiada.
                             */
                            void setUPnPStateVariable(UPnPStateVariable* var);

                            /**
                             * Retorna o <em>listener</em> para consultas à variável de estado.
                             * @return <em>listener</em> de consultas.
                             */
                            UPnPQueryListener* getUPnPQueryListener();

                            /**
                             * Configura o <em>listener</em> para consultas à variável de estado.
                             * @param listener <em>listener</em> de consultas.
                             */
                            void setUPnPQueryListener(UPnPQueryListener* listener);

                            /**
                             * Processa uma consulta à variável de estado.
                             * @param req requisição da consulta.
                             * @return true, se a ação for executada com sucesso, ou false, caso contrário.
                             */
                            bool executeUPnPQueryListener(UPnPQueryRequest* req);

                            /**
                             * Retorna a resposta de uma consulta a uma variável de estado.
                             * @return resposta de uma consulta a uma variável de estado.
                             */
                            UPnPQueryResponse* getUPnPQueryResponse();

                            /**
                             * Retorna o </em>status</em> da consulta à variável de estado.
                             * @return </em>status</em> da consulta à variável de estado.
                             */
                            UPnPStatus* getUPnPQueryStatus();

                            /**
                             * Envia uma consulta à variável de estado.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool sendUPnPQueryAction();

                            /**
                             * Configura o <em>status</em> da variável de estado.
                             * @param code código de <em>status</em>.
                             * @param desc descrição do <em>status</em>.
                             */
                            void setStatus(int code, const char *desc);

                            /**
                             * Configura o <em>status</em> da variável de estado.
                             * @param code código de <em>status</em>.
                             */
                            void setStatus(int code);

                            /**
                             * Retorna o <em>status</em> da variável de estado.
                             * @return <em>status</em> da variável de estado.
                             */
                            UPnPStatus* getStatus();

                        private:

                            /**
                             * Atribui um nó XML descrevendo a variável de estado.
                             * @param node nó XML.
                             */
                            void setNode(NodePtr node);

                            /**
                             * Retorna o nó XML que descreve a variável de estado.
                             * @return nó XML.
                             */
                            NodePtr getNode();

                            /**
                             * Configura o retorno de uma consulta a uma variável de estado.
                             * @param res retorno de uma consulta a uma variável de estado.
                             */
                            void setUPnPQueryResponse(UPnPQueryResponse *res);

                            /**
                             * Inicializa a lista de valores permitidos à variável de estado.
                             */
                            void initializeUPnPAllowedValueList();

                            /**
                             * Inicializa a faixa de valores permitidos à variável de estado.
                             */
                            void initializeUPnPAllowedValueRange();

                        };
                    }
                }
            }
        }
    }
}

#endif	/* _UPNPSTATEVARIABLE_H */

