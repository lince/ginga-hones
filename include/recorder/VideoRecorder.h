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

#ifndef _VIDEORECORDER_H
#define	_VIDEORECORDER_H

#include <string>
#include <vector>
#include <pthread.h>
#include "ScheduledItem.h"
using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace recorder {

                        class VideoRecorder {
                        private:
                            static VideoRecorder* instance;
                            vector<ScheduledItem *> scheduledItems;
                            pthread_t thread;
                            bool running;

                        public:

                            /**
                             * Factory.
                             * Devolve uma instância de VideoRecorder.
                             * @return instância de VideoRecorder.
                             */
                            static VideoRecorder* VideoRecorderFactory();

                            /**
                             * Destrói a instância de VideoRecorder.
                             */
                            static void VideoRecorderDestructor();

                            /**
                             * Adiciona um agendamento de gravação de vídeo.
                             * @param item agendamento de gravação de vídeo.
                             */
                            void addScheduledItem(ScheduledItem* item);

                            /**
                             * Remove um agendamento de gravação de vídeo.
                             * Caso não exista o agendamento informado lança exception (ItemNotFoundException)
                             * @param item agendamento de gravação de vídeo.
                             */
                            void removeScheduledItem(ScheduledItem* item);

                            /**
                             * Retorna os agendamentos de gravação de vídeo.
                             * @return agendamentos de gravação de vídeo.
                             */
                            vector<ScheduledItem*> getScheduledItems();

                            /**
                             * Verifica se alguma instância de VideoRecorder está em execução.
                             * @return true, caso alguma instância de VideoRecorder esteja em execução, ou false, caso contrário.
                             */
                            bool isRunning();

                        private:

                            /**
                             * Construtor.
                             */
                            VideoRecorder();

                            /**
                             * Destrutor.
                             */
                            ~VideoRecorder();

                            /**
                             * Inicia o gravador de vídeo conforme os parâmetros préviamente configurados.
                             * Caso não possa escrever em algum arquivo de destino, lança exception (FileWriteException)
                             * Caso não possa escrever em algum formato informado (jpg, png, etc) lança exception (EncondingException)
                             * @return true, caso a ação ocorra com sucesso, ou false, caso contrário.
                             */
                            bool start();

                            /**
                             * Pára o gravador de vídeo.
                             * @return true, caso a ação ocorra com sucesso, ou false, caso contrário.
                             */
                            bool stop();

                            /**
                             * Armazena/carrega as informações do VideoRecorder em/do arquivo.
                             */
                            void load();

                            /**
                             * Coloca em execução as gravações de vídeo.
                             * Deve ser executado por uma thread paralela.
                             * @param param alguma parâmetro que será utilizado pela thread ou NULL.
                             * @return NULL, caso a ação seja executada com sucesso, ou alguma instância de um objeto, caso necessário.
                             */
                            static void* createVideoRecorderThread(void* param);

                            /**
                             * Cria uma thread para interromper cada gravação de vídeo.
                             * @param param ScheduledItem.
                             * @return NULL, caso a ação seja executada com sucesso, ou alguma instância de um objeto, caso necessário.
                             */
                            static void* createScheduledItemThread(void* param);
                            
                        };

                    }
                }
            }
        }
    }
}

#endif	/* _VIDEORECORDER_H */
