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

#ifndef _TVMSCONTENTDIRECTORY_H
#define	_TVMSCONTENTDIRECTORY_H

#include <string>
#include "UPnPActionListener.h"
#include "UPnPQueryListener.h"
#include "UPnPService.h"
#include "UPnPAction.h"
#include "UPnPArgument.h"
#include "UPnPHttpListener.h"
#include "UPnPHttpServer.h"
#include "DirectoryList.h"
#include "Utils.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
using namespace br::ufscar::lince::ginga::hones::util;
using namespace std;
using namespace br::ufscar::lince::ginga::hones::upnp;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediaserver {

                        class TVMediaServer;

                        class TVMSContentDirectory : public UPnPActionListener, public UPnPQueryListener, public UPnPHttpListener {
                        private:
                            TVMediaServer* tvServer;
                            DirectoryList directoryList;
                            int nextContentID;
                            UPnPHttpServer* httpServer;
                            HLoggerPtr logger;

                        public:

                            static const char *SERVICE_TYPE;

                            static const char *GET_SEARCH_CAPABILITIES;
                            static const char *GET_SORT_CAPABILITIES;
                            static const char *GET_SYSTEM_UPDATE_ID;
                            static const char *BROWSE;

                            static const char *SEARCH_CAPS;
                            static const char *SORT_CAPS;
                            static const char *ID;
                            static const char *OBJECT_ID;
                            static const char *BROWSE_FLAG;
                            static const char *FILTER;
                            static const char *STARTING_INDEX;
                            static const char *REQUESTED_COUNT;
                            static const char *SORT_CRITERIA;
                            static const char *RESULT;
                            static const char *NUMBER_RETURNED;
                            static const char *TOTAL_MATCHES;
                            static const char *UPDATE_ID;
                            
                            static const char *SYSTEM_UPDATE_ID;
                            static const char *BROWSE_METADATA;
                            static const char *BROWSE_DIRECT_CHILDREN;

                            static const char *CONTENT_DIRECTORY;

                            static const char *SCPD;


                            TVMSContentDirectory(TVMediaServer* tvs);
                            ~TVMSContentDirectory();

                            /**
                             * Inicializa as variáveis de estado necessárias ao funcionamento do serviço de diretório de conteúdo.
                             */
                            void initializeUPnPStateVariables();

                            /**
                             * Retorna a instância do servidor de mídias.
                             * @return servidor de mídias.
                             */
                            TVMediaServer* getMediaServer();

                            /**
                             * Retorna a instância do serviço de diretório de conteúdo.
                             * @return serviço de diretório de conteúdo.
                             */
                            UPnPService* getService();

                            /**
                             * Recebe solicitações de ações a serem executadas.
                             * @param action ação a ser executada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionRequest(UPnPAction *action);

                            /**
                             * Envia informações sobre as possibilidades de busca suportadas.
                             *
                             * Argumentos de saída da ação:
                             *      - SearchCaps
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetSearchCapabilities(UPnPAction *action);

                            /**
                             * Envia informações sobre as possibilidades de ordenação suportadas.
                             *
                             * Argumentos de saída da ação:
                             *      - SortCaps
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetSortCapabilities(UPnPAction *action);

                            /**
                             * Envia o valor atual da variável <em>SystemUpdateID</em>.
                             * Desta forma, os clientes são informados sobre mudanças no diretório de conteúdo.
                             *
                             * Argumentos de saída da ação:
                             *      - Id
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionGetSystemUpdateID(UPnPAction *action);

                            /**
                             * Permite ao cliente navegar na estrutura de arquivos e diretórios disponibilizados.
                             *
                             * Argumentos de entrada da ação:
                             *      - ObjectID
                             *      - BrowseFlag
                             *      - Filter
                             *      - StartingIndex
                             *      - RequestedCount
                             *      - SortCriteria
                             *
                             * Argumentos de saída da ação:
                             *      - Result
                             *      - NumberReturned
                             *      - TotalMatches
                             *      - UpdateID
                             *
                             * @param action ação solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool actionBrowse(UPnPAction *action);

                            /**
                             * Retorna um identificador único para cada arquivo e/ou diretório disponibilizado.
                             * @return identificador.
                             */
                            int getNextContentID();

                            /**
                             * Adiciona um diretório a lista de conteúdo disponibilizado.
                             * @param name caminho completo do diretório.
                             */
                            void addContentDirectory(const char* name);

                            /**
                             * Remove um diretório da lista de conteúdo disponibilizado.
                             * @param name caminho completo do diretório.
                             */
                            void removeContentDirectory(const char* name);

                            /**
                             * Retorna uma lista dos diretórios disponibilizados pelo Servidor de Mídias.
                             * @return lista de diretórios.
                             */
                            DirectoryList* getContentDirectories();

                            /**
                             * Envia informações sobre um arquivo solicitado.
                             * @param filename arquivo solicitado.
                             * @param fileLength tamanho do arquivo.
                             * @param lastModified data da última alteração.
                             * @param isDirectory se é um diretório.
                             * @param isReadable se pode ser lido.
                             * @param contentType tipo do conteúdo (MIME Type).
                             * @param absolutePath caminho completo no sistema de arquivos.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool httpGetInfo(const char* filename, off_t* fileLength, time_t* lastModified, bool* isDirectory, bool* isReadable, char** contentType, char** absolutePath);

                            /**
                             * Abre um arquivo para leitura.
                             * @param filename nome do arquivo a ser aberto.
                             * @param mode modo de abertura (leitura ou escrita).
                             * @return informações sobre o arquivo aberto, tais como caminho e posição da leitura.
                             */
                            UPnPHttpFileHandle* httpOpen(const char* filename, enum OpenFileMode mode);

                            /**
                             * Lê um arquivo no Servidor de Mídias.
                             * @param fh informações sobre o arquivo, tais como caminho e posição para leitura.
                             * @param buf local para envio dos dados.
                             * @param buflen quantidade de bytes a serem lidos.
                             * @return quantidade de bytes lidos.
                             */
                            off_t httpRead(UPnPHttpFileHandle* fh, char* buf, off_t buflen);

                            /**
                             * Necessário na implementação da interface @see UPnPHttpListener
                             */
                            off_t httpWrite(UPnPHttpFileHandle* fh, char* buf, off_t buflen);

                            /**
                             * Configura a posição de leitura de um arquivo.
                             * @param fh informações sobre o arquivo, tais como caminho e posição para leitura.
                             * @param offset nova posição.
                             * @param origin origem no novo posicionamento (a partir do início, do fim ou da posição atual).
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool httpSeek(UPnPHttpFileHandle* fh, off_t offset, int origin);

                            /**
                             * Fecha um arquivo aberto no Servidor de Mídias.
                             * @param fh informações sobre o arquivo, tais como caminho e posição para leitura.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool httpClose(UPnPHttpFileHandle* fh);

                            /**
                             * Envia o estado atual de uma variável do serviço.
                             * @param stateVar variável solicitada.
                             * @return true, caso a ação seja executada com sucesso, ou false, caso contrário.
                             */
                            bool variableRequest(UPnPStateVariable *stateVar);

                        private:
                            /**
                             * Configura a instância do servidor de mídias.
                             * @param tvs nova instância.
                             */
                            void setMediaServer(TVMediaServer* tvs);

                            /**
                             * Retorna a URI de um arquivo baseada na localização do </em>Control Point</em>.
                             * @param action ação do </em>Control Point</em>
                             * @param file arquivo
                             * @return URI
                             */
                            char* getURI(UPnPAction* action, File* file);

                            /**
                             * Adiciona a descrição de um item de mídia (arquivo) a um documento XML.
                             * @param didl documento XML.
                             * @param file arquivo.
                             * @param restricted se é restrito.
                             * @param uri localização original
                             * @param filter filtro das propriedades a serem descritas/inseridas.
                             */
                            void addUPnPItem(NodePtr didl, File* file, bool restricted, char* uri, string filter);

                            /**
                             * Adiciona a descrição de um item de mídia (diretório) a um documento XML.
                             * @param didl documento XML.
                             * @param dir diretório.
                             * @param restricted se é restrito.
                             * @param searchable se buscas podem ser realizadas.
                             */
                            void addUPnPContainer(NodePtr didl, Directory* dir, bool restricted, bool searchable);

                        };

                    }
                }
            }
        }
    }
}

#endif	/* _TVMSCONTENTDIRECTORY_H */

