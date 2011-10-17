#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include "upnp/UPnPAction.h"
#include "upnp/UPnPArgument.h"
#include "upnp/UPnPEventListener.h"
#include "upnp/UPnPDevice.h"
#include "upnp/UPnPService.h"
#include "upnp/UPnPDeviceList.h"
#include "upnp/UPnPDeviceController.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
using namespace std;
using namespace br::ufscar::lince::ginga::hones::upnp;

HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.helloworld");

/** constante para fazer a conversão de base64 para binário **/
static const string base64_chars =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz"
        "0123456789+/";

int main(int argc, char* argv[]){
    int result = 0;

    try{

        LoggerUtil::configure();

        LoggerUtil_info(logger, "Entrando na aplicacao.");

	UPnPDeviceController* ctrl = UPnPDeviceController::getInstance();

	LoggerUtil_info(logger, "UPnPDeviceController instanciado: " << (ctrl != NULL));

        ctrl->start();

        int ch;

    	do
	{
		ch = getchar();
		ch = toupper( ch );
		switch (ch) {
                case 'A':
                {
                    /**
                     * Procura por dispositivos do tipo HelloWorld.
                     * Retorna a primeira instância encontrada.
                     * Caso fosse preciso, é possível percorrer a lista com todos os dispositivos encontrados, 
                     * pelo método getUPnPDeviceList(), e verificar seus tipos.
                     */
                    UPnPDevice* dev = ctrl->getUPnPDevice("urn:schemas-upnp-org:device:HelloWorld:1");
                    if(dev != NULL){
                        /** Procura pelo serviço HelloWorldService na instância do dispositivo encontrado no passo anterior. */
                        UPnPService* serv = dev->getUPnPService("urn:schemas-upnp-org:service:HelloWorldService:1");
                        /** Procura pela ação/método SetFile no serviço encontrado no passo anterior. */
                        UPnPAction* action = serv->getUPnPAction("SetFile");

                        LoggerUtil_info(logger, "Digite o caminho completo do arquivo a ser enviado: ");

                        char path[80];
                        scanf("%s", path);

                        string filename = path;
                        filename = filename.substr(filename.find_last_of('/') + 1);

                        off_t in_len = (off_t) 0;
                        char* bytes_to_encode = NULL;

                        struct stat st;
                        if (stat(path, &st) >= 0)
                            in_len = st.st_size;
                        
                        if (in_len > 0) {
                            bytes_to_encode = (char*) malloc(in_len * sizeof(char));

                            int fd = open(path, O_RDONLY | O_NONBLOCK | O_SYNC | O_NDELAY);
                            if (fd != -1) {
                                off_t length = read(fd, bytes_to_encode, in_len);
                                if (length == -1) {
                                    free(bytes_to_encode);
                                    bytes_to_encode = NULL;
                                }
                                close(fd);
                            }


                            string ret;

                            if (bytes_to_encode != NULL) {
                                
                                /** função para fazer a conversão de binário para base64 */
                                int i = 0;
                                int j = 0;
                                unsigned char char_array_3[3];
                                unsigned char char_array_4[4];

                                while (in_len--) {
                                    char_array_3[i++] = *(bytes_to_encode++);
                                    if (i == 3) {
                                        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                                        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                                        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                                        char_array_4[3] = char_array_3[2] & 0x3f;

                                        for (i = 0; (i < 4); i++)
                                            ret += base64_chars[char_array_4[i]];
                                        i = 0;
                                    }
                                }

                                if (i) {
                                    for (j = i; j < 3; j++)
                                        char_array_3[j] = '\0';

                                    char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
                                    char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
                                    char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
                                    char_array_4[3] = char_array_3[2] & 0x3f;

                                    for (j = 0; (j < i + 1); j++)
                                        ret += base64_chars[char_array_4[j]];

                                    while ((i++ < 3))
                                        ret += '=';

                                }
                                /** fim da conversão de binário para base64 */

                                free(bytes_to_encode);
                            }

                            /** Atribui valores aos parâmetros da ação/método recuperado no passo anterior. */
                            action->getUPnPArgument("Filename")->setValue(filename.c_str());
                            action->getUPnPArgument("Data")->setValue(ret.c_str());

                            /** Aciona a ação/método. */
                            if (action->sendUPnPAction() == true) {
                                /** Verifica o parâmetro de resposta dessa ação/método. */
                                string result = action->getUPnPArgument("Result")->getValue();
                                LoggerUtil_info(logger, "Resultado do processamento do arquivo: " << ((result.compare("1") == 0) ? "sucesso!" : "falha!"));
                            } else {
                                LoggerUtil_info(logger, "Falha no envio do arquivo!");
                            }
                        } else {
                            LoggerUtil_info(logger, "Falha ao ler o arquivo!");
                        }
                    }
                    break;
                }
                case 'C':
                {
                    /**
                     * Procura por dispositivos do tipo HelloWorld.
                     * Retorna a primeira instância encontrada.
                     * Caso fosse preciso, é possível percorrer a lista com todos os dispositivos encontrados, 
                     * pelo método getUPnPDeviceList(), e verificar seus tipos.
                     */
                    UPnPDevice* dev = ctrl->getUPnPDevice("urn:schemas-upnp-org:device:HelloWorld:1");
                    if(dev != NULL){
                        /** Procura pelo serviço HelloWorldService na instância do dispositivo encontrado no passo anterior. */
                        UPnPService* serv = dev->getUPnPService("urn:schemas-upnp-org:service:HelloWorldService:1");
                        /** Procura pela ação/método SetFile no serviço encontrado no passo anterior. */
                        UPnPAction* action = serv->getUPnPAction("GetMessage");

                        /** Aciona a ação/método. */
                        if(action->sendUPnPAction() == true){
                            /** Verifica o parâmetro de resposta dessa ação/método. */
                            string message = action->getUPnPArgument("Message")->getValue();
                            LoggerUtil_info(logger, "Resultado da consulta a mensagem: " + message);
                        } else {
                            LoggerUtil_info(logger, "Falha na consulta a mensagem!");
                        }
                    }
                    break;
                }
                case 'M':
                {
                    /**
                     * Procura por dispositivos do tipo HelloWorld.
                     * Retorna a primeira instância encontrada.
                     * Caso fosse preciso, é possível percorrer a lista com todos os dispositivos encontrados, 
                     * pelo método getUPnPDeviceList(), e verificar seus tipos.
                     */
                    UPnPDevice* dev = ctrl->getUPnPDevice("urn:schemas-upnp-org:device:HelloWorld:1");
                    if(dev != NULL){
                        /** Procura pelo serviço HelloWorldService na instância do dispositivo encontrado no passo anterior. */
                        UPnPService* serv = dev->getUPnPService("urn:schemas-upnp-org:service:HelloWorldService:1");
                        /** Procura pela ação/método SetFile no serviço encontrado no passo anterior. */
                        UPnPAction* action = serv->getUPnPAction("SetMessage");

                        LoggerUtil_info(logger, "Digite a mensagem a ser enviada: ");

                        char message[80];
                        scanf("%s", message);

                        /** Atribui valor ao parâmetro da ação/método recuperado no passo anterior. */
                        action->getUPnPArgument("Message")->setValue(message);

                        /** Aciona a ação/método. */
                        if(action->sendUPnPAction() == true){
                            /** Verifica o parâmetro de resposta dessa ação/método. */
                            string result = action->getUPnPArgument("Result")->getValue();
                            LoggerUtil_info(logger, "Resultado do processamento da mensagem: " << ((result.compare("1") == 0) ? "sucesso!" : "falha!"));
                        } else {
                            LoggerUtil_info(logger, "Falha no envio da mensagem!");
                        }
                    }
                    break;
                }
                case 'Q':
                    break;
                default:
                    LoggerUtil_info(logger, "\n'q' : Sair\n'a' : Enviar um arquivo\n'm' : Enviar uma mensagem\n'c' : Consultar a última/atual mensagem enviada ao serviço");
		}
	} while( ch != 'Q');

	ctrl->stop();

	LoggerUtil_info(logger, "Saindo da aplicacao.");

    } catch(...) {
        result = -1;
    }

    return result;
}

