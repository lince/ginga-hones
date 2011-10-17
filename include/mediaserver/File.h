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

#ifndef _FILE_H
#define	_FILE_H

#include <string>
#include <time.h>
#include <fcntl.h>
using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediaserver {

                        class File {
                        private:
                            string name;
                            char* type;
                            const char* upnpClass;
                            char* dlnaAdditionalInfo;
                            char *duration, *title, *author, *album, *genre, *comment;
                            int bitrate, channels, samplerate, width, height, track, year;
                            string path;
                            string id;
                            string parentId;
                            string suffix;
                            off_t length;
                            bool readable;
                            bool directory;
                            time_t lastModified;

                        public:

                            File(const char* path);
                            ~File();

                            /**
                             * Retorna o nome do arquivo.
                             * @return nome.
                             */
                            const char* getName();

                            /**
                             * Configura o nome do arquivo.
                             * @param name novo nome.
                             */
                            void setName(const char* name);

                            /**
                             * Retorna o <em>MIME Type</em> do arquivo.
                             * @return <em>MIME Type</em>.
                             */
                            const char* getMimeType();

                            /**
                             * Retorna a classe UPnP do arquivo.
                             * @return classe UPnP.
                             */
                            const char* getUPnPClass();

                            /**
                             * Retorna informações DLNA.
                             * @return informações DLNA.
                             */
                            const char* getDLNAAdditionalInfo();

                            /**
                             * Retorna a extensão do arquivo.
                             * @return extensão.
                             */
                            const char* getSuffix();

                            /**
                             * Configura o caminho do arquivo.
                             * @param path caminho do arquivo.
                             */
                            void setPath(const char* path);

                            /**
                             * Retorna o caminho do arquivo.
                             * @return caminho.
                             */
                            const char* getPath();

                            /**
                             * Configura o identificador do arquivo.
                             * @param id identificador.
                             */
                            void setID(const char* id);

                            /**
                             * Retorna o identificador do arquivo.
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
                             * Retorna o tamanho, em bytes, do arquivo.
                             * @return tamanho.
                             */
                            off_t getLength();

                            /**
                             * Diz se o arquivo pode ser lido.
                             * @return true, se o arquivo pode ser lido, ou false, caso contrário.
                             */
                            bool isReadable();

                            /**
                             * Diz se é um diretório.
                             * @return true, se é um diretório, ou false, caso contrário.
                             */
                            bool isDirectory();

                            /**
                             * Retorna a data da última modificação.
                             * @return data de alteração.
                             */
                            const time_t getLastModified();

                            /**
                             * Diz se é um arquivo de áudio.
                             * @return true, se é um arquivo de áudio, ou false, caso contrário.
                             */
                            bool isAudio();

                            /**
                             * Diz se é um arquivo de vídeo.
                             * @return true, se é um arquivo de vídeo, ou false, caso contrário.
                             */
                            bool isVideo();

                            /**
                             * Diz se é um arquivo de imagem.
                             * @return true, se é um arquivo de imagem, ou false, caso contrário.
                             */
                            bool isImage();

                            /**
                             * Retorna o tempo de duração da mídia.
                             * @return duração.
                             */
                            const char* getDuration();

                            /**
                             * Retorna o título da mídia.
                             * @return título.
                             */
                            const char* getTitle();

                            /**
                             * Retorna o autor da mídia.
                             * @return autor.
                             */
                            const char* getAuthor();

                            /**
                             * Retorna o albúm da mídia.
                             * @return albúm.
                             */
                            const char* getAlbum();

                            /**
                             * Retorna o gênero da mídia.
                             * @return gênero.
                             */
                            const char* getGenre();

                            /**
                             * Retorna comentários/descrição da mídia.
                             * @return comentários/descrição.
                             */
                            const char* getComment();

                            /**
                             * Retorna a taxa de bytes por segundo.
                             * @return taxa de bytes.
                             */
                            int getBitrate();

                            /**
                             * Retorna a quantidade de canais (mono, estéreo) da mídia.
                             * @return quantidade de canais.
                             */
                            int getChannels();

                            /**
                             * Retorna a taxa de amostragem da mídia.
                             * @return taxa de amostragem.
                             */
                            int getSamplerate();

                            /**
                             * Retorna a largura de uma imagem/vídeo.
                             * @return largura.
                             */
                            int getWidth();

                            /**
                             * Retorna a altura de uma imagem/vídeo.
                             * @return altura.
                             */
                            int getHeight();

                            /**
                             * Retorna a faixa original de uma mídia.
                             * @return faixa.
                             */
                            int getTrack();

                            /**
                             * Retorna a data de gravação da mídia.
                             * @return data de gravação.
                             */
                            int getYear();

                        private:

                            /**
                             * Recupera as propriedades da mídia.
                             */
                            void initializeMediaProperties();

                        };

                    }
                }
            }
        }
    }
}

#endif	/* _FILE_H */

