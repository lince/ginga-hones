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

#ifndef _PLAYERHANDLER_H
#define	_PLAYERHANDLER_H

#include <string>
#include <pthread.h>
#include "GraphicsHandler.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace ui {

                        class PlayerHandler {
                        private:
                            void* avProvider;
                            void* imgProvider;
                            void* win;
                            void* font;
                            void* surface;
                            void* buffer;
                            HLoggerPtr logger;
                            string uri, metadata, tmpFile, prevURI, prevFile;
                            int width, height;
                            bool running;
                            pthread_t thread;
                            pthread_t* streamingThread;
                            bool hasVisualContent;
                            bool mediaReady;
                            static PlayerHandler* instance;

                        public:

                            /**
                             * Factory.
                             * Devolve uma instância de PlayerHandler.
                             * @return instância de PlayerHandler.
                             */
                            static PlayerHandler* PlayerHandlerFactory();

                            /**
                             * Destrói a instância de PlayerHandler.
                             */
                            static void PlayerHandlerDestructor();

                            /**
                             * Verifica se existe mídia sendo reproduzida.
                             * @return true, caso alguma mídia esteja sendo reproduzida, ou false, caso contrário.
                             */
                            bool isRunning();

                            /**
                             * Configura o caminho da mídia.
                             * @param uri caminho da mídia.
                             */
                            void setURI(string uri);

                            /**
                             * Configura os metadados da mídia.
                             * @param metadata metadados da mídia.
                             */
                            void setMetadata(string metadata);

                            /**
                             * Configura a área para reprodução da mídia.
                             * @param surface área para reprodução da mídia.
                             */
                            void setPlaybackSurface(void* surface);

                            /**
                             * Prepara a mídia para reprodução.
                             */
                            void prepareMedia();

                            /**
                             * Reproduz a mídia.
                             * @return true caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool play();

                            /**
                             * Pára a reprodução da mídia.
                             * @return true caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool stop();

                            /**
                             * Coloca a reprodução da mídia em modo "pause".
                             * @return true caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool pause();

                            /**
                             * Desloca no tempo a reprodução da mídia.
                             * @param pos nova posição (em segundos).
                             * @return true caso a nova posição seja configurada com sucesso, ou false, caso contrário.
                             */
                            bool seek(double pos);

                            /**
                             * Configura o nível do áudio.
                             * @param v novo volume (0 - 100).
                             * @return true caso o novo valor seja configurado com sucesso, ou false, caso contrário.
                             */
                            bool setVolume(int v);

                            /**
                             * Obtém o nível (0 - 100) do áudio.
                             * @return volume.
                             */
                            int getVolume();

                            /**
                             * Obtém a posição (tempo) atual de reprodução da mídia.
                             * @return posição atual da mídia (em segundos).
                             */
                            double getMediaTimePosition();

                            /**
                             * Obtém a duração (tempo) total da mídia.
                             * @return duração da mídia (em segundos).
                             */
                            double getMediaDuration();

                            /**
                             * Configura o brilho da mídia em reprodução.
                             * @param v novo brilho (0 - 100).
                             * @return true caso configure o novo valor do brilho com sucesso, ou false, caso contrário.
                             */
                            bool setBrightness(int v);

                            /**
                             * Obtém o valor (0 - 100) do brilho da mídia em reprodução.
                             * @return valor do brilho.
                             */
                            int getBrightness();

                            /**
                             * Configura o valor do contraste da mídia em reprodução.
                             * @param v novo valor do contraste (0 - 100).
                             * @return true caso o novo valor seja configurado com sucesso, ou false, caso contrário.
                             */
                            bool setContrast(int v);

                            /**
                             * Obtém o valor (0 - 100) do contraste da mídia em reprodução.
                             * @return valor do contraste.
                             */
                            int getContrast();

                            /**
                             * Método executado em paralelo que pára a reprodução quando a mídia atinge seu fim.
                             */
                            void run();

                        private:

                            /**
                             * Construtor
                             */
                            PlayerHandler();

                            /**
                             * Destrutor
                             */
                            ~PlayerHandler();
                            
                            /**
                             * Configura e inicializa a janela de reprodução da mídia de acordo com a largura e altura da mesma.
                             */
                            void initializeWindow();

                            /**
                             * Configura a variável que indica se uma mídia está sendo reproduzida.
                             * @param running true, caso alguma mídia esteja sendo reproduzida, ou false, caso contrário.
                             */
                            void setRunning(bool running);

                        };
                    
                    }
                }
            }
        }
    }
}

#endif	/* _PLAYERHANDLER_H */

