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

#ifndef _UPNPSERVICE_H
#define	_UPNPSERVICE_H

#include <string.h>
#include <string>
#include "Utils.h"
#include "UPnPActionList.h"
#include "UPnPStateVariable.h"
#include "UPnPServiceStateTable.h"
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
                        class UPnPAction;
                        class UPnPDevice;
                        class UPnPQueryListener;
                        class UPnPActionListener;

                        class UPnPService {
                        private:
                            NodePtr node;
                            NodePtr SCPD_Node;
                            UPnPActionList actions;
                            UPnPServiceStateTable stateVariableTable;
                            UPnPDevice* device;
                            char* subscriptionID;
                            HLoggerPtr logger;
                            static const string ELEMENT_NAME;
                            static const string SERVICE_TYPE;
                            static const string SERVICE_ID;
                            static const string SCPD_URL;
                            static const string CONTROL_URL;
                            static const string EVENT_SUB_URL;
                            static const string SCPD;

                        public:

                            UPnPService(NodePtr node);
                            UPnPService(NodePtr node, UPnPDevice* device);
                            ~UPnPService();

                            /**
                             * Diz se o nó XML passado como parâmetro descreve um serviço UPnP.
                             * @param node nó XML.
                             * @return true, caso o nó XML descreva um serviço, ou false, caso contrário.
                             */
                            static bool isUPnPServiceNode(NodePtr node);

                            /**
                             * Atribui a instância do dispositivo ao qual o serviço está associado.
                             * @param device dispositivo.
                             */
                            void setUPnPDevice(UPnPDevice* device);

                            /**
                             * Retorna a instância do dispositivo ao qual o serviço está associado.
                             * @return dispositivo.
                             */
                            UPnPDevice* getUPnPDevice();

                            /**
                             * Retorna a instância do dispositivo principal ao qual o serviço está associado.
                             * @return dispositivo principal.
                             */
                            UPnPDevice* getUPnPRootDevice();

                            /**
                             * Configura o tipo do serviço.
                             * @param type tipo do serviço.
                             */
                            void setServiceType(const char* type);

                            /**
                             * Retorna o tipo do serviço.
                             * @return tipo do serviço.
                             */
                            const char* getServiceType();

                            /**
                             * Configura o identificador do serviço.
                             * @param id identificador do serviço.
                             */
                            void setServiceID(const char* id);

                            /**
                             * Retorna o identificador do serviço.
                             * @return identificador do serviço.
                             */
                            const char* getServiceID();

                            /**
                             * Verifica se o serviço possui identificador.
                             * @return true, caso o serviço possua identificador, ou false, caso contrário.
                             */
                            bool hasServiceID();

                            /**
                             * Configura a URL do documento XML que descreve o serviço.
                             * @param url URL do documento XML.
                             */
                            void setSCPDURL(const char* url);

                            /**
                             * Retorna a URL do documento XML que descreve o serviço.
                             * @return URL do documento XML.
                             */
                            const char* getSCPDURL();

                            /**
                             * Verifica se o parâmetro passado corresponde a URL do documento XML que descreve o serviço.
                             * @param url URL a ser comparada.
                             * @return true, caso o parâmetro passado corresponda a URL do documento XML, ou false, caso contrário.
                             */
                            bool isSCPDURL(const char* url);

                            /**
                             * Retorna o documento XML de descrição do serviço.
                             * @return descrição XML do serviço.
                             */
                            const char* getSCPD();

                            /**
                             * Carrega o documento de descrição do serviço a partir do parâmetro passado.
                             * @param desc descrição XML do serviço.
                             * @return true, caso a descrição XML esteja correta, ou false, caso contrário.
                             */
                            bool loadSCPD(const char* desc);

                            /**
                             * Retorna o nó XML que descreve o serviço.
                             * @return nó XML.
                             */
                            NodePtr getSCPDNode();

                            /**
                             * Configura a URL de controle do serviço.
                             * @param url URL de controle do serviço.
                             */
                            void setControlURL(const char* url);

                            /**
                             * Retorna a URL de controle do serviço.
                             * @return URL de controle do serviço.
                             */
                            const char* getControlURL();

                            /**
                             * Verifica se o parâmetro passado corresponde a URL de controle do serviço.
                             * @param url URL a ser comparada.
                             * @return true, caso o parâmetro passado corresponda a URL de controle, ou false, caso contrário.
                             */
                            bool isControlURL(const char* url);

                            /**
                             * Configura a URL de subscrição em eventos do serviço.
                             * @param url URL de subscrição do serviço.
                             */
                            void setEventSubURL(const char* url);

                            /**
                             * Retorna a URL de subscrição em eventos do serviço.
                             * @return URL de subscrição do serviço.
                             */
                            const char* getEventSubURL();

                            /**
                             * Verifica se o parâmetro passado corresponde a URL de subscrição em eventos do serviço.
                             * @param url URL a ser comparada.
                             * @return true, caso o parâmetro passado corresponda a URL de subscrição, ou false, caso contrário.
                             */
                            bool isEventSubURL(const char* url);

                            /**
                             * Retorna a lista de ações do serviço.
                             * @return lista de ações.
                             */
                            UPnPActionList* getUPnPActions();

                            /**
                             * Retorna uma ação do serviço pelo nome.
                             * @param name nome da ação.
                             * @return ação.
                             */
                            UPnPAction* getUPnPAction(const char* name);

                            /**
                             * Retorna a tabela de variáveis de estado do serviço.
                             * @return tabela de variáveis de estado.
                             */
                            UPnPServiceStateTable* getUPnPServiceStateTable();

                            /**
                             * Retorna uma variável de estado do serviço pelo nome.
                             * @param name nome da variável.
                             * @return variável de estado.
                             */
                            UPnPStateVariable* getUPnPStateVariable(const char* name);

                            /**
                             * Verifica se o serviço possui alguma variável de estado com o nome passado como parâmetro.
                             * @param name nome da variável de estado procurada.
                             * @return true, caso o serviço possua a variável de estado, ou false, caso contrário.
                             */
                            bool hasUPnPStateVariable(const char* name);

                            /**
                             * Verifica se o parâmetro passado corresponde ao serviço.
                             * @param name tipo ou identificador do serviço a ser comparado.
                             * @return true, caso o parâmetro passado corresponda ao serviço, ou false, caso contrário.
                             */
                            bool isUPnPService(const char* name);

                            /**
                             * Envia uma notificão de alteração em uma variável de estado aos dispositivos registrados no serviço.
                             * @param var variável de estado alterada.
                             */
                            void sendEventMessage(UPnPStateVariable* var);

                            /**
                             * Envia notificões de alteração de todas as variáveis de estado aos dispositivos registrados no serviço.
                             */
                            void notifyAllUPnPStateVariables();

                            /**
                             * Configura o <em>listener</em> para consultas ao serviço.
                             * @param listener <em>listener</em> de consultas.
                             */
                            void setUPnPQueryListener(UPnPQueryListener* listener);

                            /**
                             * Configura o <em>listener</em> para ações do serviço.
                             * @param listener <em>listener</em> de ações.
                             */
                            void setUPnPActionListener(UPnPActionListener* listener);

                            /**
                             * Verifica se o serviço está registrado para receber notificações/eventos.
                             * @return true, caso o serviço esteja registrado para receber notificações/eventos, ou false, caso contrário.
                             */
                            bool isSubscribed();

                            /**
                             * Configura o identificador do registro para receber notificações/eventos.
                             * @param sid identificador do registro para receber notificações/eventos.
                             */
                            void setSubscriptionID(char* sid);

                            /**
                             * Retorna o identificador do registro para receber notificações/eventos.
                             * @return identificador do registro para receber notificações/eventos.
                             */
                            char* getSubscriptionID();

                        private:

                            /**
                             * Atribui um nó XML que aponta o documento XML que descreve o serviço.
                             * @param node nó XML.
                             */
                            void setNode(NodePtr node);

                            /**
                             * Retorna o nó XML que aponta o documento XML que descreve o serviço.
                             * @return nó XML.
                             */
                            NodePtr getNode();

                            /**
                             * Atribui um nó XML descrevendo o serviço.
                             * @param node nó XML.
                             */
                            void setSCPDNode(NodePtr node);

                            /**
                             * Inicializa a lista de ações do serviço.
                             */
                            void initializeUPnPActionList();

                            /**
                             * Inicializa a tabela de variáveis de estado do serviço.
                             */
                            void initializeUPnPServiceStateTable();

                        };
                    }
                }
            }
        }
    }
}

#endif	/* _UPNPSERVICE_H */

