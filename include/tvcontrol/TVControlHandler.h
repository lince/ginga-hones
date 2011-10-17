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

#ifndef _TVCONTROLHANDLER_H
#define	_TVCONTROLHANDLER_H

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace tvcontrol {

                        class TVControlHandler {
                        private:
                            static TVControlHandler* instance;

                        public:

                            /**
                             * Factory.
                             * Devolve uma instância de TVControlHandler.
                             * @return instância de TVControlHandler.
                             */
                            static TVControlHandler* TVControlHandlerFactory();

                            /**
                             * Destrói a instância de TVControlHandler.
                             */
                            static void TVControlHandlerDestructor();

                            /**
                             * Sintoniza o canal posterior ao sintonizado.
                             * @return true, caso a ação ocorra com sucesso, ou false, caso contrário.
                             */
                            bool channelUp();

                            /**
                             * Sintoniza o canal anterior ao sintonizado.
                             * @return true, caso a ação ocorra com sucesso, ou false, caso contrário.
                             */
                            bool channelDown();

                            /**
                             * Defini o canal a ser sintonizado.
                             * @param channel canal a ser sintonizado.
                             * @return true, caso a ação ocorra com sucesso, ou false, caso contrário.
                             */
                            bool setChannel(int channel);

                            /**
                             * Obtém o canal sintonizado.
                             * @return canal sintonizado.
                             */
                            int getChannel();

                            /**
                             * Configura o nível do áudio.
                             * @param v novo volume (0 - 100).
                             * @return true caso o novo valor seja configurado com sucesso, ou false, caso contrário.
                             */
                            bool setVolume(int v);

                            /**
                             * Configura o brilho da TV.
                             * @param v novo brilho (0 - 100).
                             * @return true caso configure o novo valor do brilho com sucesso, ou false, caso contrário.
                             */
                            bool setBrightness(int v);

                            /**
                             * Obtém o valor (0 - 100) do brilho da TV.
                             * @return valor do brilho.
                             */
                            int getBrightness();

                            /**
                             * Configura o valor do contraste da TV.
                             * @param v novo valor do contraste (0 - 100).
                             * @return true caso o novo valor seja configurado com sucesso, ou false, caso contrário.
                             */
                            bool setContrast(int v);

                            /**
                             * Obtém o valor (0 - 100) do contraste da TV.
                             * @return valor do contraste.
                             */
                            int getContrast();

                            /**
                             * Configura o valor da saturação da TV.
                             * @param v novo valor da saturação (0 - 100).
                             * @return true caso o novo valor seja configurado com sucesso, ou false, caso contrário.
                             */
                            bool setSaturation(int v);

                            /**
                             * Obtém o valor (0 - 100) da saturação da TV.
                             * @return valor da saturação.
                             */
                            int getSaturation();

                            /**
                             * Configura o valor da matiz da TV.
                             * @param v novo valor da matiz (0 - 100).
                             * @return true caso o novo valor seja configurado com sucesso, ou false, caso contrário.
                             */
                            bool setHue(int v);

                            /**
                             * Obtém o valor (0 - 100) da matiz da TV.
                             * @return valor da matiz.
                             */
                            int getHue();

                        private:

                            /**
                             * Construtor.
                             */
                            TVControlHandler();

                            /**
                             * Destrutor.
                             */
                            ~TVControlHandler();

                        };

                    }
                }
            }
        }
    }
}

#endif	/* _TVCONTROLHANDLER_H */
