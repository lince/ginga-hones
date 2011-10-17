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

#ifndef _IMAGECAPTURE_H
#define	_IMAGECAPTURE_H

#include <string>
#include <vector>
#include <sstream>
#include <ffmpeg_ginga.h>

#include "OutputStream.h"

using namespace std;
using namespace br::ufscar::lince::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace capture {

                        class ImageCapture {
                        private:
                            int width;
                            int height;
                            string imageFormat;
                            string destination;
                            vector<OutputStream *> outputStreams;
                            static ImageCapture* instance;
                            static GingaFFMpeg ffmpegCapture;

                        public:

                            /**
                             * Factory
                             * Devolve uma instância de ImageCapture.
                             * @return instância de ImageCapture.
                             */
                            static ImageCapture* ImageCaptureFactory();

                            /**
                             * Destrói a instância de ImageCapture.
                             */
                            static void ImageCaptureDestructor();

                            /**
                             * Faz a captura de imagem do framebuffer conforme os parâmetros préviamente configurados.
                             * Caso não possa escrever no arquivo de destino, lança exception (FileWriteException)
                             * Caso não possa escrever no fluxo de saída (OutputStream) lança exception (StreamWriteException)
                             * Caso não possa escrever no formato informado (jpg, png, etc) lança exception (EncondingException)
                             * @return true, caso a ação ocorra com sucesso, ou false, caso contrário.
                             */
                            bool capture() throw();

                            /**
                             * Configura a largura (em pixels) da imagem a ser capturada.
                             * @param width largura da imagem destino.
                             */
                            void setWidth(int width);

                            /**
                             * Retorna a largura (em pixels) da imagem a ser capturada.
                             * @return largura da imagem destino.
                             */
                            int getWidth();

                            /**
                             * Configura a altura (em pixels) da imagem a ser capturada.
                             * @param height altura da imagem destino.
                             */
                            void setHeight(int height);

                            /**
                             * Retorna a altura (em pixels) da imagem a ser capturada.
                             * @return altura da imagem destino.
                             */
                            int getHeight();

                            /**
                             * Configura o formato da imagem a ser capturada.
                             * @param imageFormat formato da imagem destino (possíveis valores: png, jpeg).
                             */
                            void setImageFormat(const char* imageFormat);

                            /**
                             * Retorna o formato da imagem a ser capturada.
                             * @return formato da imagem destino.
                             */
                            const char* getImageFormat();

                            /**
                             * Configura o destino (arquivo local) da imagem a ser capturada.
                             * Caso não seja especificado um destino, o método write() deve ser usado.
                             * Caso não consiga reconhecer a URI lança exception (URIException)
                             * @param destination destino da imagem (possíveis valores: "/home/user/file.jpg", "").
                             */
                            void setDestination(const char* destination) throw();

                            /**
                             * Retorna o destino da imagem a ser capturada.
                             * @return destino da imagem.
                             */
                            const char* getDestination();

                            /**
                             * Adiciona um fluxo de saída (OutputStream) à captura.
                             * Quando adicionado algum fluxo de saída (OutputStream), o método write() deve ser usado.
                             * @param stream fluxo de saída.
                             * @return true, se a ação for executada com sucesso, ou false, caso contrário.
                             */
                            bool addOutputStream(OutputStream* stream);

                            /**
                             * Remove um fluxo de saída (OutputStream) da captura.
                             * @param stream fluxo de saída.
                             * @return true, se a ação for executada com sucesso, ou false, caso contrário.
                             */
                            bool removeOutputStream(OutputStream* stream);

                        private:

                            /**
                             * Construtor.
                             */
                            ImageCapture();

                            /**
                             * Destrutor.
                             */
                            ~ImageCapture();

                            /**
                             * Percorre o vetor de fluxos de saída e envia os bytes da imagem que está sendo capturada para cada fluxo (OutputStream).
                             * Este método somente é usado caso não seja especificado um destino para a imagem a ser capturada.
                             * @param b um byte da imagem que está sendo capturada.
                             */
                            void write(int b);

                            /**
                             * Percorre o vetor de fluxos de saída e envia os bytes da imagem que está sendo capturada para cada fluxo (OutputStream).
                             * Este método somente é usado caso não seja especificado um destino para a imagem a ser capturada.
                             * @param b origem dos dados.
                             * @param len quantidade de bytes da origem.
                             * @return quantidade de bytes escritos ou -1 caso ocorra erro.
                             */
                            int write(const char* b, int len);
                            
                        };

                    }
                }
            }
        }
    }
}

#endif	/* _IMAGECAPTURE_H */
