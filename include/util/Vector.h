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

#ifndef _HONES_VECTOR_H
#define	_HONES_VECTOR_H

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace util {
                        class Vector {
                            private:
                                void* vectorPtr;

                            public:
                                Vector();

                                /**
                                 * Adiciona um objeto ao vetor.
                                 * @param object objeto a ser inserido.
                                 * @return true, caso o objeto seja inserido com sucesso, ou false, caso contrário.
                                 */
                                bool add(void *object);

                                /**
                                 * Remove um objeto ao vetor.
                                 * @param object objeto a ser removido.
                                 * @return true, caso o objeto seja removido com sucesso, ou false, caso contrário.
                                 */
                                bool remove(void *object);

                                /**
                                 * Retorna o índice do objeto no vetor.
                                 * @param object objeto a ser pesquisado.
                                 * @return índice do objeto, caso o objeto seja encontrado, ou -1, caso contrário.
                                 */
                                int indexOf(void *object);

                                /**
                                 * Retorna um objeto do vetor de acordo com seu índice.
                                 * @param index índice do objeto.
                                 * @return objeto, caso o índice seja válido, ou NULL, caso contrário.
                                 */
                                void *get(int index);

                                /**
                                 * Adiciona um objeto ao vetor em uma determinada posição.
                                 * @param object objeto a ser inserido.
                                 * @param index posição no vetor.
                                 * @return true, caso o objeto seja inserido com sucesso, ou false, caso contrário.
                                 */
                                bool insertAt(void *object, int index);

                                /**
                                 * Retorna o tamanho do vetor.
                                 * @return tamanho do vetor.
                                 */
                                int size();

                                /**
                                 * Remove todos os elementos do vetor.
                                 */
                                void clear();

                            private:
                                /**
                                 * Remove todos os elementos do vetor recursivamente.
                                 */
                                void clear(void* elemPtr);
                        };
                    }
                }
            }
        }
    }
}

#endif	/* _HONES_VECTOR_H */

