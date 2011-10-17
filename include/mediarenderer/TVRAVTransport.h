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

#ifndef _TVRAVTRANSPORT_H
#define	_TVRAVTRANSPORT_H

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

                        class TVRAVTransport : public UPnPActionListener, public UPnPQueryListener {
                        private:
                            TVRenderer *tvRender;
                            HLoggerPtr logger;

                        public:

                            static const char *SERVICE_TYPE;

                            static const char *SCPD;

                            static const char *INSTANCE_ID;
                            static const char *CURRENT_URI;
                            static const char *CURRENT_URI_METADATA;
                            static const char *NR_TRACKS;
                            static const char *MEDIA_DURATION;
                            static const char *NEXT_URI;
                            static const char *NEXT_URI_METADATA;
                            static const char *PLAY_MEDIUM;
                            static const char *RECORD_MEDIUM;
                            static const char *WRITE_STATUS;
                            static const char *NOT_IMPLEMENTED;
                            static const int NOT_IMPLEMENTED_I4;
                            static const char *NONE;
                            static const char *OK;
                            static const char *NETWORK;
                            static const char *CURRENT_TRANSPORT_STATE;
                            static const char *CURRENT_TRANSPORT_STATUS;
                            static const char *CURRENT_SPEED;
                            static const char *STOPPED;
                            static const char *PLAYING;
                            static const char *TRANSITIONING;
                            static const char *PAUSED_PLAYBACK;
                            static const char *PAUSED_RECORDING;
                            static const char *RECORDING;
                            static const char *NO_MEDIA_PRESENT;
                            static const char *TRACK;
                            static const char *TRACK_DURATION;
                            static const char *TRACK_METADATA;
                            static const char *TRACK_URI;
                            static const char *REL_TIME;
                            static const char *ABS_TIME;
                            static const char *REL_COUNT;
                            static const char *ABS_COUNT;
                            static const char *PLAY_MEDIA;
                            static const char *REC_MEDIA;
                            static const char *REC_QUALITY_MODES;
                            static const char *NORMAL;
                            static const char *SHUFFLE;
                            static const char *REPEAT_ONE;
                            static const char *REPEAT_ALL;
                            static const char *RANDOM;
                            static const char *DIRECT_1;
                            static const char *INTRO;
                            static const char *PLAY_MODE;
                            static const char *REC_QUALITY_MODE;
                            static const char *UNIT;
                            static const char *TARGET;

                            static const char *PLAYBACK_STORAGE_MEDIUM;
                            static const char *AVTRANSPORT_URI;
                            static const char *AVTRANSPORT_URI_METADATA;
                            static const char *CURRENT_MEDIA_DURATION;
                            static const char *NUMBER_OF_TRACKS;
                            static const char *TRANSPORT_STATE;
                            static const char *TRANSPORT_STATUS;
                            static const char *TRANSPORT_PLAY_SPEED;
                            static const char *CURRENT_TRACK;
                            static const char *CURRENT_TRACK_DURATION;
                            static const char *CURRENT_TRACK_METADATA;
                            static const char *CURRENT_TRACK_URI;
                            static const char *RELATIVE_TIME_POSITION;
                            static const char *ABSOLUTE_TIME_POSITION;
                            static const char *RELATIVE_COUNTER_POSITION;
                            static const char *ABSOLUTE_COUNTER_POSITION;
                            static const char *POSSIBLE_PLAYBACK_STORAGE_MEDIA;
                            static const char *POSSIBLE_RECORD_STORAGE_MEDIA;
                            static const char *POSSIBLE_RECORD_QUALITY_MODES;
                            static const char *CURRENT_PLAY_MODE;
                            static const char *CURRENT_RECORD_QUALITY_MODE;

                            static const char *SET_AVTRANSPORT_URI;
                            static const char *GET_MEDIA_INFO;
                            static const char *GET_TRANSPORT_INFO;
                            static const char *GET_POSITION_INFO;
                            static const char *GET_DEVICE_CAPABILITIES;
                            static const char *GET_TRANSPORT_SETTINGS;
                            static const char *STOP;
                            static const char *PLAY;
                            static const char *SEEK;
                            static const char *NEXT;
                            static const char *PREVIOUS;
                            static const char *PAUSE;


                            TVRAVTransport(TVRenderer *tvr);

                            ~TVRAVTransport();

                            /**
                             * Inicializa as variáveis de estado necessárias ao funcionamento do serviço gerenciador de transferência de mídia.
                             */
                            void initializeStateVariables();

                            /**
                             * Retorna a instância do serviço de apresentação.
                             * @return serviço de apresentação.
                             */
                            TVRenderer* getMediaRenderer();

                            /**
                             * Retorna a instância do serviço gerenciador de transferência de mídia.
                             * @return gerenciador de transferência de mídia.
                             */
                            UPnPService* getService();

                            /**
                             * Recebe solicitações de ações a serem executadas.
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionRequest(UPnPAction *action);

                            /**
                             * Configura a <em>URI</em> da mídia a ser controlada pelo gerenciador de transferência de mídia.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *      - CurrentURI
                             *      - CurrentURIMetaData    \n<em>DIDL-Lite MetaData da URI</em>
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionSetAVTransportURI(UPnPAction *action);

                            /**
                             * Acessa informações relativas a uma mídia associada ao identificador <em>InstanceID</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *
                             * Argumentos de saída da ação:
                             *      - NrTracks              \n<em>Número de faixas</em>
                             *      - MediaDuration         \n<em>Duração da mídia no formato HH:MM:SS (ex.: "10:00" )</em>
                             *      - CurrentURI            \n<em>URI atual</em>
                             *      - CurrentURIMetaData    \n<em>DIDL-Lite MetaData da URI</em>
                             *      - NextURI
                             *      - NextURIMetaData
                             *      - PlayMedium
                             *      - RecordMedium
                             *      - WriteStatus
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetMediaInfo(UPnPAction *action);

                            /**
                             * Acessa informações relativas ao serviço gerenciador de transferência de mídia especificado pelo identificador <em>InstanceID</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *
                             * Argumentos de saída da ação:
                             *      - CurrentTransportState
                             *      - CurrentTransportStatus
                             *      - CurrentSpeed
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetTransportInfo(UPnPAction *action);

                            /**
                             * Acessa informações sobre a posição atual (tempo de reprodução) de uma mídia associada ao identificador <em>InstanceID</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *
                             * Argumentos de saída da ação:
                             *      - Track
                             *      - TrackDuration
                             *      - TrackMetaData
                             *      - TrackURI
                             *      - RelTime
                             *      - AbsTime
                             *      - RelCount
                             *      - AbsCount
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetPositionInfo(UPnPAction *action);

                            /**
                             * Acessa informações relativas a capacidade do dispositivo, tais como reprodução, formatos e qualidade de gravação suportados.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *
                             * Argumentos de saída da ação:
                             *      - PlayMedia
                             *      - RecMedia
                             *      - RecQualityModes
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetDeviceCapabilities(UPnPAction *action);

                            /**
                             * Acessa informações relativas a uma mídia associada ao identificador <em>InstanceID</em>, tais como modo de reprodução e qualidade de gravação.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *
                             * Argumentos de saída da ação:
                             *      - PlayMode         \n<em>NORMAL, SHUFFLE, REPEAT_ONE, REPEAT_ALL, RANDOM,
                             *                       DIRECT_1, ou INTRO</em>
                             *      - RecQualityMode   \n<em>0:EP, 1:LP, 2:SP, 0:BASIC, 1:MEDIUM,
                             *                       2:HIGH, ou NOT_IMPLEMENTED</em>
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetTransportSettings(UPnPAction *action);

                            /**
                             * Pára a reprodução de uma mídia associada ao identificador <em>InstanceID</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionStop(UPnPAction *action);

                            /**
                             * Inícia/reinicia a reprodução de uma mídia associada ao identificador <em>InstanceID</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionPlay(UPnPAction *action);

                            /**
                             * Configura a posição atual (tempo de reprodução) de uma mídia associada ao identificador <em>InstanceID</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *      - Unit
                             *      - Target
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionSeek(UPnPAction *action);

                            /**
                             * Avança para a próxima faixa de uma mídia associada ao identificador <em>InstanceID</em>.
                             * Equivalente a <em>Seek(TRACK_NR,CurrentTrackNr+1)</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionNext(UPnPAction *action);

                            /**
                             * Volta para a faixa anterior de uma mídia associada ao identificador <em>InstanceID</em>.
                             * Equivalente a <em>Seek(TRACK_NR,CurrentTrackNr-1)</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionPrevious(UPnPAction *action);

                            /**
                             * Coloca em modo <em>pause</em> a reprodução de uma mídia associada ao identificador <em>InstanceID</em>.
                             *
                             * Argumentos de entrada da ação:
                             *      - InstanceID
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionPause(UPnPAction *action);

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

#endif	/* _TVRAVTRANSPORT_H */

