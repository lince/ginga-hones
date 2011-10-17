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

#ifndef _TVRRENDERINGCONTROL_H
#define	_TVRRENDERINGCONTROL_H

#include <stdlib.h>
#include "UPnPActionListener.h"
#include "UPnPQueryListener.h"
#include "UPnPService.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
using namespace std;
using namespace br::ufscar::lince::ginga::hones::upnp;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediarenderer {

                        class TVRenderer;

                        class TVRRenderingControl : public UPnPActionListener, public UPnPQueryListener {
                        private:
                            TVRenderer *tvRender;
                            HLoggerPtr logger;

                        public:

                            static const char *SERVICE_TYPE;

                            static const char *SCPD;

                            static const char *INSTANCE_ID;
                            static const char *PRESET_NAME_LIST;
                            static const char *CURRENT_PRESET_NAME_LIST;
                            static const char *PRESET_NAME;
                            static const char *MUTE;
                            static const char *CURRENT_MUTE;
                            static const char *CHANNEL;
                            static const char *MASTER;
                            static const char *DESIRED_MUTE;
                            static const char *VOLUME;
                            static const char *CURRENT_VOLUME;
                            static const char *DESIRED_VOLUME;
                            static const char *CURRENT_CONTRAST;
                            static const char *CONTRAST;
                            static const char *DESIRED_CONTRAST;
                            static const char *CURRENT_BRIGHTNESS;
                            static const char *BRIGHTNESS;
                            static const char *DESIRED_BRIGHTNESS;

                            static const char *LIST_PRESETS;
                            static const char *SELECT_PRESET;
                            static const char *GET_MUTE;
                            static const char *SET_MUTE;
                            static const char *GET_VOLUME;
                            static const char *SET_VOLUME;
                            static const char *GET_CONTRAST;
                            static const char *SET_CONTRAST;
                            static const char *GET_BRIGHTNESS;
                            static const char *SET_BRIGHTNESS;


                            TVRRenderingControl(TVRenderer *tvr);

                            ~TVRRenderingControl();

                            /**
                             * Inicializa as variáveis de estado necessárias ao funcionamento do serviço gerenciador de apresentação.
                             */
                            void initializeStateVariables();

                            /**
                             * Retorna a instância do serviço de apresentação.
                             * @return serviço de apresentação.
                             */
                            TVRenderer* getMediaRenderer();

                            /**
                             * Retorna a instância do serviço gerenciador de apresentação.
                             * @return gerenciador de apresentação.
                             */
                            UPnPService *getService();

                            /**
                             * Recebe solicitações de ações a serem executadas.
                             * @param action ação a ser executada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionRequest(UPnPAction *action);

                            /**
                             * Retorna uma lista de <em>presets</em>, associados ao identificador <em>InstanceID</em>, separados por vírgula.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *
                             * Argumentos de saída da ação:
                             *      - CurrentPresetNameList
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionListPresets(UPnPAction *action);

                            /**
                             * Seleciona um <em>preset</em> associado ao identificador <em>InstanceID</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *      - PresetName
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionSelectPreset(UPnPAction *action);

                            /**
                             * Diz se o volume de um canal associado ao identificador <em>InstanceID</em> está mudo.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *      - Channel
                             *
                             * Argumentos de saída da ação:
                             *      - CurrentMute
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetMute(UPnPAction *action);

                            /**
                             * Configura o volume de um canal associado ao identificador <em>InstanceID</em> para mudo ou não.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *      - Channel
                             *      - DesiredMute
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionSetMute(UPnPAction *action);

                            /**
                             * Retorna o nível do volume de um canal associado ao identificador <em>InstanceID</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *      - Channel
                             *
                             * Argumentos de saída da ação:
                             *      - CurrentVolume
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetVolume(UPnPAction *action);

                            /**
                             * Configura o nível do volume de um canal associado ao identificador <em>InstanceID</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *      - Channel
                             *      - DesiredVolume
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionSetVolume(UPnPAction *action);

                            /**
                             * Retorna o valor do contraste associado ao identificador <em>InstanceID</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *
                             * Argumentos de saída da ação:
                             *      - CurrentContrast
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetContrast(UPnPAction *action);

                            /**
                             * Configura o valor do contraste associado ao identificador <em>InstanceID</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *      - DesiredContrast
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionSetContrast(UPnPAction *action);

                            /**
                             * Retorna o valor do brilho associado ao identificador <em>InstanceID</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *
                             * Argumentos de saída da ação:
                             *      - CurrentBrightness
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetBrightness(UPnPAction *action);

                            /**
                             * Configura o valor do brilho associado ao identificador <em>InstanceID</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *      - DesiredBrightness
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionSetBrightness(UPnPAction *action);

                            /**
                             * Envia o estado atual de uma variável do serviço.
                             * @param stateVar variável solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool variableRequest(UPnPStateVariable *stateVar);

                        private:
                            /**
                             * Configura a instância do serviço de apresentação.
                             * @param tvr nova instância.
                             */
                            void setMediaRenderer(TVRenderer *tvr);

                        };

                    }
                }
            }
        }
    }
}

#endif	/* _TVRRENDERINGCONTROL_H */

