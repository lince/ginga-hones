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

#ifndef _DIRECTORY_H
#define	_DIRECTORY_H

#include <string>
#include "FileList.h"
#include "DirectoryList.h"
using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediaserver {

                        class TVMSContentDirectory;

                        class Directory {
                        private:
                            string name;
                            string path;
                            string id;
                            char* parentId;
                            FileList fileList;
                            DirectoryList directoryList;
                            
                        public:

                            Directory(TVMSContentDirectory* contentDirectory, const char* path);
                            ~Directory();

                            /**
                             * Retorna o nome do diretório.
                             * @return nome.
                             */
                            const char* getName();

                            /**
                             * Configura o nome do diretório.
                             * @param name nome.
                             */
                            void setName(const char* name);

                            /**
                             * Configura o caminho do diretório.
                             * @param path caminho.
                             */
                            void setPath(const char* path);

                            /**
                             * Retorna o caminho do diretório.
                             * @return caminho.
                             */
                            const char* getPath();

                            /**
                             * Configura o identificador do diretório.
                             * @param id identificador.
                             */
                            void setID(const char* id);

                            /**
                             * Retorna o identificador do diretório.
                             * @return identificador.
                             */
                            const char* getID();

                            /**
                             * Configura o identificador do diretório pai.
                             * @param id identificador.
                             */
                            void setParentID(const char* id);

                            /**
                             * Retorna o identificador do diretório pai.
                             * @return identificador.
                             */
                            const char* getParentID();

                            /**
                             * Retorna uma lista com todos os arquivos do diretório.
                             * @return lista de arquivos.
                             */
                            FileList* getFiles();

                            /**
                             * Retorna uma lista com todos os subdiretórios.
                             * @return lista de subdiretórios.
                             */
                            DirectoryList* getDirectories();

                            /**
                             * Diz se o diretório está vazio.
                             * @return true, caso o diretório esteja vázio, ou false, caso contrário.
                             */
                            bool isEmpty();

                            /**
                             * Retorna um arquivo pelo seu identificador.
                             * @param id identificador do arquivo.
                             * @return arquivo.
                             */
                            File* getFileByID(const char* id);

                            /**
                             * Retorna um subdiretório pelo seu identificador.
                             * @param id identificador do subdiretório.
                             * @return subdiretório.
                             */
                            Directory* getDirectoryByID(const char* id);

                        };

                    }
                }
            }
        }
    }
}

#endif	/* _DIRECTORY_H */

