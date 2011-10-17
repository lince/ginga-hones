#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include "HelloWorld.h"
#include "HelloWorldService.h"
#include "util/Utils.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace helloworld {


                        const char* HelloWorldService::HELLOWORLD_SERVICE_TYPE = "urn:schemas-upnp-org:service:HelloWorldService:1";
                        const char* HelloWorldService::HELLOWORLD_SERVICE_DESCRIPTION =
                                "<?xml version=\"1.0\"?>\n"
                                "<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\" >\n"
                                " 	<specVersion>\n"
                                " 		<major>1</major>\n"
                                " 		<minor>0</minor>\n"
                                " 	</specVersion>\n"
                                " 	<actionList>\n"
                                " 		<action>\n"
                                " 			<name>SetMessage</name>\n"
                                " 			<argumentList>\n"
                                " 				<argument>\n"
                                " 					<name>Message</name>\n"
                                " 					<relatedStateVariable>CurrentMessage</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>Result</name>\n"
                                " 					<relatedStateVariable>CurrentResult</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 			</argumentList>\n"
                                " 		</action>\n"
                                " 		<action>\n"
                                " 			<name>GetMessage</name>\n"
                                " 			<argumentList>\n"
                                " 				<argument>\n"
                                " 					<name>Message</name>\n"
                                " 					<relatedStateVariable>CurrentMessage</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 			</argumentList>\n"
                                " 		</action>\n"
                                " 		<action>\n"
                                " 			<name>SetFile</name>\n"
                                " 			<argumentList>\n"
                                " 				<argument>\n"
                                " 					<name>Filename</name>\n"
                                " 					<relatedStateVariable>CurrentFilename</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>Data</name>\n"
                                " 					<relatedStateVariable>CurrentData</relatedStateVariable>\n"
                                " 					<direction>in</direction>\n"
                                " 				</argument>\n"
                                " 				<argument>\n"
                                " 					<name>Result</name>\n"
                                " 					<relatedStateVariable>CurrentResult</relatedStateVariable>\n"
                                " 					<direction>out</direction>\n"
                                " 				</argument>\n"
                                " 			</argumentList>\n"
                                " 		</action>\n"
                                " 	</actionList>\n"
                                " 	<serviceStateTable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentMessage</name>\n"
                                " 			<dataType>string</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentFilename</name>\n"
                                " 			<dataType>string</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentData</name>\n"
                                " 			<dataType>bin.base64</dataType>\n"
                                " 		</stateVariable>\n"
                                " 		<stateVariable sendEvents=\"no\">\n"
                                " 			<name>CurrentResult</name>\n"
                                " 			<dataType>boolean</dataType>\n"
                                " 		</stateVariable>\n"
                                " 	</serviceStateTable>\n"
                                "</scpd>\n";

                        HelloWorldService::HelloWorldService(HelloWorld* instance) {
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.helloworld.helloworldservice");
                            LoggerUtil_info(logger, "Entrando no construtor HelloWorldService()!");

                            device = instance;

                            UPnPService* serv = getService();

                            bool ret = serv->loadSCPD(HELLOWORLD_SERVICE_DESCRIPTION);
                            if (ret == false)
                                throw "Invalid description";

                            serv->setUPnPActionListener(this);
                            serv->setUPnPQueryListener(this);

                            initializeUPnPStateVariables();

                            LoggerUtil_info(logger, "Saindo do construtor HelloWorldService()!");
                        }

                        HelloWorldService::~HelloWorldService() {
                        }

                        void HelloWorldService::initializeUPnPStateVariables() {
                            getService()->getUPnPStateVariable("CurrentMessage")->setValue("");
                        }

                        UPnPService* HelloWorldService::getService() {
                            return device->getUPnPService(HELLOWORLD_SERVICE_TYPE);
                        }

                        bool HelloWorldService::actionRequest(UPnPAction *action) {
                            string actionName = action->getName();

                            if (actionName.compare("SetMessage") == 0)
                                return actionSetMessage(action);
                            else if (actionName.compare("GetMessage") == 0)
                                return actionGetMessage(action);
                            else if (actionName.compare("SetFile") == 0)
                                return actionSetFile(action);

                            return false;
                        }

                        bool HelloWorldService::variableRequest(UPnPStateVariable *stateVar) {
                            UPnPStateVariable* serviceStateVar = getService()->getUPnPStateVariable(stateVar->getName());
                            if (serviceStateVar == NULL)
                                return false;

                            stateVar->setValue(serviceStateVar->getValue());

                            return true;
                        }

                        bool HelloWorldService::actionSetMessage(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no metodo actionSetMessage(...)!");

                            string message = action->getUPnPArgument("Message")->getValue();

                            getService()->getUPnPStateVariable("CurrentMessage")->setValue(message.c_str());

                            LoggerUtil_debug(logger, "Mensagem recebida: " << message.c_str());

                            action->getUPnPArgument("Result")->setValue("1");

                            return true;
                        }

                        bool HelloWorldService::actionGetMessage(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no metodo actionGetMessage(...)!");

                            string message = getService()->getUPnPStateVariable("CurrentMessage")->getValue();

                            LoggerUtil_debug(logger, "Mensagem atual/última: " << message.c_str());

                            action->getUPnPArgument("Message")->setValue(message.c_str());

                            return true;
                        }

                        /** constante para fazer a conversão de base64 para binário **/
                        static const string base64_chars =
                                "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                "abcdefghijklmnopqrstuvwxyz"
                                "0123456789+/";

                        /** função para fazer a conversão de base64 para binário **/
                        static inline bool is_base64(unsigned char c) {
                            return (isalnum(c) || (c == '+') || (c == '/'));
                        }

                        bool HelloWorldService::actionSetFile(UPnPAction *action) {
                            LoggerUtil_info(logger, "Entrando no metodo actionSetFile(...)!");

                            string filename = action->getUPnPArgument("Filename")->getValue();
                            string encoded_string = action->getUPnPArgument("Data")->getValue();

                            LoggerUtil_debug(logger, "Arquivo recebido: " << filename.c_str());
                            
                            filename = string(Utils::getHomeDir()).append("/").append(filename).c_str();

                            /** conversão de base64 para binário **/
                            int in_len = encoded_string.size();
                            int i = 0;
                            int j = 0;
                            int in_ = 0;
                            unsigned char char_array_4[4], char_array_3[3];
                            string ret;

                            while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) {
                                char_array_4[i++] = encoded_string[in_];
                                in_++;
                                if (i == 4) {
                                    for (i = 0; i < 4; i++)
                                        char_array_4[i] = base64_chars.find(char_array_4[i]);

                                    char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                                    char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                                    char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                                    for (i = 0; (i < 3); i++)
                                        ret += char_array_3[i];
                                    i = 0;
                                }
                            }

                            if (i) {
                                for (j = i; j < 4; j++)
                                    char_array_4[j] = 0;

                                for (j = 0; j < 4; j++)
                                    char_array_4[j] = base64_chars.find(char_array_4[j]);

                                char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
                                char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
                                char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

                                for (j = 0; (j < i - 1); j++) ret += char_array_3[j];
                            }
                            /** fim da conversão de base64 para binário **/

                            int fd = open(filename.c_str(), O_CREAT | O_RDWR);
                            if (fd != -1) {
                                if (write(fd, ret.c_str(), ret.length()) == -1){
                                    action->getUPnPArgument("Result")->setValue("0");
                                }
                                close(fd);
                            }

                            action->getUPnPArgument("Result")->setValue("1");

                            return true;
                        }

                    }
                }
            }
        }
    }
}
