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

#ifndef _UPNPHTTPSERVER_H
#define	_UPNPHTTPSERVER_H

#include <pthread.h>
#include <time.h>
#include <fcntl.h>
#include <string>
#include "Vector.h"
#include "UPnPHttpListener.h"
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

                        class UPnPHttpServer {
                        private:
                            static UPnPHttpServer* instance;
                            static Vector* listeners;
                            int port;
                            int serverSock;
                            string ip;
                            bool running;
                            pthread_t thread;
                            HLoggerPtr logger;

                        public:

                            /**
                             * Retorna a instância do servidor HTTP.
                             * @return instância do servidor HTTP.
                             */
                            static UPnPHttpServer* getInstance();

                            /**
                             * Destrói a instância do servidor HTTP.
                             */
                            static void deleteInstance();

                            /**
                             * Adiciona um <em>listener</em> HTTP ao servidor.
                             * @param listener <em>listener</em> HTTP.
                             * @return true, se a ação for executada com sucesso, ou false, caso contrário.
                             */
                            bool addUPnPHttpListener(UPnPHttpListener* listener);

                            /**
                             * Remove um <em>listener</em> HTTP ao servidor.
                             * @param listener <em>listener</em> HTTP.
                             * @return true, se a ação for executada com sucesso, ou false, caso contrário.
                             */
                            bool removeUPnPHttpListener(UPnPHttpListener* listener);

                            /**
                             * Coloca em execução o servidor HTTP.
                             * Deve ser executado em uma <em>thread</em> paralela.
                             */
                            void run();

                            /**
                             * Verifica se o servidor HTTP está em execução.
                             * @return true, se o servidor HTTP está ativo, ou false, caso contrário.
                             */
                            bool isRunning();

                            /**
                             * Ativa o servidor HTTP.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool start();

                            /**
                             * Pára o servidor HTTP.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool stop();

                            /**
                             * Configura a porta do servidor HTTP.
                             * @param port porta do servidor HTTP.
                             */
                            void setPort(int port);

                            /**
                             * Retorna a porta do servidor HTTP.
                             * @return porta do servidor HTTP.
                             */
                            int getPort();

                            /**
                             * Configura o IP em que o servidor HTTP atuará.
                             * O IP é utilizado na descoberta de qual versão será utilizada (IPv4 ou IPv6).
                             * Caso não seja fornecido, é utilizado o IPv4.
                             * @param ip IP em que o servidor HTTP atuará.
                             */
                            void setServerAddress(const char* ip);

                        private:

                            UPnPHttpServer();

                            ~UPnPHttpServer();

                            /**
                             * Configura o estado de execução do servidor HTTP.
                             * @param running true, caso o servidor HTTP esteja ativo, ou false, caso contrário.
                             */
                            void setRunning(bool running);

                            /**
                             * Atende uma requisição ao servidor HTTP.
                             * Deve ser executado em uma <em>thread</em> paralela.
                             * @param param <em>socket</em> que será tratada a requisição.
                             * @return NULL, caso a ação seja executada com sucesso, ou instância de um objeto, caso contrário.
                             */
                            static void* serve(void* param);

                            /**
                             * Retorna um cabeçalho HTTP de acordo com os parâmetros passados.
                             * @param statusCode código que representa o estado atual da requisição.
                             * @param statusStr descrição do estado atual da requisição.
                             * @param contentType tipo de conteúdo (arquivo) que será transferido ao cliente.
                             * @param range faixa de bytes que está sendo tranferida ao cliente.
                             * @param contentLength tamanho do arquivo (em bytes).
                             * @param lastModified última alteração do arquivo.
                             * @param contentFeatures descritores DLNA sobre o conteúdo (arquivo).
                             * @param protocol protocolo e sua versão.
                             * @return cabeçalho HTTP.
                             */
                            static char* getHeader(const char* statusCode, const char* statusStr, const char* contentType, const char* range, off_t contentLength, time_t lastModified, const char* contentFeatures, const char* protocol);
                            
                        };

                    }
                }
            }
        }
    }
}

#endif	/* _UPNPHTTPSERVER_H */

