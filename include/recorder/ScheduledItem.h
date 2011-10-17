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

#ifndef _SCHEDULEDITEM_H
#define	_SCHEDULEDITEM_H

#include <string>
#include <vector>
using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace recorder {

                        class ScheduledItem {
                        private:
                            int frameRate;
                            int sampleRate;
                            int videoBitrate;
                            int audioBitrate;
                            int audioChannels;
                            int width;
                            int height;
                            string videoFormat;
                            string videoCodec;
                            string audioCodec;
                            string destinationFile;
                            string beginDate;
                            string beginTime;
                            string endDate;
                            string endTime;
                            double duration;

                        public:

                            /**
                             * Construtor.
                             */
                            ScheduledItem();

                            /**
                             * Destrutor.
                             */
                            ~ScheduledItem();

                            /**
                             * Configura a taxa de quadros por segundo (fps) do vídeo a ser gravado.
                             * @param frameRate taxa de quadros (fps) do vídeo destino (valor padrão: 25).
                             */
                            void setFrameRate(int frameRate);

                            /**
                             * Retorna a taxa de quadros por segundo (fps) do vídeo a ser gravado.
                             * @return taxa de quadros (fps) do vídeo destino.
                             */
                            int getFrameRate();

                            /**
                             * Configura a taxa de amostragem do áudio do vídeo a ser gravado.
                             * @param sampleRate taxa de amostragem do áudio do vídeo destino (valor padrão: 44100).
                             */
                            void setSampleRate(int sampleRate);

                            /**
                             * Retorna a taxa de amostragem do áudio do vídeo a ser gravado.
                             * @return taxa de amostragem do áudio do vídeo destino.
                             */
                            int getSampleRate();

                            /**
                             * Configura a taxa de bits (bit/s) do vídeo a ser gravado.
                             * @param videoBitrate taxa de bits do vídeo destino (valor padrão: 200000).
                             */
                            void setVideoBitrate(int videoBitrate);

                            /**
                             * Retorna a taxa de bits (bit/s) do vídeo a ser gravado.
                             * @return taxa de bits do vídeo destino.
                             */
                            int getVideoBitrate();

                            /**
                             * Configura a taxa de bits (bit/s) do áudio do vídeo a ser gravado.
                             * @param audioBitrate taxa de bits do áudio do vídeo destino (valor padrão: 64000).
                             */
                            void setAudioBitrate(int audioBitrate);

                            /**
                             * Retorna a taxa de bits (bit/s) do áudio do vídeo a ser gravado.
                             * @return taxa de bits do áudio do vídeo destino.
                             */
                            int getAudioBitrate();

                            /**
                             * Configura a quantidade de canais do áudio do vídeo a ser gravado.
                             * @param audioChannels quantidade de canais do áudio do vídeo destino (valor padrão: 1).
                             */
                            void setAudioChannels(int audioChannels);

                            /**
                             * Retorna a quantidade de canais do áudio do vídeo a ser gravado.
                             * @return quantidade de canais do áudio do vídeo destino.
                             */
                            int getAudioChannels();

                            /**
                             * Configura a largura (em pixels) do vídeo a ser gravado.
                             * @param width largura do vídeo destino.
                             */
                            void setWidth(int width);

                            /**
                             * Retorna a largura (em pixels) do vídeo a ser gravado.
                             * @return largura do vídeo destino.
                             */
                            int getWidth();

                            /**
                             * Configura a altura (em pixels) do vídeo a ser gravado.
                             * @param height altura do vídeo destino.
                             */
                            void setHeight(int height);

                            /**
                             * Retorna a altura (em pixels) do vídeo a ser gravado.
                             * @return altura do vídeo destino.
                             */
                            int getHeight();

                            /**
                             * Configura o formato do vídeo a ser gravado.
                             * @param videoFormat formato do vídeo destino (possíveis valores: mpeg2video, mpegts, mp4, rtp, ...).
                             */
                            void setVideoFormat(const char* videoFormat);

                            /**
                             * Retorna o formato do vídeo a ser gravado.
                             * @return formato do vídeo destino.
                             */
                            const char* getVideoFormat();

                            /**
                             * Configura o codec (compressão) do vídeo a ser gravado.
                             * @param videoCodec codec do vídeo destino (possíveis valores: mpeg2video, mpeg4, theora, h264, ...).
                             */
                            void setVideoCodec(const char* videoCodec);

                            /**
                             * Retorna o codec (compressão) do vídeo a ser gravado.
                             * @return codec do vídeo destino.
                             */
                            const char* getVideoCodec();

                            /**
                             * Configura o codec (compressão) do áudio do vídeo a ser gravado.
                             * @param audioCodec codec do áudio do vídeo destino (possíveis valores: mp2, mp3, vorbis, aac, ...).
                             */
                            void setAudioCodec(const char* audioCodec);

                            /**
                             * Retorna o codec (compressão) do áudio do vídeo a ser gravado.
                             * @return codec do áudio do vídeo destino.
                             */
                            const char* getAudioCodec();

                            /**
                             * Configura um destino (arquivo local) do vídeo a ser gravado.
                             * @param destination destino do vídeo (possível valor: "/home/user/file.mpg").
                             */
                            void setDestinationFile(const char* destination);

                            /**
                             * Retorna o destino do vídeo a ser gravado.
                             * @return destino do vídeo.
                             */
                            const char* getDestinationFile();

                            /**
                             * Configura a data de início da gravação do vídeo.
                             * @param date data de início da gravação (possível valor: "01/11/2011").
                             */
                            void setBeginRecordingDate(const char* date);

                            /**
                             * Retorna a data de início da gravação do vídeo.
                             * @return data de início da gravação.
                             */
                            const char* getBeginRecordingDate();

                            /**
                             * Configura a hora de início da gravação do vídeo.
                             * @param time hora de início da gravação (possível valor: "21:30:00").
                             */
                            void setBeginRecordingTime(const char* time);

                            /**
                             * Retorna a hora de início da gravação do vídeo.
                             * @return hora de início da gravação.
                             */
                            const char* getBeginRecordingTime();

                            /**
                             * Configura a data de término da gravação do vídeo.
                             * @param date data de término da gravação (possível valor: "02/11/2011").
                             */
                            void setEndRecordingDate(const char* date);

                            /**
                             * Retorna a data de término da gravação do vídeo.
                             * @return data de término da gravação.
                             */
                            const char* getEndRecordingDate();

                            /**
                             * Configura a hora de término da gravação do vídeo.
                             * @param time hora de término da gravação (possível valor: "01:30:00").
                             */
                            void setEndRecordingTime(const char* time);

                            /**
                             * Retorna a hora de término da gravação do vídeo.
                             * @return hora de término da gravação.
                             */
                            const char* getEndRecordingTime();

                            /**
                             * Configura a duração da gravação do vídeo.
                             * É uma alternativa aos métodos setEndRecordingDate() e setEndRecordingTime().
                             * @param duration duração da gravação em segundos (possível valor: 25800).
                             */
                            void setRecordingDuration(double duration);

                            /**
                             * Retorna duração da gravação do vídeo.
                             * @return duração da gravação.
                             */
                            double getRecordingDuration();

                        };

                    }
                }
            }
        }
    }
}

#endif	/* _SCHEDULEDITEM_H */
