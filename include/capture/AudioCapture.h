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

#ifndef _AUDIOCAPTURE_H
#define	_AUDIOCAPTURE_H

#include <string>
#include <vector>
#include <pthread.h>
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

                        class AudioCapture {
                        private:
                            int sampleRate;
                            int audioBitrate;
                            int audioChannels;
                            bool isStreaming;
                            bool running;
                            string audioFormat;
                            string audioCodec;
                            vector<string *> destinations;
                            vector<OutputStream *> outputStreams;
                            pthread_t thread;
                            static AudioCapture* instance;
                            static GingaFFMpeg ffmpegCapture;

                        public:

                            /**
                             * Factory.
                             * Devolve uma instância de AudioCapture.
                             * @return instância de AudioCapture.
                             */
                            static AudioCapture* AudioCaptureFactory();

                            /**
                             * Destrói a instância de AudioCapture.
                             */
                            static void AudioCaptureDestructor();

                            /**
                             * Configura a taxa de amostragem do áudio a ser capturado.
                             * @param sampleRate taxa de amostragem do áudio destino (valor padrão: 44100).
                             */
                            void setSampleRate(int sampleRate);

                            /**
                             * Retorna a taxa de amostragem do áudio a ser capturado.
                             * @return taxa de amostragem do áudio destino.
                             */
                            int getSampleRate();

                            /**
                             * Configura a taxa de bits (bit/s) do áudio a ser capturado.
                             * @param audioBitrate taxa de bits do áudio destino (valor padrão: 64000).
                             */
                            void setAudioBitrate(int audioBitrate);

                            /**
                             * Retorna a taxa de bits (bit/s) do áudio a ser capturado.
                             * @return taxa de bits do áudio destino.
                             */
                            int getAudioBitrate();

                            /**
                             * Configura a quantidade de canais do áudio a ser capturado.
                             * @param audioChannels quantidade de canais do áudio destino (valor padrão: 1).
                             */
                            void setAudioChannels(int audioChannels);

                            /**
                             * Retorna a quantidade de canais do áudio a ser capturado.
                             * @return quantidade de canais do áudio destino.
                             */
                            int getAudioChannels();

                            /**
                             * Configura o formato do áudio a ser capturado.
                             * @param audioFormat formato do áudio destino (possíveis valores: mp2, mp3, ogg, rtp, ...).
                             */
                            void setAudioFormat(const char* audioFormat);

                            /**
                             * Retorna o formato do áudio a ser capturado.
                             * @return formato do áudio destino.
                             */
                            const char* getAudioFormat();

                            /**
                             * Configura o codec (compressão) do áudio a ser capturado.
                             * @param audioCodec codec do áudio destino (possíveis valores: mp2, mp3, vorbis, aac, ...).
                             */
                            void setAudioCodec(const char* audioCodec);

                            /**
                             * Retorna o codec (compressão) do áudio a ser capturado.
                             * @return codec do áudio destino.
                             */
                            const char* getAudioCodec();

                            /**
                             * Adiciona um destino (arquivo local ou rede) do áudio a ser capturado.
                             * Caso não possa escrever no arquivo de destino, lança exception (FileWriteException)
                             * Caso não possa escrever no fluxo de saída (OutputStream) lança exception (StreamWriteException)
                             * Caso não possa escrever no formato informado (mp2, mp3, ogg, etc) lança exception (EncondingException)
                             * Caso não possa enviar streaming pela rede lança exception (NetworkingException)
                             * Caso não consiga reconhecer a URI lança exception (URIException)
                             * @param destination destino do áudio (possíveis valores: "/home/user/file.mp3", "rtp://239.255.255.250:5123").
                             */
                            void addDestination(const char* destination) throw();

                            /**
                             * Remove um destino (arquivo local ou rede) do áudio a ser capturado.
                             * Caso não exista o destino informado lança exception (URIException)
                             * @param destination destino do áudio (possíveis valores: "/home/user/file.mp3", "rtp://239.255.255.250:5123").
                             */
                            void removeDestination(const char* destination) throw();

                            /**
                             * Adiciona um conjunto de destinos (arquivo local ou rede) do áudio a ser capturado.
                             * Caso não possa escrever no arquivo de destino, lança exception (FileWriteException)
                             * Caso não possa escrever no fluxo de saída (OutputStream) lança exception (StreamWriteException)
                             * Caso não possa escrever no formato informado (mp2, mp3, ogg, etc) lança exception (EncondingException)
                             * Caso não possa enviar streaming pela rede lança exception (NetworkingException)
                             * Caso não consiga reconhecer a URI lança exception (URIException)
                             * @param destination conjunto de destinos do áudio (possíveis valores: "/home/user/file.mp3", "rtp://239.255.255.250:5123").
                             */
                            void addDestination(vector<const char*> destination) throw();

                            /**
                             * Remove um conjunto de destinos (arquivo local ou rede) do áudio a ser capturado.
                             * Caso não consiga reconhecer a URI lança exception (URIException)
                             * @param destination conjunto de destinos do áudio (possíveis valores: "/home/user/file.mp3", "rtp://239.255.255.250:5123").
                             */
                            void removeDestination(vector<const char*> destination) throw();

                            /**
                             * Retorna os destinos do áudio a ser capturado.
                             * @return destinos do áudio.
                             */
                            vector<const char*> getDestinations();

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

                            /**
                             * Verifica se existe alguma captura em execução.
                             * @return true, caso alguma captura esteja sendo executada, ou false, caso contrário.
                             */
                            bool isRunning();

                        private:

                            /**
                             * Construtor.
                             */
                            AudioCapture();

                            /**
                             * Destrutor.
                             */
                            ~AudioCapture();

                            /**
                             * Inicia a captura de áudio conforme os parâmetros préviamente configurados.
                             * Caso não possa escrever no arquivo de destino, lança exception (FileWriteException)
                             * Caso não possa escrever no fluxo de saída (OutputStream) lança exception (StreamWriteException)
                             * Caso não possa escrever no formato informado (jpg, png, etc) lança exception (EncondingException)
                             * Caso não possa enviar streaming pela rede lança exception (NetworkingException)
                             * @return true, caso a ação ocorra com sucesso, ou false, caso contrário.
                             */
                            bool start();

                            /**
                             * Pára a captura de áudio.
                             * @return true, caso a ação ocorra com sucesso, ou false, caso contrário.
                             */
                            bool stop();

                            /**
                             * Coloca em execução a captura de áudio.
                             * Deve ser executado por uma thread paralela.
                             * @param param alguma parâmetro que será utilizado pela thread ou NULL.
                             * @return NULL, caso a ação seja executada com sucesso, ou alguma instância de um objeto, caso necessário.
                             */
                            static void* createAudioCaptureThread(void* param);

                            /**
                             * Percorre o vetor de fluxos de saída e envia os bytes do áudio que está sendo capturado para cada fluxo (OutputStream).
                             * Este método somente é usado caso não seja especificado um destino para o áudio a ser capturado.
                             * @param b um byte do áudio que está sendo capturado.
                             */
                            void write(int b);

                            /**
                             * Percorre o vetor de fluxos de saída e envia os bytes do áudio que está sendo capturado para cada fluxo (OutputStream).
                             * Este método somente é usado caso não seja especificado um destino para o áudio a ser capturado.
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

#endif	/* _AUDIOCAPTURE_H */
