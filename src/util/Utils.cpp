#include "honesconfig.h"

#include <ctype.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <sstream>
#include <upnp/ixml.h>
#include <upnp/upnp.h>
#include "upnp/UPnPService.h"
#include "upnp/UPnPStateVariable.h"
#include "util/Utils.h"
#include <ginga/linceutil/LoggerUtil.h>
using namespace br::ufscar::lince::util;
#ifdef HAVE_DLNA
  #define class _class
  #include <dlna.h>
  #undef class
#endif

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace util {

                        bool Utils::initializedUPnP = false;
                        void* Utils::dlna = NULL;

                        string Utils::timeToString(double time) {
                            ostringstream out;
                            if (time > 59.0) {
                                int hours = time / 3600;
                                int minutes = (time - (hours * 3600)) / 60;
                                int seconds = time - (hours * 3600) - (minutes * 60);

                                if (hours < 10)
                                    out << "0";
                                out << hours;
                                out << ":";
                                if (minutes < 10)
                                    out << "0";
                                out << minutes;
                                out << ":";
                                if (seconds < 10)
                                    out << "0";
                                out << seconds;
                            } else {
                                out << "00:00:";
                                if (time < 10.0)
                                    out << "0";
                                out << time;
                            }

                            return out.str();
                        }

                        double Utils::stringToTime(string str) {
                            string time = str;
                            double hours, minutes, seconds, milliseconds;

                            hours = atof(time.substr(0, time.find(":")).c_str());
                            time = time.substr(time.find(":") + 1);
                            minutes = atof(time.substr(0, time.find(":")).c_str());
                            time = time.substr(time.find(":") + 1);
                            seconds = atof(time.substr(0, time.find(".")).c_str());
                            time = time.substr(time.find(".") + 1);
                            if (time.find("/") != string::npos) {
                                double f0, f1;
                                f0 = atof(time.substr(0, time.find("/")).c_str());
                                time = time.substr(time.find("/") + 1);
                                f1 = atof(time.c_str());
                                milliseconds = f0 / f1;
                            } else {
                                milliseconds = atof(time.c_str());
                            }

                            return (double) (
                                    (hours * 3600) +
                                    (minutes * 60) +
                                    (seconds) +
                                    (milliseconds / 1000));
                        }

                        void Utils::updateLastChangeStateVariable(UPnPService* service, UPnPStateVariable* var) {
                            UPnPStateVariable* lastChangeStateVar = service->getUPnPStateVariable("LastChange");
                            if (lastChangeStateVar != NULL) {
                                ostringstream out;
                                string valBuf = var->getValue();
                                string varName = var->getName();

                                if ((var->isSendEvents() == false) && (int(varName.find("A_ARG")) != 0)) {
                                    out << "<Event>\n";
                                    out << "<InstanceID val=\"0\">\n";
                                    out << "<" << var->getName() << " val=\"" << encodeXMLChars(valBuf) << "\"/>\n";
                                    out << "</InstanceID>\n";
                                    out << "</Event>";
                                    lastChangeStateVar->setValue(out.str().c_str());
                                }
                            }
                        }
                        
                        const char* Utils::stringReplaceChars(string &str, const char *from, const char *to) {
                            size_t i = 0;
                            size_t pos = 0;
                            string fromStr = from;
                            string toStr = to;
                            while ((i = str.find(fromStr, pos)) != string::npos) {
                                str.replace(i, fromStr.size(), toStr);
                                pos = i + toStr.size();
                            }
                            return str.c_str();
                        }

                        const char* Utils::parseXMLChars(string &str){
                            stringReplaceChars(str, "&amp;", "&");
                            stringReplaceChars(str, "&lt;", "<");
                            stringReplaceChars(str, "&gt;", ">");
                            stringReplaceChars(str, "&quot;", "\"");
                            stringReplaceChars(str, "&apos;", "'");
                            return str.c_str();
                        }

                        const char* Utils::encodeXMLChars(string &str){
                            stringReplaceChars(str, "&", "&amp;");
                            stringReplaceChars(str, "<", "&lt;");
                            stringReplaceChars(str, ">", "&gt;");
                            stringReplaceChars(str, "\"", "&quot;");
                            stringReplaceChars(str, "'", "&apos;");
                            return str.c_str();
                        }

                        const char* Utils::stringToLowerCase(string &str){
                            int length = str.length();
                            for(int i = 0; i < length; i++)
                                str[i] = tolower(str.at(i));
                            return str.c_str();
                        }

                        bool Utils::equalsIgnoreCase(const char* str1, const char* str2){
                            if((str1 != NULL) && (str2 != NULL)){
                                string s1 = str1;
                                string s2 = str2;
                                stringToLowerCase(s1);
                                stringToLowerCase(s2);
                                return (s1.compare(s2.c_str()) == 0);
                            } else {
                                return false;
                            }
                        }

                        bool Utils::stringEndsWith(const char* str, const char* end){
                            if((str != NULL) && (end != NULL)){
                                string str1 = str;
                                string str2 = end;
                                int length1 = str1.length();
                                int length2 = str2.length();
                                if(length1 >= length2){
                                    return (str2.compare(str1.substr(length1 - length2, length2).c_str()) == 0);
                                }
                            }
                            return false;
                        }

                        bool Utils::isAbsoluteURL(string url){
                            string tmp = url;
                            stringToLowerCase(tmp);
                            return (tmp.substr(0, tmp.find("://")).compare("http") == 0);
                        }

                        char* Utils::getHostURL(const char* url){
                            string tmp = url;
                            if(isAbsoluteURL(tmp)){
                                // busca depois do "http://"
                                tmp = tmp.substr(7, tmp.find("/", 7) - 7);
                                tmp = tmp.substr(0, tmp.find_last_of(":"));
                                return strdup(tmp.c_str());
                            } else {
                                return NULL;
                            }
                        }

                        int Utils::getPortURL(const char* url){
                            string tmp = url;
                            if(isAbsoluteURL(tmp)){
                                // busca depois do "http://"
                                tmp = tmp.substr(7, tmp.find("/", 7) - 7);
                                tmp = tmp.substr(tmp.find_last_of(":") + 1);
                                return stringToInteger(tmp.c_str());
                            } else {
                                return 80;
                            }
                        }

                        char* Utils::getRelativeURL(const char *url){
                            if(url != NULL){
                                string url1 = url;
                                string relative;
                                if(isAbsoluteURL(url1)){
                                    // busca depois de "http://"
                                    relative = url1.substr(url1.find("/", 7));
                                    if(stringEndsWith(relative.c_str(), "/"))
                                        relative = relative.substr(0, relative.length() - 1);
                                } else {
                                    if((url1.length() > 0) && (url1[0] != '/')){
                                        relative = "/";
                                        relative.append(url1);
                                    } else {
                                        relative = url1;
                                    }
                                }
                                return strdup(relative.c_str());
                            } else {
                                return NULL;
                            }
                        }

                        bool Utils::equalsURL(const char* absoluteUrl, const char* url) {
                            if ((absoluteUrl != NULL) && (url != NULL)) {
                                string urlStr = url;
                                if (urlStr.compare(absoluteUrl) == 0) {
                                    return true;
                                } else {
                                    string relative = getRelativeURL(absoluteUrl);
                                    return (relative.compare(url) == 0);
                                }
                            } else {
                                return false;
                            }
                        }

                        void Utils::wait(long mtime){
                            usleep(mtime*1000);
                        }

                        void Utils::setNode(NodePtr parentNode, string nodeName, const char* nodeValue){
                            if(parentNode == NULL)
                                return;

                            IXML_Node* pNode = (IXML_Node*) parentNode;

                            IXML_Node* node = pNode->firstChild;
                            while(node != NULL){
                                if((node->nodeName != NULL) && (nodeName.compare(node->nodeName) == 0)){
                                    setNodeValue(node, nodeValue);
                                    return;
                                }
                                node = node->nextSibling;
                            }
                            IXML_Element *newElem = (IXML_Element*)malloc(sizeof(IXML_Element));
                            memset(newElem, 0, sizeof(IXML_Element));
                            IXML_Node* newNode = (IXML_Node*) newElem;
                            newNode->nodeName = strdup(nodeName.c_str());
                            newNode->nodeType = eELEMENT_NODE;
                            ixmlNode_appendChild(pNode, newNode);

                            IXML_Text *newTxt = (IXML_Text*)malloc(sizeof(IXML_Text));
                            memset(newTxt, 0, sizeof(IXML_Text));
                            IXML_Node* txtNode = (IXML_Node*) newTxt;
                            txtNode->nodeName = strdup(TEXTNODENAME);
                            txtNode->nodeValue = strdup(nodeValue);
                            txtNode->nodeType = eTEXT_NODE;
                            ixmlNode_appendChild(newNode, txtNode);
                        }

                        const char* Utils::getNodeValue(NodePtr parentNode, string nodeName){
                            if(parentNode == NULL)
                                return NULL;

                            IXML_Node* pNode = (IXML_Node*) parentNode;

                            IXML_Node* node = pNode->firstChild;
                            while(node != NULL){
                                if((node->nodeName != NULL) && (nodeName.compare(node->nodeName) == 0)){
                                    return getNodeValue(node);
                                }
                                node = node->nextSibling;
                            }
                            return "";
                        }

                        void Utils::setNodeValue(NodePtr node, const char* nodeValue){
                            if(node == NULL)
                                return;

                            IXML_Node* pNode = (IXML_Node*) node;

                            if((pNode->firstChild != NULL) && (pNode->firstChild->nodeType == eTEXT_NODE)){
                                if(pNode->firstChild->nodeValue != NULL)
                                    free(pNode->firstChild->nodeValue);
                                pNode->firstChild->nodeValue = strdup(nodeValue);
                            }else {
                                IXML_Text *newTxt = (IXML_Text*)malloc(sizeof(IXML_Text));
                                memset(newTxt, 0, sizeof(IXML_Text));
                                IXML_Node* newNode = (IXML_Node*) newTxt;
                                newNode->nodeName = strdup(TEXTNODENAME);
                                newNode->nodeValue = strdup(nodeValue);
                                newNode->nodeType = eTEXT_NODE;
                                ixmlNode_appendChild(pNode, newNode);
                            }
                        }

                        const char* Utils::getNodeValue(NodePtr node){
                            if(node == NULL)
                                return NULL;

                            IXML_Node* pNode = (IXML_Node*) node;

                            if((pNode->firstChild != NULL) && (pNode->firstChild->nodeType == eTEXT_NODE))
                                return pNode->firstChild->nodeValue;
                            else
                                return "";
                        }
                        
                        void Utils::setAttribute(NodePtr node, string attrName, const char* attrValue) {
                            if(node == NULL)
                                return;

                            IXML_Node* pNode = (IXML_Node*) node;

                            IXML_Node* attr = pNode->firstAttr;
                            while (attr != NULL) {
                                if ((attr->nodeName != NULL) && (attrName.compare(attr->nodeName) == 0)) {
                                    if (attr->nodeValue != NULL)
                                        free(attr->nodeValue);
                                    attr->nodeValue = strdup(attrValue);
                                    return;
                                }
                                attr = attr->nextSibling;
                            }

                            IXML_Attr *newAttr = (IXML_Attr*) malloc(sizeof (IXML_Attr));
                            memset(newAttr, 0, sizeof (IXML_Attr));
                            IXML_Node *attrNode = (IXML_Node*) newAttr;
                            attrNode->nodeName = strdup(attrName.c_str());
                            attrNode->nodeValue = strdup(attrValue);
                            attrNode->nodeType = eATTRIBUTE_NODE;
                            attrNode->nextSibling = pNode->firstAttr;
                            if (pNode->firstAttr != NULL)
                                pNode->firstAttr->prevSibling = attrNode;
                            pNode->firstAttr = attrNode;
                        }

                        const char* Utils::getAttributeValue(NodePtr node, string attrName){
                            if(node == NULL)
                                return NULL;

                            IXML_Node* pNode = (IXML_Node*) node;

                            IXML_Node* attr = pNode->firstAttr;
                            while(attr != NULL){
                                if((attr->nodeName != NULL) && (attrName.compare(attr->nodeName) == 0)){
                                    return attr->nodeValue;
                                }
                                attr = attr->nextSibling;
                            }
                            return "";
                        }

                        char* Utils::integerToString(int value){
                            ostringstream out;
                            out << value;
                            return strdup(out.str().c_str());
                        }

                        char* Utils::longToString(long value){
                            ostringstream out;
                            out << value;
                            return strdup(out.str().c_str());
                        }

                        char* Utils::offtToString(off_t value) {
                            ostringstream out;
                            out << value;
                            return strdup(out.str().c_str());
                        }

                        int Utils::stringToInteger(const char* value){
                            return atoi(value);
                        }

                        char* Utils::createRandomUUID(){
                            char uuid[21];
                            time_t t1 = time(NULL);
                            time_t t2 = (time_t)((double)time(NULL) * ((double)rand() / (double)RAND_MAX));
                            sprintf(uuid, "%04x-%04x-%04x-%04x",
                                    (int)(t1 & 0xFFFF),
                                    (int)(((t1 >> 31) | 0xA000) & 0xFFFF),
                                    (int)(t2 & 0xFFFF),
                                    (int)(((t2 >> 31) | 0xE000) & 0xFFFF));
                            return strdup(uuid);
                        }

                        char* Utils::getHostAddress(int n){
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.util.utils");
                            LoggerUtil_info(logger, "Entrando no metodo getHostAddress(n)");

                            int count = 0;

                            int sock = socket(AF_INET, SOCK_DGRAM, 0);
                            if (sock >= 0){
                                char buf[8192];
                                ifconf ifc;
                                ifreq ifr;

                                LoggerUtil_debug(logger, "socket created!");

                                ifc.ifc_len = sizeof(buf);
                                ifc.ifc_buf = buf;

                                if(ioctl(sock, SIOCGIFCONF, &ifc) >= 0) {
                                    int size = ifc.ifc_len / sizeof(ifreq);
                                    for(int i = 0; i < size; i++){
                                        ifr = ifc.ifc_req[i];
                                        if( (ioctl(sock, SIOCGIFFLAGS, &ifr) >= 0) && (ifr.ifr_flags & IFF_UP) && !(ifr.ifr_flags & IFF_LOOPBACK) && (ioctl(sock, SIOCGIFADDR, &ifr) >= 0) ){
                                            if(count == n){
                                                char ip[INET_ADDRSTRLEN];
                                                inet_ntop(AF_INET, &(((sockaddr_in*)&ifr.ifr_addr)->sin_addr), ip, INET_ADDRSTRLEN);

                                                LoggerUtil_debug(logger, "ip: " << ip);

                                                return strdup(ip);
                                            }
                                            count++;
                                        }
                                    }
                                }
                                shutdown(sock,SHUT_RDWR);
                                close(sock);
                            }
                            sock = socket(AF_INET6, SOCK_DGRAM, 0);
                            if (sock >= 0){
                                char buf[8192];
                                ifconf ifc;
                                ifreq ifr;

                                LoggerUtil_debug(logger, "socket created!");

                                ifc.ifc_len = sizeof(buf);
                                ifc.ifc_buf = buf;

                                if(ioctl(sock, SIOCGIFCONF, &ifc) >= 0) {
                                    int size = ifc.ifc_len / sizeof(ifreq);
                                    for(int i = 0; i < size; i++){
                                        ifr = ifc.ifc_req[i];
                                        if( (ioctl(sock, SIOCGIFFLAGS, &ifr) >= 0) && (ifr.ifr_flags & IFF_UP) && !(ifr.ifr_flags & IFF_LOOPBACK) && (ioctl(sock, SIOCGIFADDR, &ifr) >= 0) ){
                                            if(count == n){
                                                char ip[INET6_ADDRSTRLEN];
                                                inet_ntop(AF_INET6, &(((sockaddr_in6*)&ifr.ifr_addr)->sin6_addr), ip, INET6_ADDRSTRLEN);

                                                LoggerUtil_debug(logger, "ip: " << ip);

                                                return strdup(ip);
                                            }
                                            count++;
                                        }
                                    }
                                }
                                shutdown(sock,SHUT_RDWR);
                                close(sock);
                            }

                            return NULL;
                        }

                        int Utils::getHostInterfaces(){
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.util.utils");
                            LoggerUtil_info(logger, "Entrando no metodo getHostInterfaces()");

                            int count = 0;

                            int sock = socket(AF_INET, SOCK_DGRAM, 0);
                            if (sock >= 0){
                                char buf[8192];
                                ifconf ifc;
                                ifreq ifr;

                                LoggerUtil_debug(logger, "socket created!");

                                ifc.ifc_len = sizeof(buf);
                                ifc.ifc_buf = buf;

                                if(ioctl(sock, SIOCGIFCONF, &ifc) >= 0) {
                                    int size = ifc.ifc_len / sizeof(ifreq);
                                    for(int i = 0; i < size; i++){
                                        ifr = ifc.ifc_req[i];
                                        if( (ioctl(sock, SIOCGIFFLAGS, &ifr) >= 0) && (ifr.ifr_flags & IFF_UP) && !(ifr.ifr_flags & IFF_LOOPBACK) && (ioctl(sock, SIOCGIFADDR, &ifr) >= 0) ){
                                            count++;
                                        }
                                    }
                                }
                                shutdown(sock,SHUT_RDWR);
                                close(sock);
                            }
                            sock = socket(AF_INET6, SOCK_DGRAM, 0);
                            if (sock >= 0){
                                char buf[8192];
                                ifconf ifc;
                                ifreq ifr;

                                LoggerUtil_debug(logger, "socket created!");
                                
                                ifc.ifc_len = sizeof(buf);
                                ifc.ifc_buf = buf;

                                if(ioctl(sock, SIOCGIFCONF, &ifc) >= 0) {
                                    int size = ifc.ifc_len / sizeof(ifreq);
                                    for(int i = 0; i < size; i++){
                                        ifr = ifc.ifc_req[i];
                                        if( (ioctl(sock, SIOCGIFFLAGS, &ifr) >= 0) && (ifr.ifr_flags & IFF_UP) && !(ifr.ifr_flags & IFF_LOOPBACK) && (ioctl(sock, SIOCGIFADDR, &ifr) >= 0) ){
                                            count++;
                                        }
                                    }
                                }
                                shutdown(sock,SHUT_RDWR);
                                close(sock);
                            }

                            return count;
                        }

                        bool Utils::isIPv4(const char* ip){
                            if(ip == NULL) {
                                return false;
                            } else {
                                sockaddr_in sa;
                                return (inet_pton(AF_INET, ip, &(sa.sin_addr)) == 1);
                            }
                        }

                        bool Utils::isIPv6(const char* ip){
                            if(ip == NULL) {
                                return false;
                            } else {
                                sockaddr_in6 sa;
                                return (inet_pton(AF_INET6, ip, &(sa.sin6_addr)) == 1);
                            }
                        }

                        int Utils::socketServer(const char* ip, int* port){
                            int sock = -1;
                            if((ip == NULL) || (strlen(ip) == 0) || isIPv4(ip)){
                                sock = socket(PF_INET, SOCK_STREAM, 0);
                                if (sock >= 0){
                                    sockaddr_in sa;
                                    memset(&sa, 0, sizeof(sa));
                                    sa.sin_family = PF_INET;
                                    sa.sin_addr.s_addr = INADDR_ANY;
                                    sa.sin_port = htons(*port);
                                    if((bind(sock, (sockaddr*) &sa, sizeof(sa)) < 0 ) || ( listen(sock, 1024) < 0 )){
                                        shutdown(sock,SHUT_RDWR);
                                        close(sock);
                                        sock = -1;
                                    }

                                    socklen_t length = sizeof(sa);
                                    if(getsockname(sock, (sockaddr*) &sa, &length ) != -1){
                                        *port = (int) ntohs(sa.sin_port);
                                    }
                                    
                                }
                            }else if(isIPv6(ip)){
                                sock = socket(PF_INET6, SOCK_STREAM, 0);
                                if (sock >= 0){
                                    sockaddr_in6 sa;
                                    memset(&sa, 0, sizeof(sa));
                                    sa.sin6_family = PF_INET6;
                                    sa.sin6_addr = in6addr_any;
                                    sa.sin6_port = htons(*port);
                                    if((bind(sock, (sockaddr*) &sa, sizeof(sa)) < 0) || (listen(sock, 1024) < 0)){
                                        shutdown(sock,SHUT_RDWR);
                                        close(sock);
                                        sock = -1;
                                    }

                                    socklen_t length = sizeof(sa);
                                    if(getsockname(sock, (sockaddr*) &sa, &length ) != -1){
                                        *port = (int) ntohs(sa.sin6_port);
                                    }

                                }
                            }

                            return sock;
                        }

                        int Utils::socketConnect(const char* ip, int port){
                            int sock = -1;
                            if(isIPv4(ip)){
                                sock = socket(AF_INET, SOCK_STREAM, 0);
                                if (sock >= 0){
                                    sockaddr_in sa;
                                    memset(&sa, 0, sizeof(sa));
                                    inet_pton(AF_INET, ip, &(sa.sin_addr));
                                    sa.sin_family = AF_INET;
                                    sa.sin_port = htons(port);
                                    if(connect(sock, (sockaddr*) &sa, sizeof(sa)) != 0){
                                        shutdown(sock,SHUT_RDWR);
                                        close(sock);
                                        sock = -1;
                                    }
                                }
                            }else if(isIPv6(ip)){
                                sock = socket(AF_INET6, SOCK_STREAM, 0);
                                if (sock >= 0){
                                    sockaddr_in6 sa;
                                    memset(&sa, 0, sizeof(sa));
                                    inet_pton(AF_INET6, ip, &(sa.sin6_addr));
                                    sa.sin6_family = AF_INET6;
                                    sa.sin6_port = htons(port);
                                    if(connect(sock, (sockaddr*) &sa, sizeof(sa)) != 0){
                                        shutdown(sock,SHUT_RDWR);
                                        close(sock);
                                        sock = -1;
                                    }
                                }
                            }

                            return sock;
                        }

                        off_t Utils::socketSend(int sock, const char* data, off_t length){
                            off_t pos = (off_t) 0;
                            if (length > 0) {
                                int retry = 0;
                                do {
                                    off_t sent = send(sock, data + pos, length, MSG_NOSIGNAL);
                                    if (sent <= 0) {
                                        if (retry >= 10)
                                            return pos;
                                        wait(1000);
                                        retry++;
                                    } else {
                                        pos += sent;
                                        length -= sent;
                                        retry = 0;
                                    }
                                } while (length > 0);
                            }
                            return pos;
                        }

                        const char* Utils::getHomeDir() {
                            static const char* homeDir = NULL;

                            if (homeDir == NULL) {
                                passwd* pw = getpwuid(getuid());

                                if (pw != NULL){
                                    homeDir = pw->pw_dir;
                                } else {
                                    char* var = getenv("HOME");
                                    if (var != NULL)
                                        homeDir = var;
                                }
                                if (homeDir == NULL)
                                    homeDir = "/tmp";
                            }

                            return homeDir;
                        }

                        const char* Utils::mimeTypes[][2] = {
                              {"asf", "video/x-ms-asf"},
                              {"avc", "video/avi"},
                              {"avi", "video/avi"},
                              {"dv", "video/x-dv"},
                              {"divx", "video/x-msvideo"},
                              {"wmv", "video/x-ms-wmv"},
                              {"mjpg", "video/x-motion-jpeg"},
                              {"mjpeg", "video/x-motion-jpeg"},
                              {"mpeg", "video/mpeg"},
                              {"mpg", "video/mpeg"},
                              {"mpe", "video/mpeg"},
                              {"mp2p", "video/mp2p"},
                              {"vob", "video/mpeg"},
                              {"mp2t", "video/mp2t"},
                              {"m1v", "video/mpeg"},
                              {"m2v", "video/mpeg2"},
                              {"mpg2", "video/mpeg2"},
                              {"mpeg2", "video/mpeg2"},
                              {"m4v", "video/mp4"},
                              {"m4p", "video/mp4"},
                              {"ts", "video/mpeg2"},
                              {"ogm", "video/mpeg"},
                              {"mkv", "video/mpeg"},
                              {"rmvb", "video/mpeg"},
                              {"mov", "video/quicktime"},
                              {"hdmov", "video/quicktime"},
                              {"qt", "video/quicktime"},
                              {"xvid", "video/x-msvideo"},
                              {"3gp", "audio/3gpp"},
                              {"aac", "audio/x-aac"},
                              {"ac3", "audio/x-ac3"},
                              {"aif", "audio/aiff"},
                              {"aiff", "audio/aiff"},
                              {"at3p", "audio/x-atrac3"},
                              {"au", "audio/basic"},
                              {"snd", "audio/basic"},
                              {"dts", "audio/x-dts"},
                              {"rmi", "audio/midi"},
                              {"mid", "audio/midi"},
                              {"mp1", "audio/mp1"},
                              {"mp2", "audio/mp2"},
                              {"mp3", "audio/mpeg"},
                              {"mp4", "audio/mp4"},
                              {"m4a", "audio/mp4"},
                              {"ogg", "audio/x-ogg"},
                              {"wav", "audio/x-wav"},
                              {"pcm", "audio/l16"},
                              {"lpcm", "audio/l16"},
                              {"l16", "audio/l16"},
                              {"wma", "audio/x-ms-wma"},
                              {"mka", "audio/mpeg"},
                              {"ra", "audio/x-pn-realaudio"},
                              {"rm", "audio/x-pn-realaudio"},
                              {"ram", "audio/x-pn-realaudio"},
                              {"flac", "audio/x-flac"},
                              {"bmp", "image/bmp"},
                              {"ico", "image/x-icon"},
                              {"gif", "image/gif"},
                              {"jpeg","image/jpeg"},
                              {"jpg", "image/jpeg"},
                              {"jpe", "image/jpeg"},
                              {"jp2", "image/jp2"},
                              {"jpx", "image/jp2"},
                              {"pcd", "image/x-ms-bmp"},
                              {"png", "image/png"},
                              {"pnm", "image/x-portable-anymap"},
                              {"ppm", "image/x-portable-pixmap"},
                              {"qti", "image/x-quicktime"},
                              {"qtf", "image/x-quicktime"},
                              {"qtif","image/x-quicktime"},
                              {"tbn", "image/jpeg"},
                              {"tif", "image/tiff"},
                              {"tiff", "image/tiff"}
                        };

                        const char* Utils::dlnaProfiles[][2] = {
                              {"audio/l16", "LPCM"},
                              {"audio/mpeg", "MP3"},
                              {"audio/mp4", "AAC_ISO_320"},
                              {"audio/x-wav", "WAV"},
                              {"audio/x-ms-wma", "WMAFULL"},
                              {"image/jpeg", "JPEG_LRG"},
                              {"image/jp2", "JPEG_LRG"},
                              {"image/png", "PNG_LRG"},
                              {"image/bmp", "BMP_LRG"},
                              {"image/tiff", "TIFF_LRG"},
                              {"image/gif", "GIF_LRG"},
                              {"video/avi", "AVI"},
                              {"video/mpeg", "MPEG_PS_PAL"},
                              {"video/mp4", "MPEG4_P2_SP_AAC"},
                              {"video/x-ms-wmv", "WMVHIGH_FULL"},
                              {"video/x-msvideo", "AVI"}
                        };

                        const char* Utils::getMimeType(const char* filepath){
                            if(filepath != NULL){
#ifdef HAVE_DLNA
                                dlna_profile_t* p = dlna_guess_media_profile((dlna_t*) dlna, filepath);
                                if (p != NULL)
                                    return p->mime;
#endif
                                string tmp = filepath;
                                tmp = tmp.substr(tmp.find_last_of('.') + 1);
                                stringToLowerCase(tmp);
                                int length = getMimeTypesLength();
                                for(int i=0; i < length; i++){
                                    if(tmp.compare(getMimeTypes()[i][0]) == 0)
                                        return getMimeTypes()[i][1];
                                }
                            }
                            return NULL;
                        }

                        const char* Utils::getDLNAProfile(const char* filepath){
                            if (filepath != NULL){
#ifdef HAVE_DLNA
                                dlna_profile_t* p = dlna_guess_media_profile((dlna_t*) dlna, filepath);
                                if (p != NULL)
                                    return p->id;
#endif
                                const char* mimeType = getMimeType(filepath);
                                if (mimeType != NULL) {
                                    string tmp = mimeType;
                                    int length = getDLNAProfilesLength();
                                    for (int i = 0; i < length; i++) {
                                        if (tmp.compare(getDLNAProfiles()[i][0]) == 0)
                                            return getDLNAProfiles()[i][1];
                                    }
                                }
                                
                            }
                            return "";
                        }

/*
 * Thanks to libdlna project for informations in your dlna.h file
 *
 * DLNA.ORG_PS: Play Speed (integer)
 *   1 normal
 *   0 invalid
 *
 * DLNA.ORG_CI: Convertion Indicator (integer)
 *     1 transcoded
 *     0 not transcoded
 *
 * DLNA.ORG_OP: Operations (string)
 *    00 not time seek range, not range
 *    01 range supported
 *    10 time seek range supported
 *    11 both supported
 *
 * DLNA.ORG_FLAGS: Flags (string) padded with 24 trailing 0s
 *     80000000  31  senderPaced
 *     40000000  30  lsopTimeBasedSeekSupported
 *     20000000  29  lsopByteBasedSeekSupported
 *     10000000  28  playcontainerSupported
 *      8000000  27  s0IncreasingSupported
 *      4000000  26  sNIncreasingSupported
 *      2000000  25  rtspPauseSupported
 *      1000000  24  streamingTransferModeSupported
 *       800000  23  interactiveTransferModeSupported
 *       400000  22  backgroundTransferModeSupported
 *       200000  21  connectionStallingSupported
 *       100000  20  dlnaVersion15Supported
 * Example:
 * DLNA.ORG_FLAGS=01700000[000000000000000000000000] // [] show padding
 */

                        char* Utils::getDLNAAdditionalInfo(const char* filepath){
                            ostringstream info;
                            info << "DLNA.ORG_PN=" << getDLNAProfile(filepath) << ";DLNA.ORG_OP=01;DLNA.ORG_PS=1;DLNA.ORG_CI=0;DLNA.ORG_FLAGS=01700000000000000000000000000000";
                            return strdup(info.str().c_str());
                        }

                        int Utils::getMimeTypesLength(){
                            return sizeof(mimeTypes) / (sizeof(const char*) * 2);
                        }

                        int Utils::getDLNAProfilesLength(){
                            return sizeof(dlnaProfiles) / (sizeof(const char*) * 2);
                        }

                        const char* (*Utils::getMimeTypes())[2] {
                            return mimeTypes;
                        }

                        const char* (*Utils::getDLNAProfiles())[2] {
                            return dlnaProfiles;
                        }

                        const char* Utils::getUPnPClass(const char* mimeType){
                            string tmp = mimeType;
                            if(tmp.find("video") == 0)
                                return "object.item.videoItem.movie";
                            if(tmp.find("audio") == 0)
                                return "object.item.audioItem.musicTrack";
                            if(tmp.find("image") == 0)
                                return "object.item.imageItem.photo";
                            return "unknown";
                        }

                        bool Utils::addArgumentToUPnPAction(bool response, DocumentPtr* result, const char* actionName, const char* servType, const char* name, const char* value) {
                            IXML_Node* node = NULL;
                            IXML_Element* argElem = NULL;
                            IXML_Node* argValueNode = NULL;
                            IXML_Document** dResult = (IXML_Document**) result;

                            if((actionName != NULL) && (servType != NULL)){
                                if(*dResult == NULL){
                                    ostringstream out;
                                    if(response == true) {
                                        out << "<u:" << actionName << "Response xmlns:u=\"" << servType << "\">\r\n</u:" << actionName << "Response>";
                                    } else {
                                        out << "<u:" << actionName << " xmlns:u=\"" << servType << "\">\r\n</u:" << actionName << ">";
                                    }

                                    if (ixmlParseBufferEx(out.str().c_str(), dResult) != IXML_SUCCESS) {
                                        return false;
                                    }
                                }
                                
                                if(name != NULL){
                                    node = ixmlNode_getFirstChild((IXML_Node*) *dResult);
                                    argElem = ixmlDocument_createElement(*dResult, name);
                                    if(value != NULL) {
                                        argValueNode = ixmlDocument_createTextNode(*dResult, value);
                                        ixmlNode_appendChild((IXML_Node*) argElem, argValueNode);
                                    }

                                    ixmlNode_appendChild(node, (IXML_Node*) argElem);
                                }

                                return true;
                            } else {
                                return false;
                            }
                        }

                        bool Utils::addArgumentToPropertySet(DocumentPtr* result, const char* name, const char* value){
                            IXML_Node* node;
                            IXML_Element* argElem;
                            IXML_Element* propElem;
                            IXML_Node* argValueNode;
                            IXML_Document** dResult = (IXML_Document**) result;

                            if (name != NULL) {
                                if((*dResult == NULL) && (ixmlParseBufferEx("<e:propertyset xmlns:e=\"urn:schemas-upnp-org:event-1-0\"></e:propertyset>", dResult) != IXML_SUCCESS)) {
                                    return false;
                                }

                                node = ixmlNode_getFirstChild((IXML_Node*) *dResult);

                                propElem = ixmlDocument_createElement(*dResult, "e:property");
                                argElem = ixmlDocument_createElement(*dResult, name);

                                if (value != NULL) {
                                    argValueNode = ixmlDocument_createTextNode(*dResult, value);
                                    ixmlNode_appendChild((IXML_Node*) argElem, argValueNode);
                                }

                                ixmlNode_appendChild((IXML_Node*) propElem, (IXML_Node*) argElem);
                                ixmlNode_appendChild(node, (IXML_Node*) propElem);

                                return true;
                            } else {
                                return false;
                            }
                        }

                        static void exitSignal(int in) {
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.util.utils");
                            LoggerUtil_info(logger, "Entrando no metodo exitSignal");

                            Utils::stopUPnP();
                            abort();
                        }

                        char* Utils::initializeUPnP(){
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.util.utils");
                            LoggerUtil_info(logger, "Entrando no metodo initializeUPnP()");

                            if (initializedUPnP == false) {
                                signal(SIGTERM, exitSignal);
                                signal(SIGINT, exitSignal);
                                signal(SIGKILL, exitSignal);
                                signal(SIGHUP, exitSignal);
                                signal(SIGSEGV, exitSignal);
                                //signal(SIGABRT, exitSignal);
#ifdef HAVE_DLNA
                                dlna = dlna_init();
                                dlna_register_all_media_profiles((dlna_t*)dlna);
#endif
                                string location;

                                int result = 0;

                                LoggerUtil_debug(logger, "UpnpInit(NULL, 0)");
                                if ((result = UpnpInit(NULL, 0)) == UPNP_E_SUCCESS) {
                                    string url = "http://";
                                    url.append(UpnpGetServerIpAddress());
                                    url.append(":");
                                    url.append(Utils::integerToString(UpnpGetServerPort()));
                                    location = url;
                                } else {
                                    LoggerUtil_debug(logger, "UpnpInit > result: " << UPnPStatus::codeToString(result));
                                    int interfaces = Utils::getHostInterfaces();
                                    for (int i = 0; i < interfaces; i++) {
                                        string ip = Utils::getHostAddress(i);
                                        LoggerUtil_debug(logger, "UpnpInit(" << ip.c_str() << ", 0)");
                                        if ((result = UpnpInit(ip.c_str(), 0)) == UPNP_E_SUCCESS) {
                                            string url = "http://";
                                            url.append(ip);
                                            url.append(":");
                                            url.append(Utils::integerToString(UpnpGetServerPort()));
                                            location = url;
                                            i = interfaces;
                                        }
                                        LoggerUtil_debug(logger, "UpnpInit > result: " << UPnPStatus::codeToString(result));
                                    }

                                    if (location.length() <= 0) {
                                        for (int i = 0; i < interfaces; i++) {
                                            string ip = Utils::getHostAddress(i);
                                            LoggerUtil_debug(logger, "UpnpInit(" << ip.c_str() << ", 4040)");
                                            if ((result = UpnpInit(ip.c_str(), 4040)) == UPNP_E_SUCCESS) {
                                                string url = "http://";
                                                url.append(ip);
                                                url.append(":");
                                                url.append("4040");
                                                location = url;
                                                i = interfaces;
                                            }
                                            LoggerUtil_debug(logger, "UpnpInit > result: " << UPnPStatus::codeToString(result));
                                        }
                                    }
                                }

                                if (UpnpIsWebserverEnabled() == FALSE)
                                    UpnpEnableWebserver(TRUE);

                                UpnpAddVirtualDir("/");

                                initializedUPnP = true;

                                return strdup(location.c_str());
                            } else {
                                return NULL;
                            }
                        }

                        void Utils::stopUPnP() {
                            UpnpFinish();
                            initializedUPnP = false;
#ifdef HAVE_DLNA
                            if (dlna != NULL) {
                                dlna_uninit((dlna_t*)dlna);
                                dlna = NULL;
                            }
#endif
                        }

                    }
                }
            }
        }
    }
}
