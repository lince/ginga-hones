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

#ifndef _UPNPHTTPLISTENER_H
#define	_UPNPHTTPLISTENER_H

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace upnp {

                        struct UPnPHttpFileHandle {
                            off_t pos;

                            enum {
                                FILE_LOCAL,
                                FILE_MEMORY
                            } type;

                            struct {
                                int fd;
                                void* file;
                            } local;

                            struct {
                                char *contents;
                                off_t len;
                            } memory;
                        };
                        
                        enum OpenFileMode {
                            READ,
                            WRITE
                        };

                        class UPnPHttpListener {
                        public:

                            /**
                             * Recupera informações sobre um arquivo.
                             * @param filename nome do arquivo que se deseja obter informações (entrada).
                             * @param fileLength tamanho do arquivo.
                             * @param lastModified data da última alteração.
                             * @param isDirectory diz se é um diretório.
                             * @param isReadable diz se pode ser lido.
                             * @param contentType tipo do arquivo (MIME type).
                             * @param absolutePath caminho completo do arquivo no sistema.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            virtual bool httpGetInfo(const char* filename, off_t* fileLength, time_t* lastModified, bool* isDirectory, bool* isReadable, char** contentType, char** absolutePath) = 0;

                            /**
                             * Abre um arquivo para leitura/escrita.
                             * @param filename nome do arquivo que se deseja abrir.
                             * @param mode modo da operação (READ ou WRITE).
                             * @return manipulador do arquivo.
                             */
                            virtual UPnPHttpFileHandle* httpOpen(const char* filename, enum OpenFileMode mode) = 0;

                            /**
                             * Realiza a leitura de um arquivo.
                             * @param fh manipulador do arquivo.
                             * @param buf destino dos dados lidos.
                             * @param buflen capacidade de armazenamento do destino.
                             * @return quantidade de bytes lidos ou -1 caso ocorra erro.
                             */
                            virtual off_t httpRead(UPnPHttpFileHandle* fh, char* buf, off_t buflen) = 0;

                            /**
                             * Realiza a escrita em um arquivo.
                             * @param fh manipulador do arquivo.
                             * @param buf origem dos dados.
                             * @param buflen quantidade de bytes da origem.
                             * @return quantidade de bytes escritos ou -1 caso ocorra erro.
                             */
                            virtual off_t httpWrite(UPnPHttpFileHandle* fh, char* buf, off_t buflen) = 0;

                            /**
                             * Desloca o cursor para uma determinada posição no arquivo.
                             * @param fh manipulador do arquivo.
                             * @param offset deslocamento em bytes.
                             * @param origin posição inicial do deslocamento (SEEK_SET, SEEK_CUR ou SEEK_END).
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            virtual bool httpSeek(UPnPHttpFileHandle* fh, off_t offset, int origin) = 0;

                            /**
                             * Encerra a leitura/escrita de um arquivo.
                             * @param fh manipulador do arquivo.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            virtual bool httpClose(UPnPHttpFileHandle* fh) = 0;
                        };

                    }
                }
            }
        }
    }
}

#endif	/* _UPNPHTTPLISTENER_H */

