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

#ifndef _GRAPHICSHANDLER_H
#define	_GRAPHICSHANDLER_H

#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace ui {

                    class GraphicsHandler {

                    private:

                        static HLoggerPtr logger;
                        static int screenWidth;
                        static int screenHeight;
                        
                    public:

                        /**
                         * Inicializa o ambiente gráfico.
                         */
                        static void initialize();

                        /**
                         * Encerra o ambiente gráfico.
                         */
                        static void release();

                        /**
                         * Retorna a classe principal do ambiente gráfico.
                         * @return classe principal do ambiente gráfico.
                         */
                        static void* getGraphicsRoot();

                        /**
                         * Retorna a largura da tela em pixels.
                         * @return largura da tela em pixels.
                         */
                        static int getScreenWidth();

                        /**
                         * Retorna a altura da tela em pixels.
                         * @return altura da tela em pixels.
                         */
                        static int getScreenHeight();

                        /**
                         * Retorna um reprodutor de áudio/vídeo para o endereço passado.
                         * @param uri endereço da mídia a ser tocada.
                         * @return reprodutor de áudio/vídeo.
                         */
                        static void* createAVProvider(char* uri);

                        /**
                         * Retorna um reprodutor de imagem para o endereço passado.
                         * @param uri endereço da mídia a ser tocada.
                         * @return reprodutor de imagem.
                         */
                        static void* createImageProvider(char* uri);

                        /**
                         * Constrói uma janela no ambiente gráfico de acordo com a descrição passada.
                         * @param dsc descrição da janela a ser criada.
                         * @return instância da janela.
                         */
                        static void* createFrame(void* dsc);

                    };
                    
                    }
                }
            }
        }
    }
}

#endif	/* _GRAPHICSHANDLER_H */

