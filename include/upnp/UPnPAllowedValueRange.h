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

#ifndef _UPNPALLOWEDVALUERANGE_H
#define	_UPNPALLOWEDVALUERANGE_H

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

                        class UPnPAllowedValueRange {
                        private:
                            NodePtr node;
                            static const string ELEMENT_NAME;
                            static const string MAXIMUM;
                            static const string MINIMUM;
                            static const string STEP;

                        public:

                            UPnPAllowedValueRange();
                            UPnPAllowedValueRange(NodePtr node);
                            ~UPnPAllowedValueRange();

                            /**
                             * Diz se o nó XML passado como parâmetro descreve uma faixa de valores permitida a um argumento UPnP.
                             * @param node nó XML.
                             * @return true, caso o nó XML descreva uma faixa de valores permitida a um argumento UPnP, ou false, caso contrário.
                             */
                            static bool isUPnPAllowedValueRangeNode(NodePtr node);

                            /**
                             * Configura o menor valor permitido a um argumento UPnP.
                             * @param value menor valor permitido.
                             */
                            void setMinimumValue(const char *value);

                            /**
                             * Retorna o menor valor permitido a um argumento UPnP.
                             * @return menor valor permitido.
                             */
                            const char* getMinimumValue();

                            /**
                             * Configura o maior valor permitido a um argumento UPnP.
                             * @param value maior valor permitido.
                             */
                            void setMaximumValue(const char *value);

                            /**
                             * Retorna o maior valor permitido a um argumento UPnP.
                             * @return maior valor permitido.
                             */
                            const char* getMaximumValue();

                            /**
                             * Configura o valor permitido para cada passo de um argumento UPnP.
                             * @param value valor permitido para cada passo.
                             */
                            void setStepValue(const char *value);

                            /**
                             * Retorna o valor permitido para cada passo de um argumento UPnP.
                             * @return valor permitido para cada passo.
                             */
                            const char* getStepValue();

                            /**
                             * Verifica se a faixa de valores permitidos está configurada.
                             * @return true, se a faixa de valores permitidos está configurada, ou false, caso contrário.
                             */
                            bool hasValueRange();

                        private:

                            /**
                             * Atribui um nó XML descrevendo uma faixa de valores permitida a um argumento UPnP.
                             * @param node nó XML.
                             */
                            void setNode(NodePtr node);

                            /**
                             * Retorna o nó XML que descreve uma faixa de valores permitida a um argumento UPnP.
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

#endif	/* _UPNPALLOWEDVALUERANGE_H */

