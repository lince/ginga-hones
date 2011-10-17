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

#ifndef _UPNPDEVICELIST_H
#define	_UPNPDEVICELIST_H

#include <string>
#include "Utils.h"
#include "Vector.h"
using namespace br::ufscar::lince::ginga::hones::util;
using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {
                        class UPnPDevice;
                        class UPnPService;
                        class UPnPStateVariable;
                        class UPnPAction;
                        class UPnPActionListener;
                        class UPnPQueryListener;

                        class UPnPDeviceList : public Vector {
                        private:
                            static const string ELEMENT_NAME;
                            
                        public:

                            UPnPDeviceList();
                            ~UPnPDeviceList();

                            /**
                             * Limpa a lista de dispositivos.
                             */
                            void clear();

                            /**
                             * Diz se o nó XML descreve uma lista de dispositivos.
                             * @param node nó XML.
                             * @return true, caso o nó XML descreva uma lista de dispositivos, ou false, caso contrário.
                             */
                            static bool isUPnPDeviceListNode(NodePtr node);

                            /**
                             * Retorna um dispositivo da lista pelo seu índice.
                             * @param i índice do dispositivo.
                             * @return dispositivo.
                             */
                            UPnPDevice* getUPnPDevice(int i);

                            /**
                             * Retorna um dispositivo da lista ou aninhado pelo identificador numérico, nome amigável ou tipo do dispositivo.
                             * @param name identificador numérico, nome amigável ou tipo do dispositivo.
                             * @return dispositivo.
                             */
                            UPnPDevice* getUPnPDevice(const char* name);

                            /**
                             * Retorna um dispositivo da lista ou aninhado pela URI do documento que o descreve.
                             * @param uri URI do documento que descreve o dispositivo.
                             * @return dispositivo.
                             */
                            UPnPDevice* getUPnPDeviceByDescriptionURI(const char* uri);

                            /**
                             * Retorna um serviço de um dispositivo da lista ou aninhado pelo tipo ou identificador do serviço.
                             * @param name tipo ou identificador do serviço.
                             * @return serviço.
                             */
                            UPnPService* getUPnPService(const char* name);

                            /**
                             * Retorna um serviço de um dispositivo da lista ou aninhado pela URL do documento que descreve o serviço.
                             * @param url URL do documento que descreve o serviço.
                             * @return serviço.
                             */
                            UPnPService* getUPnPServiceBySCPDURL(const char* url);

                            /**
                             * Retorna um serviço de um dispositivo da lista ou aninhado pela URL que controla o serviço.
                             * @param url URL que controla o serviço.
                             * @return serviço.
                             */
                            UPnPService* getUPnPServiceByControlURL(const char* url);

                            /**
                             * Retorna um serviço de um dispositivo da lista ou aninhado pela URL que recebe subscrições para os eventos do serviço.
                             * @param url URL que recebe subscrições para os eventos do serviço.
                             * @return serviço.
                             */
                            UPnPService* getUPnPServiceByEventSubURL(const char* url);

                            /**
                             * Retorna uma variável de estado de um dispositivo da lista ou aninhado pelo nome.
                             * @param name nome da variável.
                             * @return variável de estado.
                             */
                            UPnPStateVariable* getUPnPStateVariable(const char* name);

                            /**
                             * Retorna uma ação de um dispositivo da lista pelo seu nome.
                             * @param name nome da ação.
                             * @return ação.
                             */
                            UPnPAction* getUPnPAction(const char* name);

                            /**
                             * Configura o <em>listener</em> para ações dos dispositivos e seus aninhados.
                             * @param listener <em>listener</em> de ações.
                             * @param embeddedDevices configurar o mesmo <em>listener</em> para os dispositivos aninhados.
                             */
                            void setUPnPActionListener(UPnPActionListener* listener, bool embeddedDevices);

                            /**
                             * Configura o <em>listener</em> para consultas aos dispositivos e seus aninhados.
                             * @param listener <em>listener</em> de consultas.
                             * @param embeddedDevices configurar o mesmo <em>listener</em> para os dispositivos aninhados.
                             */
                            void setUPnPQueryListener(UPnPQueryListener* listener, bool embeddedDevices);
                        };
                    }
                }
            }
        }
    }
}

#endif	/* _UPNPDEVICELIST_H */

