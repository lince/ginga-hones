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

#ifndef _UPNPICON_H
#define	_UPNPICON_H

#include <string>
#include "Utils.h"
using namespace std;
using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        class UPnPIcon {
                        private:
                            NodePtr node;
                            static const string ELEMENT_NAME;
                            static const string MIME_TYPE;
                            static const string WIDTH;
                            static const string HEIGHT;
                            static const string DEPTH;
                            static const string URL;

                        public:

                            UPnPIcon(NodePtr node);
                            ~UPnPIcon();

                            /**
                             * Diz se o nó XML passado como parâmetro descreve um ícone.
                             * @param node nó XML.
                             * @return true, caso o nó XML descreva um ícone, ou false, caso contrário.
                             */
                            static bool isUPnPIconNode(NodePtr node);

                            /**
                             * Configura o tipo do arquivo (<em>MIME type</em>) que representa o ícone.
                             * @param value tipo do arquivo (<em>MIME type</em>) que representa o ícone.
                             */
                            void setMimeType(const char* value);

                            /**
                             * Retorna o tipo do arquivo (<em>MIME type</em>) que representa o ícone.
                             * @return tipo do arquivo (<em>MIME type</em>).
                             */
                            const char* getMimeType();

                            /**
                             * Configura a largura da imagem que representa o ícone.
                             * @param value largura da imagem (pixels).
                             */
                            void setWidth(const char* value);

                            /**
                             * Retorna a largura da imagem que representa o ícone.
                             * @return largura da imagem (pixels).
                             */
                            const char* getWidth();

                            /**
                             * Configura a altura da imagem que representa o ícone.
                             * @param value altura da imagem (pixels).
                             */
                            void setHeight(const char* value);

                            /**
                             * Retorna a altura da imagem que representa o ícone.
                             * @return altura da imagem (pixels).
                             */
                            const char* getHeight();

                            /**
                             * Configura a intensidade de cores da imagem que representa o ícone.
                             * @param value intensidade (bits).
                             */
                            void setDepth(const char* value);

                            /**
                             * Retorna a intensidade de cores da imagem que representa o ícone.
                             * @return intensidade (bits).
                             */
                            const char* getDepth();

                            /**
                             * Configura a URL de acesso à imagem que representa o ícone.
                             * @param value URL de acesso à imagem que representa o ícone.
                             */
                            void setURL(const char* value);

                            /**
                             * Retorna a URL de acesso à imagem que representa o ícone.
                             * @return URL de acesso à imagem que representa o ícone.
                             */
                            const char* getURL();

                        private:

                            /**
                             * Atribui um nó XML descrevendo o ícone.
                             * @param node nó XML.
                             */
                            void setNode(NodePtr node);

                            /**
                             * Retorna o nó XML que descreve o ícone.
                             * @return nó XML.
                             */
                            NodePtr getNode();

                        };
                    }
                }
            }
        }
    }
}

#endif	/* _UPNPICON_H */

