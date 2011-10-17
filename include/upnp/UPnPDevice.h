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

#ifndef _UPNPDEVICE_H
#define	_UPNPDEVICE_H

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <pthread.h>
#include <string>
#include "Utils.h"
#include "UPnPDeviceList.h"
#include "UPnPServiceList.h"
#include "UPnPIconList.h"
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
                        class UPnPStateVariable;
                        class UPnPAction;
                        class UPnPIcon;
                        class UPnPQueryListener;
                        class UPnPActionListener;

                        class UPnPDevice {
                        private:
                            NodePtr node;
                            NodePtr rootnode;
                            UpnpDevice_HandlePtr device_handle;
                            UPnPDevice* root;
                            UPnPDevice* parent;
                            UPnPDeviceList embeddedDevices;
                            UPnPServiceList services;
                            UPnPIconList icons;
                            string uuid;
                            string descriptionURI;
                            string location;
                            pthread_t* adsThread;
                            static UPnPDeviceList* localRootDeviceInstances;
                            HLoggerPtr logger;
                            static const string ELEMENT_NAME;
                            static const string UPNP_ROOTDEVICE;
                            static const string DEFAULT_DESCRIPTION_URI;
                            static const string URLBASE;
                            static const string DEVICE_TYPE;
                            static const string FRIENDLY_NAME;
                            static const string MANUFACTURE;
                            static const string MANUFACTURE_URL;
                            static const string MODEL_DESCRIPTION;
                            static const string MODEL_NAME;
                            static const string MODEL_NUMBER;
                            static const string MODEL_URL;
                            static const string SERIAL_NUMBER;
                            static const string UDN;
                            static const string UPC;
                            static const string PRESENTATION_URL;

                        public:

                            UPnPDevice();
                            UPnPDevice(NodePtr node);
                            UPnPDevice(NodePtr rootnode, const char* location);
                            ~UPnPDevice();

                            /**
                             * Retorna o nó XML que descreve o dispositivo principal.
                             * @return nó XML.
                             */
                            NodePtr getRootNode();

                            /**
                             * Diz se o nó XML passado como parâmetro descreve um dispositivo UPnP.
                             * @param node nó XML.
                             * @return true, caso o nó XML descreva um dispositivo, ou false, caso contrário.
                             */
                            static bool isUPnPDeviceNode(NodePtr node);

                            /**
                             * Diz se o dispositivo é o principal.
                             * @return true, caso o dispositivo seja o principal, ou false, caso contrário.
                             */
                            bool isUPnPRootDevice();

                            /**
                             * Retorna o dispositivo principal ao qual está associado.
                             * @return dispositivo principal.
                             */
                            UPnPDevice* getUPnPRootDevice();

                            /**
                             * Retorna o dispositivo ao qual está associado.
                             * @return dispositivo pai.
                             */
                            UPnPDevice* getUPnPParentDevice();

                            /**
                             * Associa o dispositivo a um dispositivo pai.
                             * @param parent dispositivo pai.
                             */
                            void setUPnPParentDevice(UPnPDevice* parent);

                            /**
                             * Configura a URI do documento XML que descreve o dispositivo.
                             * @param uri URI do documento XML.
                             */
                            void setDescriptionURI(const char* uri);

                            /**
                             * Retorna a URI do documento XML que descreve o dispositivo.
                             * @return URI do documento XML.
                             */
                            const char* getDescriptionURI();

                            /**
                             * Verifica se o parâmetro passado corresponde a URI do documento XML que descreve o dispositivo.
                             * @param uri URI a ser comparada.
                             * @return true, caso o parâmetro passado corresponda a URI do documento XML, ou false, caso contrário.
                             */
                            bool isDescriptionURI(const char* uri);

                            /**
                             * Carrega o documento de descrição do dispositivo a partir do parâmetro passado.
                             * @param desc descrição XML do dispositivo.
                             * @return true, caso a descrição XML esteja correta, ou false, caso contrário.
                             */
                            bool loadDescription(const char* desc);

                            /**
                             * Carrega o documento de descrição do dispositivo a partir do parâmetro passado (nó XML).
                             * @param rootNode descrição XML do dispositivo.
                             * @return true, caso a descrição XML esteja correta, ou false, caso contrário.
                             */
                            bool loadDescription(NodePtr rootNode);

                            /**
                             * Retorna o documento XML de descrição do dispositivo.
                             * @return descrição XML do dispositivo.
                             */
                            const char* getDescription();

                            /**
                             * Configura a localização do dispositivo.
                             * @param location localização do dispositivo (na forma http://ip:porta).
                             */
                            void setLocation(const char* location);

                            /**
                             * Retorna a localização do dispositivo.
                             * @return localização do dispositivo (na forma http://ip:porta).
                             */
                            const char* getLocation();

                            /**
                             * Configura o tipo do dispositivo.
                             * @param type tipo do dispositivo.
                             */
                            void setDeviceType(const char* type);

                            /**
                             * Retorna o tipo do dispositivo.
                             * @return tipo do dispositivo.
                             */
                            const char* getDeviceType();

                            /**
                             * Diz se o dispositivo é do tipo passado no parâmetro.
                             * @param type tipo do dispositivo a ser comparado.
                             * @return true, caso seja o mesmo tipo de dispositivo, ou false, caso contrário.
                             */
                            bool isDeviceType(const char* type);

                            /**
                             * Configura o nome amigável do dispositivo.
                             * @param name nome amigável.
                             */
                            void setFriendlyName(const char* name);

                            /**
                             * Retorna o nome amigável do dispositivo.
                             * @return nome amigável.
                             */
                            const char* getFriendlyName();

                            /**
                             * Configura o nome do fabricante do dispositivo.
                             * @param value nome do fabricante.
                             */
                            void setManufacture(const char* value);

                            /**
                             * Retorna o nome do fabricante do dispositivo.
                             * @return nome do fabricante.
                             */
                            const char* getManufacture();

                            /**
                             * Configura a URL do fabricante do dispositivo.
                             * @param url URL do fabricante.
                             */
                            void setManufactureURL(const char* url);

                            /**
                             * Retorna a URL do fabricante do dispositivo.
                             * @return URL do fabricante.
                             */
                            const char* getManufactureURL();

                            /**
                             * Configura a descrição do modelo do dispositivo.
                             * @param desc descrição do modelo.
                             */
                            void setModelDescription(const char* desc);

                            /**
                             * Retorna a descrição do modelo do dispositivo.
                             * @return descrição do modelo.
                             */
                            const char* getModelDescription();

                            /**
                             * Configura o nome do modelo do dispositivo.
                             * @param name nome do modelo.
                             */
                            void setModelName(const char* name);

                            /**
                             * Retorna o nome do modelo do dispositivo.
                             * @return nome do modelo.
                             */
                            const char* getModelName();

                            /**
                             * Configura o número do modelo do dispositivo.
                             * @param number número do modelo.
                             */
                            void setModelNumber(const char* number);

                            /**
                             * Retorna o número do modelo do dispositivo.
                             * @return número do modelo.
                             */
                            const char* getModelNumber();

                            /**
                             * Configura a URL do modelo do dispositivo.
                             * @param url URL do modelo.
                             */
                            void setModelURL(const char* url);

                            /**
                             * Retorna a URL do modelo do dispositivo.
                             * @return URL do modelo.
                             */
                            const char* getModelURL();

                            /**
                             * Configura o número de série do dispositivo.
                             * @param number número de série.
                             */
                            void setSerialNumber(const char* number);

                            /**
                             * Retorna o número de série do dispositivo.
                             * @return número de série.
                             */
                            const char* getSerialNumber();

                            /**
                             * Configura um identificador numérico único para o dispositivo.
                             * @param udn identificador na forma uuid:HHHH-HHHH-HHHH-HHHH, onde H é um número hexadecimal.
                             */
                            void setUDN(const char* udn);

                            /**
                             * Retorna o identificador numérico único do dispositivo.
                             * @return identificador na forma uuid:HHHH-HHHH-HHHH-HHHH, onde H é um número hexadecimal.
                             */
                            const char* getUDN();

                            /**
                             * Diz se o dispositivo possui um identificador numérico único.
                             * @return true, caso o dispositivo possua um identificador numérico único, ou false, caso contrário.
                             */
                            bool hasUDN();

                            /**
                             * Configura o código universal do produto (12 dígitos numéricos), atribuído pelo <em>Uniform Code Council</em>.
                             * @param upc código universal do produto.
                             */
                            void setUPC(const char* upc);

                            /**
                             * Retorna o código universal do produto (12 dígitos numéricos).
                             * @return código universal do produto.
                             */
                            const char* getUPC();

                            /**
                             * Configura a URL da página HTML de apresentação/controle do dispositivo.
                             * @param url URL da página de apresentação.
                             */
                            void setPresentationURL(const char* url);

                            /**
                             * Retorna a URL da página HTML de apresentação/controle do dispositivo.
                             * @return URL da página de apresentação.
                             */
                            const char* getPresentationURL();

                            /**
                             * Retorna a URL principal do dispositivo.
                             * @return URL principal do dispositivo.
                             */
                            const char* getURLBase();

                            /**
                             * Retorna a lista dos dispositivos aninhados.
                             * @return lista dos dispositivos aninhados.
                             */
                            UPnPDeviceList* getEmbeddedDevices();

                            /**
                             * Verifica se o parâmetro passado corresponde ao dispositivo.
                             * @param name identificador numérico, nome amigável ou tipo do dispositivo a ser comparado.
                             * @return true, caso o parâmetro passado corresponda ao dispositivo, ou false, caso contrário.
                             */
                            bool isUPnPDevice(const char* name);

                            /**
                             * Retorna um dispositivo da lista dos dispositivos aninhados pelo identificador numérico, nome amigável ou tipo do dispositivo.
                             * @param name identificador numérico, nome amigável ou tipo do dispositivo.
                             * @return dispositivo aninhado.
                             */
                            UPnPDevice* getUPnPDevice(const char* name);

                            /**
                             * Retorna um dispositivo da lista dos dispositivos aninhados pela URI do documento que o descreve.
                             * @param uri URI do documento que descreve o dispositivo.
                             * @return dispositivo aninhado.
                             */
                            UPnPDevice* getUPnPDeviceByDescriptionURI(const char* uri);

                            /**
                             * Retorna a lista de serviços disponibilizados pelo dispositivo.
                             * @return lista de serviços.
                             */
                            UPnPServiceList* getUPnPServices();

                            /**
                             * Retorna um serviço do dispositivo ou de algum dispositivo aninhado pelo tipo ou identificador do serviço.
                             * @param name tipo ou identificador do serviço.
                             * @return serviço.
                             */
                            UPnPService* getUPnPService(const char* name);

                            /**
                             * Retorna um serviço do dispositivo ou de algum dispositivo aninhado pela URL do documento que descreve o serviço.
                             * @param url URL do documento que descreve o serviço.
                             * @return serviço.
                             */
                            UPnPService* getUPnPServiceBySCPDURL(const char* url);

                            /**
                             * Retorna um serviço do dispositivo ou de algum dispositivo aninhado pela URL que controla o serviço.
                             * @param url URL que controla o serviço.
                             * @return serviço.
                             */
                            UPnPService* getUPnPServiceByControlURL(const char* url);

                            /**
                             * Retorna um serviço do dispositivo ou de algum dispositivo aninhado pela URL que recebe subscrições para os eventos do serviço.
                             * @param url URL que recebe subscrições para os eventos do serviço.
                             * @return serviço.
                             */
                            UPnPService* getUPnPServiceByEventSubURL(const char* url);

                            /**
                             * Retorna uma variável de estado do dispositivo ou de algum dispositivo aninhado pelo tipo ou identificador do serviço e nome da variável.
                             * @param servType tipo ou identificador do serviço.
                             * @param name nome da variável.
                             * @return variável de estado.
                             */
                            UPnPStateVariable* getUPnPStateVariable(const char* servType, const char* name);

                            /**
                             * Retorna uma variável de estado do dispositivo ou de algum dispositivo aninhado pelo nome.
                             * @param name nome da variável.
                             * @return variável de estado.
                             */
                            UPnPStateVariable* getUPnPStateVariable(const char* name);

                            /**
                             * Envia uma notificão de alteração em uma variável de estado aos dispositivos registrados.
                             * @param var variável de estado alterada.
                             */
                            void sendEventMessage(UPnPStateVariable* var);

                            /**
                             * Retorna uma ação do dispositivo ou de algum dispositivo aninhado pelo nome.
                             * @param name nome da ação.
                             * @return ação.
                             */
                            UPnPAction* getUPnPAction(const char* name);

                            /**
                             * Retorna a lista de ícones do dispositivo.
                             * @return lista de ícones.
                             */
                            UPnPIconList* getUPnPIcons();

                            /**
                             * Retorna um ícone do dispositivo pelo seu índice.
                             * @param n índice.
                             * @return ícone.
                             */
                            UPnPIcon* getUPnPIcon(int n);

                            /**
                             * Inicializa os serviços do dispositivo e os disponibiliza na rede.
                             * @return true, caso a ação ocorra com sucesso, ou false, caso contrário.
                             */
                            bool start();

                            /**
                             * Pára os serviços do dispositivo na rede.
                             * @return true, caso a ação ocorra com sucesso, ou false, caso contrário.
                             */
                            bool stop();

                            /**
                             * Configura o <em>listener</em> para ações do dispositivo.
                             * @param listener <em>listener</em> de ações.
                             */
                            void setUPnPActionListener(UPnPActionListener* listener);

                            /**
                             * Configura o <em>listener</em> para consultas ao dispositivo.
                             * @param listener <em>listener</em> de consultas.
                             */
                            void setUPnPQueryListener(UPnPQueryListener* listener);

                            /**
                             * Configura o <em>listener</em> para ações do dispositivo e seus aninhados.
                             * @param listener <em>listener</em> de ações.
                             * @param embeddedDevices configurar o mesmo <em>listener</em> para os dispositivos aninhados.
                             */
                            void setUPnPActionListener(UPnPActionListener* listener, bool embeddedDevices);

                            /**
                             * Configura o <em>listener</em> para consultas ao dispositivo e seus aninhados.
                             * @param listener <em>listener</em> de consultas.
                             * @param embeddedDevices configurar o mesmo <em>listener</em> para os dispositivos aninhados.
                             */
                            void setUPnPQueryListener(UPnPQueryListener* listener, bool embeddedDevices);

                            /**
                             * Restaura o identificador numérico único do dispositivo a partir de um arquivo.
                             * @param path caminho do arquivo com identificador na forma HHHH-HHHH-HHHH-HHHH, onde H é um número hexadecimal
                             */
                            void restoreUUID(const char* path);

                            /**
                             * Configura o identificador numérico único do dispositivo.
                             * @param uuid identificador na forma HHHH-HHHH-HHHH-HHHH, onde H é um número hexadecimal
                             */
                            void setUUID(const char* uuid);

                            /**
                             * Retorna o identificador numérico único do dispositivo.
                             * @return identificador na forma HHHH-HHHH-HHHH-HHHH, onde H é um número hexadecimal
                             */
                            const char* getUUID();

                            /**
                             * Retorna a lista de dispositivos principais em execução.
                             * @return lista de dispositivos principais.
                             */
                            static UPnPDeviceList* getUPnPLocalRootDeviceInstances();

                        private:

                            /**
                             * Atribui um nó XML descrevendo o dispositivo.
                             * @param node nó XML.
                             */
                            void setNode(NodePtr node);

                            /**
                             * Retorna o nó XML que descreve o dispositivo.
                             * @return nó XML.
                             */
                            NodePtr getNode();

                            /**
                             * Atribui um nó XML descrevendo o dispositivo principal.
                             * @param node nó XML.
                             */
                            void setRootNode(NodePtr node);

                            /**
                             * Configura o dispositivo principal ao qual está associado.
                             * @param root dispositivo principal.
                             */
                            void setUPnPRootDevice(UPnPDevice* root);

                            /**
                             * Configura a URL principal do dispositivo.
                             * @param url URL principal do dispositivo.
                             */
                            void setURLBase(const char* url);

                            /**
                             * Inicializa as listas de dispositivos aninhados, de serviços e ícones do dispositivo.
                             */
                            void initializeLists();

                            /**
                             * Inicializa a lista de dispositivos aninhados.
                             */
                            void initializeDeviceList();

                            /**
                             * Inicializa a lista de serviços do dispositivo.
                             */
                            void initializeServiceList();

                            /**
                             * Inicializa a lista de ícones do dispositivo.
                             */
                            void initializeIconList();

                        };

                    }
                }
            }
        }
    }
}

#endif	/* _UPNPDEVICE_H */

