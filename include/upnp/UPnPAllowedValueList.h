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

#ifndef _UPNPALLOWEDVALUELIST_H
#define	_UPNPALLOWEDVALUELIST_H

#include <string>
#include "Utils.h"
#include "Vector.h"
#include "UPnPAllowedValue.h"
using namespace br::ufscar::lince::ginga::hones::util;
using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        class UPnPAllowedValueList : public Vector {
                        private:
                            static const string ELEMENT_NAME;
                            
                        public:

                            UPnPAllowedValueList();
                            ~UPnPAllowedValueList();

                            /**
                             * Limpa a lista de valores permitidos a um argumento UPnP.
                             */
                            void clear();

                            /**
                             * Diz se o nó XML descreve uma lista de valores permitidos a um argumento UPnP.
                             * @param node nó XML.
                             * @return true, caso o nó XML descreva uma lista de valores permitidos, ou false, caso contrário.
                             */
                            static bool isUPnPAllowedValueListNode(NodePtr node);

                            /**
                             * Retorna um valor permitido a um argumento UPnP da lista pelo seu índice.
                             * @param i índice.
                             * @return valor permitido.
                             */
                            UPnPAllowedValue* getUPnPAllowedValue(int i);
                        };
                    }
                }
            }
        }
    }
}

#endif	/* _UPNPALLOWEDVALUELIST_H */

