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

#ifndef _UPNPARGUMENT_H
#define	_UPNPARGUMENT_H

#include <string>
#include "Utils.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
using namespace std;
using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {
                        class UPnPStateVariable;
                        class UPnPService;

                        class UPnPArgument {
                        private:
                            NodePtr node;
                            UPnPService* service;
                            string argValue;
                            HLoggerPtr logger;
                            static const string ELEMENT_NAME;
                            static const string IN_DIRECTION;
                            static const string OUT_DIRECTION;
                            static const string NAME;
                            static const string DIRECTION;
                            static const string RELATED_STATE_VARIABLE;

                        public:

                            UPnPArgument();
                            UPnPArgument(NodePtr node);
                            UPnPArgument(const char* name, const char* value);
                            ~UPnPArgument();

                            /**
                             * Retorna o serviço UPnP ao qual está associado o argumento.
                             * @return serviço.
                             */
                            UPnPService* getUPnPService();

                            /**
                             * Associa o argumento a um serviço UPnP.
                             * @param service serviço ao qual estará associado o argumento.
                             */
                            void setUPnPService(UPnPService* service);

                            /**
                             * Diz se o nó XML passado como parâmetro descreve um argumento UPnP.
                             * @param node nó XML.
                             * @return true, caso o nó XML descreva um argumento, ou false, caso contrário.
                             */
                            static bool isUPnPArgumentNode(NodePtr node);

                            /**
                             * Configura o nome do argumento.
                             * @param value nome do argumento.
                             */
                            void setName(const char* value);

                            /**
                             * Retorna o nome do argumento.
                             * @return nome do argumento.
                             */
                            const char* getName();

                            /**
                             * Configura a direção do argumento (entrada ou saída).
                             * @param value direção do argumento ( in / out ).
                             */
                            void setDirection(const char* value);

                            /**
                             * Retorna a direção do argumento (entrada ou saída).
                             * @return direção do argumento ( in / out ).
                             */
                            const char* getDirection();

                            /**
                             * Diz se o argumento é de entrada.
                             * @return true, caso o argumento seja de entrada, ou false, caso contrário.
                             */
                            bool isInDirection();

                            /**
                             * Diz se o argumento é de saída.
                             * @return true, caso o argumento seja de saída, ou false, caso contrário.
                             */
                            bool isOutDirection();

                            /**
                             * Associa o argumento a uma variável de estado UPnP.
                             * @param value nome da variável de estado UPnP.
                             */
                            void setUPnPRelatedStateVariableName(const char* value);

                            /**
                             * Retorna o nome da variável de estado UPnP a qual está associado o argumento.
                             * @return nome da variável de estado UPnP.
                             */
                            const char* getUPnPRelatedStateVariableName();

                            /**
                             * Retorna a variável de estado UPnP a qual está associado o argumento.
                             * @return variável de estado UPnP.
                             */
                            UPnPStateVariable* getUPnPRelatedStateVariable();

                            /**
                             * Configura o valor do argumento.
                             * @param value novo valor.
                             */
                            void setValue(const char *value);

                            /**
                             * Configura o valor (inteiro) do argumento.
                             * @param value novo valor.
                             */
                            void setValue(int value);

                            /**
                             * Retorna o valor do argumento.
                             * @return valor do argumento.
                             */
                            const char* getValue();

                            /**
                             * Retorna o valor (inteiro) do argumento.
                             * @return valor do argumento.
                             */
                            int getIntegerValue();

                        private:

                            /**
                             * Atribui um nó XML descrevendo um argumento UPnP.
                             * @param node nó XML.
                             */
                            void setNode(NodePtr node);

                            /**
                             * Retorna o nó XML que descreve um argumento UPnP.
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

#endif	/* _UPNPARGUMENT_H */

