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

#ifndef _BTDEVICECONTROLLER_H
#define	_BTDEVICECONTROLLER_H

#include <pthread.h>
#include "BtDevice.h"
#include "BtDeviceList.h"
#include "BtDeviceListener.h"
#include "Vector.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
using namespace br::ufscar::lince::ginga::hones::util;
using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace idservice {

                        class BtDeviceController {
                        private:
                            static BtDeviceController* instance;
                            bool running;
                            BtDeviceList deviceList;
                            Vector deviceListeners;
                            pthread_t thread;
                            HLoggerPtr logger;

                            BtDeviceController();
                            ~BtDeviceController();

                            /**
                             * Configura o estado de execução do controlador de dispositivo.
                             * @param running true, o controlador de dispositivo está ativo, ou false, caso contrário.
                             */
                            void setRunning(bool running);

                            /**
                             * Método executado em paralelo, responsável pela busca e adição/removação de dispositivos.
                             */
                            static void* run(void* param);

                        public:

                            /**
                             * Retorna a instância do controlador de dispositivos.
                             * @return instância do controlador de dispositivos.
                             */
                            static BtDeviceController* getInstance();

                            /**
                             * Destrói a instância do controlador de dispositivos.
                             */
                            static void deleteInstance();

                            /**
                             * Verifica se o controlador de dispositivos está ativo.
                             * @return true, caso o controlador de dispositivos esteja ativo, ou false, caso contrário.
                             */
                            bool isRunning();

                            /**
                             * Ativa o controlador de dispositivos.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool start();

                            /**
                             * Pára o controlador de dispositivos.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool stop();

                            /**
                             * Adiciona um dispositivo a lista dos dispositivos controlados.
                             * @param device dispositivo.
                             */
                            void addBtDevice(BtDevice* device);

                            /**
                             * Remove um dispositivo da lista dos dispositivos controlados.
                             * @param device dispositivo.
                             */
                            void removeBtDevice(BtDevice* device);

                            /**
                             * Remove um dispositivo da lista dos dispositivos controlados pelo identificador ou nome amigável.
                             * @param name identificador ou nome amigável.
                             */
                            void removeBtDevice(const char* name);

                            /**
                             * Retorna a lista dos dispositivos controlados.
                             * @return lista dos dispositivos controlados.
                             */
                            BtDeviceList* getBtDeviceList();

                            /**
                             * Retorna um dispositivo da lista dos dispositivos controlados pelo identificador ou nome amigável.
                             * @param name identificador ou nome amigável.
                             * @return dispositivo.
                             */
                            BtDevice* getBtDevice(const char* name);

                            /**
                             * Verifica se um dispositivo faz parte da lista dos dispositivos controlados pelo identificador ou nome amigável.
                             * @param name identificador ou nome amigável.
                             * @return true, caso o dispositivo faça parte da lista, ou false, caso contrário.
                             */
                            bool hasBtDevice(const char* name);

                            /**
                             * Adiciona um <em>listener</em> de dispositivo ao controlador.
                             * @param deviceListener <em>listener</em> de dispositivo.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool addBtDeviceListener(BtDeviceListener* deviceListener);

                            /**
                             * Remove um <em>listener</em> de dispositivo do controlador.
                             * @param deviceListener <em>listener</em> de dispositivo.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool removeBtDeviceListener(BtDeviceListener* deviceListener);

                        };

                    }
                }
            }
        }
    }
}

#endif	/* _BTDEVICECONTROLLER_H */

