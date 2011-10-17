#include "honesconfig.h"

#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <upnp/upnp.h>
#include <upnp/ixml.h>
#include "mediaserver/TVMediaServer.h"
#include "mediaserver/File.h"
#include "mediaserver/Directory.h"
#include "mediaserver/TVMSContentDirectory.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediaserver {

                        const char *TVMSContentDirectory::SERVICE_TYPE = "urn:schemas-upnp-org:service:ContentDirectory:1";

                        const char *TVMSContentDirectory::GET_SEARCH_CAPABILITIES = "GetSearchCapabilities";
                        const char *TVMSContentDirectory::GET_SORT_CAPABILITIES = "GetSortCapabilities";
                        const char *TVMSContentDirectory::GET_SYSTEM_UPDATE_ID = "GetSystemUpdateID";
                        const char *TVMSContentDirectory::BROWSE = "Browse";

                        const char *TVMSContentDirectory::SEARCH_CAPS = "SearchCaps";
                        const char *TVMSContentDirectory::SORT_CAPS = "SortCaps";
                        const char *TVMSContentDirectory::ID = "Id";
                        const char *TVMSContentDirectory::OBJECT_ID = "ObjectID";
                        const char *TVMSContentDirectory::BROWSE_FLAG = "BrowseFlag";
                        const char *TVMSContentDirectory::FILTER = "Filter";
                        const char *TVMSContentDirectory::STARTING_INDEX = "StartingIndex";
                        const char *TVMSContentDirectory::REQUESTED_COUNT = "RequestedCount";
                        const char *TVMSContentDirectory::SORT_CRITERIA = "SortCriteria";
                        const char *TVMSContentDirectory::RESULT = "Result";
                        const char *TVMSContentDirectory::NUMBER_RETURNED = "NumberReturned";
                        const char *TVMSContentDirectory::TOTAL_MATCHES = "TotalMatches";
                        const char *TVMSContentDirectory::UPDATE_ID = "UpdateID";

                        const char *TVMSContentDirectory::SYSTEM_UPDATE_ID = "SystemUpdateID";
                        const char *TVMSContentDirectory::BROWSE_METADATA = "BrowseMetadata";
                        const char *TVMSContentDirectory::BROWSE_DIRECT_CHILDREN = "BrowseDirectChildren";

                        const char *TVMSContentDirectory::CONTENT_DIRECTORY = "contentDirectory";

                        const char *TVMSContentDirectory::SCPD =
                                "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
                                "<scpd xmlns=\"urn:schemas-upnp-org:service-1-0\">"
                                "  <specVersion>"
                                "    <major>1</major>"
                                "    <minor>0</minor>"
                                "  </specVersion>"
                                "  <actionList>"
                                "    <action>"
                                "      <name>Browse</name>"
                                "      <argumentList>"
                                "        <argument>"
                                "          <name>ObjectID</name>"
                                "          <direction>in</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_ObjectID</relatedStateVariable>"
                                "        </argument>"
                                "        <argument>"
                                "          <name>BrowseFlag</name>"
                                "          <direction>in</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_BrowseFlag</relatedStateVariable>"
                                "       </argument>"
                                "        <argument>"
                                "          <name>Filter</name>"
                                "          <direction>in</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_Filter</relatedStateVariable>"
                                "        </argument>"
                                "        <argument>"
                                "          <name>StartingIndex</name>"
                                "          <direction>in</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_Index</relatedStateVariable>"
                                "        </argument>"
                                "        <argument>"
                                "          <name>RequestedCount</name>"
                                "          <direction>in</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_Count</relatedStateVariable>"
                                "        </argument>"
                                "        <argument>"
                                "          <name>SortCriteria</name>"
                                "          <direction>in</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_SortCriteria</relatedStateVariable>"
                                "       </argument>"
                                "        <argument>"
                                "          <name>Result</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_Result</relatedStateVariable>"
                                "        </argument>"
                                "        <argument>"
                                "          <name>NumberReturned</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_Count</relatedStateVariable>"
                                "        </argument>"
                                "        <argument>"
                                "          <name>TotalMatches</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_Count</relatedStateVariable>"
                                "        </argument>"
                                "        <argument>"
                                "          <name>UpdateID</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>A_ARG_TYPE_UpdateID</relatedStateVariable>"
                                "        </argument>"
                                "      </argumentList>"
                                "    </action>"
                                "    <action>"
                                "      <name>GetSystemUpdateID</name>"
                                "      <argumentList>"
                                "        <argument>"
                                "          <name>Id</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>SystemUpdateID</relatedStateVariable>"
                                "        </argument>"
                                "      </argumentList>"
                                "    </action>"
                                "    <action>"
                                "      <name>GetSearchCapabilities</name>"
                                "      <argumentList>"
                                "        <argument>"
                                "          <name>SearchCaps</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>SearchCapabilities</relatedStateVariable>"
                                "        </argument>"
                                "      </argumentList>"
                                "    </action>"
                                "    <action>"
                                "      <name>GetSortCapabilities</name>"
                                "      <argumentList>"
                                "        <argument>"
                                "          <name>SortCaps</name>"
                                "          <direction>out</direction>"
                                "          <relatedStateVariable>SortCapabilities</relatedStateVariable>"
                                "        </argument>"
                                "      </argumentList>"
                                "    </action>"
                                "  </actionList>"
                                "  <serviceStateTable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>A_ARG_TYPE_BrowseFlag</name>"
                                "      <dataType>string</dataType>"
                                "      <allowedValueList>"
                                "        <allowedValue>BrowseMetadata</allowedValue>"
                                "        <allowedValue>BrowseDirectChildren</allowedValue>"
                                "      </allowedValueList>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"yes\">"
                                "      <name>SystemUpdateID</name>"
                                "      <dataType>ui4</dataType>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>A_ARG_TYPE_Count</name>"
                                "      <dataType>ui4</dataType>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>A_ARG_TYPE_SortCriteria</name>"
                                "      <dataType>string</dataType>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>SortCapabilities</name>"
                                "      <dataType>string</dataType>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>A_ARG_TYPE_Index</name>"
                                "      <dataType>ui4</dataType>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>A_ARG_TYPE_ObjectID</name>"
                                "      <dataType>string</dataType>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>A_ARG_TYPE_UpdateID</name>"
                                "      <dataType>ui4</dataType>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>A_ARG_TYPE_Result</name>"
                                "      <dataType>string</dataType>"
                                "    </stateVariable>"
                                "   <stateVariable sendEvents=\"no\">"
                                "      <name>SearchCapabilities</name>"
                                "      <dataType>string</dataType>"
                                "    </stateVariable>"
                                "    <stateVariable sendEvents=\"no\">"
                                "      <name>A_ARG_TYPE_Filter</name>"
                                "      <dataType>string</dataType>"
                                "    </stateVariable>"
                                "  </serviceStateTable>"
                                "</scpd>";

                        TVMSContentDirectory::TVMSContentDirectory(TVMediaServer* tvs){
                            logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.mediarenderer.tvmscontentdirectory");
                            LoggerUtil_info(logger, "Entrando no construtor TVMSContentDirectory(tvs)!");

                            setMediaServer(tvs);

                            nextContentID = -1;

                            UPnPService* serv = getService();

                            bool ret = serv->loadSCPD(SCPD);
                            if (ret == false)
                                throw "Invalid description";
                            serv->setUPnPActionListener(this);
                            serv->setUPnPQueryListener(this);

                            httpServer = UPnPHttpServer::getInstance();
                            httpServer->addUPnPHttpListener(this);

                            initializeUPnPStateVariables();

                            LoggerUtil_info(logger, "Saindo do construtor TVMSContentDirectory(tvs)!");
                        }

                        TVMSContentDirectory::~TVMSContentDirectory(){
                            directoryList.clear();
                        }

                        void TVMSContentDirectory::initializeUPnPStateVariables(){
                            getService()->getUPnPStateVariable(SYSTEM_UPDATE_ID)->setValue("0");
                        }

                        TVMediaServer* TVMSContentDirectory::getMediaServer() {
                            return tvServer;
                        }

                        UPnPService* TVMSContentDirectory::getService(){
                            return getMediaServer()->getUPnPService(SERVICE_TYPE);
                        }

                        bool TVMSContentDirectory::actionRequest(UPnPAction *action){
                            string actionName = action->getName();

                            if (actionName.compare(GET_SEARCH_CAPABILITIES) == 0)
                                return actionGetSearchCapabilities(action);

                            if (actionName.compare(GET_SORT_CAPABILITIES) == 0)
                                return actionGetSortCapabilities(action);

                            if (actionName.compare(GET_SYSTEM_UPDATE_ID) == 0)
                                return actionGetSystemUpdateID(action);

                            if (actionName.compare(BROWSE) == 0)
                                return actionBrowse(action);

                            return false;
                        }

                        bool TVMSContentDirectory::actionGetSearchCapabilities(UPnPAction *action){
                            LoggerUtil_info(logger, "Entrando no metodo actionGetSearchCapabilities(action)!");

                            action->getUPnPArgument(SEARCH_CAPS)->setValue("");
                            return true;
                        }

                        bool TVMSContentDirectory::actionGetSortCapabilities(UPnPAction *action){
                            LoggerUtil_info(logger, "Entrando no metodo actionGetSortCapabilities(action)!");

                            action->getUPnPArgument(SORT_CAPS)->setValue("");
                            return true;
                        }

                        bool TVMSContentDirectory::actionGetSystemUpdateID(UPnPAction *action){
                            LoggerUtil_info(logger, "Entrando no metodo actionGetSystemUpdateID(action)!");

                            action->getUPnPArgument(ID)->setValue(getService()->getUPnPStateVariable(SYSTEM_UPDATE_ID)->getValue());
                            return true;
                        }

                        bool TVMSContentDirectory::actionBrowse(UPnPAction *action){
                            LoggerUtil_info(logger, "Entrando no metodo actionBrowse(action)!");

                            if(getContentDirectories()->size() > 0){
                                int objID = Utils::stringToInteger(action->getUPnPArgument(OBJECT_ID)->getValue());
                                if(objID < Utils::stringToInteger(getContentDirectories()->getDirectory(0)->getID()))
                                    objID = Utils::stringToInteger(getContentDirectories()->getDirectory(0)->getID());

                                IXML_Element* didlElem = (IXML_Element*) malloc(sizeof(IXML_Element));
                                memset(didlElem, 0, sizeof(IXML_Element));
                                IXML_Node* didlNode = (IXML_Node*)didlElem;
                                didlNode->nodeName = strdup("DIDL-Lite");
                                didlNode->nodeType = eELEMENT_NODE;

                                Utils::setAttribute(didlNode, "xmlns:upnp", "urn:schemas-upnp-org:metadata-1-0/upnp/");
                                Utils::setAttribute(didlNode, "xmlns:dc", "http://purl.org/dc/elements/1.1/");
                                Utils::setAttribute(didlNode, "xmlns", "urn:schemas-upnp-org:metadata-1-0/DIDL-Lite/");

                                string browse = action->getUPnPArgument(BROWSE_FLAG)->getValue();
                                string filter = action->getUPnPArgument(FILTER)->getValue();

                                LoggerUtil_debug(logger, "Filter: " << filter.c_str());
                                LoggerUtil_debug(logger, "BrowseFlag: " << browse.c_str());

                                if(browse.compare(BROWSE_METADATA) == 0){
                                    File* file = getContentDirectories()->getFileByID(Utils::integerToString(objID));
                                    if(file != NULL){
                                        addUPnPItem(didlNode, file, false, getURI(action, file), filter);

                                        action->getUPnPArgument(RESULT)->setValue(ixmlNodetoString(didlNode));
                                        action->getUPnPArgument(NUMBER_RETURNED)->setValue("1");
                                        action->getUPnPArgument(TOTAL_MATCHES)->setValue("1");
                                        action->getUPnPArgument(UPDATE_ID)->setValue("0");
                                        ixmlNode_free(didlNode);
                                        return true;
                                    } else {
                                        Directory* dir = getContentDirectories()->getDirectoryByID(Utils::integerToString(objID));
                                        if(dir != NULL){
                                            addUPnPContainer(didlNode, dir, true, true);

                                            action->getUPnPArgument(RESULT)->setValue(ixmlNodetoString(didlNode));
                                            action->getUPnPArgument(NUMBER_RETURNED)->setValue("1");
                                            action->getUPnPArgument(TOTAL_MATCHES)->setValue("1");
                                            action->getUPnPArgument(UPDATE_ID)->setValue("0");
                                            ixmlNode_free(didlNode);
                                            return true;
                                        }
                                    }
                                } else if(browse.compare(BROWSE_DIRECT_CHILDREN) == 0){
                                    File* file = getContentDirectories()->getFileByID(Utils::integerToString(objID));
                                    if(file != NULL){
                                        ixmlNode_free(didlNode);
                                        return false;
                                    } else {
                                        Directory* dir = getContentDirectories()->getDirectoryByID(Utils::integerToString(objID));
                                        if(dir != NULL){
                                            int count = 0;
                                            int startingIndex = Utils::stringToInteger(action->getUPnPArgument(STARTING_INDEX)->getValue());
                                            int requestedCount = Utils::stringToInteger(action->getUPnPArgument(REQUESTED_COUNT)->getValue());
                                            int size = dir->getFiles()->size();
                                            for(int i=0; (i < size) && ((requestedCount == 0) || (count < (startingIndex + requestedCount))); i++){
                                                if(count >= startingIndex){
                                                    File* file = dir->getFiles()->getFile(i);
                                                    addUPnPItem(didlNode, file, true, getURI(action, file), filter);
                                                }
                                                count++;
                                            }
                                            size = dir->getDirectories()->size();
                                            for(int i=0; (i < size) && ((requestedCount == 0) || (count < (startingIndex + requestedCount))); i++){
                                                if(count >= startingIndex){
                                                    Directory* subdir = dir->getDirectories()->getDirectory(i);
                                                    addUPnPContainer(didlNode, subdir, false, true);
                                                }
                                                count++;
                                            }

                                            action->getUPnPArgument(RESULT)->setValue(ixmlNodetoString(didlNode));
                                            action->getUPnPArgument(NUMBER_RETURNED)->setValue(count - startingIndex);
                                            action->getUPnPArgument(TOTAL_MATCHES)->setValue(dir->getFiles()->size() + dir->getDirectories()->size());
                                            action->getUPnPArgument(UPDATE_ID)->setValue("0");
                                            ixmlNode_free(didlNode);
                                            return true;
                                        }
                                    }
                                } else {
                                    action->setStatus(UPNP_E_INVALID_ARGUMENT);
                                }
                                ixmlNode_free(didlNode);
                            }
                            return false;
                        }

                        int TVMSContentDirectory::getNextContentID(){
                            nextContentID++;
                            return nextContentID;
                        }

                        void TVMSContentDirectory::addContentDirectory(const char* name){
                            if(name == NULL)
                                return;
                            
                            string dirpath = name;
                            if(dirpath[dirpath.length() - 1] == '/')
                                dirpath = dirpath.substr(0, dirpath.length() - 1);
                            int size = getContentDirectories()->size();
                            for(int i=0; i < size; i++){
                                Directory* dir = getContentDirectories()->getDirectory(i);
                                if(dirpath.compare(dir->getPath()) == 0){
                                    return;
                                }
                            }
                            
                            File* f = new File(dirpath.c_str());
                            if(f->isDirectory()){
                                Directory* dir = new Directory(this, dirpath.c_str());
                                if(dir->isEmpty() == false){
                                    getContentDirectories()->addDirectory(dir);
                                    getService()->getUPnPStateVariable(SYSTEM_UPDATE_ID)->setValue( Utils::stringToInteger(getService()->getUPnPStateVariable(SYSTEM_UPDATE_ID)->getValue()) + 1 );
                                } else {
                                    delete dir;
                                }
                            }
                            delete f;
                        }

                        void TVMSContentDirectory::removeContentDirectory(const char* name){
                            if(name == NULL)
                                return;
                            
                            string dirpath = name;
                            if(dirpath[dirpath.length() - 1] == '/')
                                dirpath = dirpath.substr(0, dirpath.length() - 1);
                            int size = getContentDirectories()->size();
                            for(int i=0; i < size; i++){
                                Directory* dir = getContentDirectories()->getDirectory(i);
                                if(dirpath.compare(dir->getPath()) == 0){
                                    getContentDirectories()->removeDirectory(dir);
                                    if(dir != NULL){
                                        delete dir;
                                        dir = NULL;
                                    }
                                    getService()->getUPnPStateVariable(SYSTEM_UPDATE_ID)->setValue( Utils::stringToInteger(getService()->getUPnPStateVariable(SYSTEM_UPDATE_ID)->getValue()) + 1 );
                                    return;
                                }
                            }
                        }

                        DirectoryList* TVMSContentDirectory::getContentDirectories(){
                            return &directoryList;
                        }

                        bool TVMSContentDirectory::httpGetInfo(const char* filename, off_t* fileLength, time_t* lastModified, bool* isDirectory, bool* isReadable, char** contentType, char** absolutePath){
                            LoggerUtil_info(logger, "Entrando no metodo httpGetInfo()!");

                            if( filename != NULL ){
                                string fileId = filename;

                                LoggerUtil_debug(logger, "filename: " << fileId);

                                if (fileId.find(CONTENT_DIRECTORY) != string::npos) {
                                    fileId = fileId.substr(fileId.find_last_of('/') + 1);
                                    fileId = fileId.substr(0, fileId.find('.'));

                                    LoggerUtil_debug(logger, "fileId: " << fileId);

                                    File* file = getContentDirectories()->getFileByID(fileId.c_str());
                                    if (file != NULL) {
                                        *fileLength = file->getLength();
                                        *lastModified = file->getLastModified();
                                        *isDirectory = false;
                                        *isReadable = true;
                                        *contentType = ixmlCloneDOMString(file->getMimeType());
                                        *absolutePath = strdup(file->getPath());

                                        LoggerUtil_debug(logger, "result: true\ncontentType: " << *contentType);

                                        return true;
                                    }
                                }
                            }

                            LoggerUtil_debug(logger, "result: false");

                            return false;
                        }

                        UPnPHttpFileHandle* TVMSContentDirectory::httpOpen(const char* filename, enum OpenFileMode mode){
                            LoggerUtil_info(logger, "Entrando no metodo httpOpen()!");

                            if( (mode == READ) && (filename != NULL) ){
                                string fileId = filename;
                                if (fileId.find(CONTENT_DIRECTORY) != string::npos) {
                                    fileId = fileId.substr(fileId.find_last_of('/') + 1);
                                    fileId = fileId.substr(0, fileId.find('.'));
                                    File* file = getContentDirectories()->getFileByID(fileId.c_str());
                                    if (file != NULL) {
                                        UPnPHttpFileHandle* f = new UPnPHttpFileHandle;
                                        f->pos = 0;
                                        f->type = UPnPHttpFileHandle::FILE_LOCAL;
                                        f->local.file = file;
                                        f->local.fd = open(file->getPath(), O_RDONLY | O_NONBLOCK | O_SYNC | O_NDELAY);
                                        if(f->local.fd != -1){
                                            return f;
                                        } else {
                                            delete f;
                                        }
                                    }
                                }
                            }
                            return NULL;
                        }

                        off_t TVMSContentDirectory::httpRead(UPnPHttpFileHandle* fh, char* buf, off_t buflen){
                            LoggerUtil_info(logger, "Entrando no metodo httpRead()!");

                            if((fh != NULL) && (fh->type == UPnPHttpFileHandle::FILE_LOCAL)){
                                File* f = (File*) fh->local.file;
                                if(f != NULL){
                                    if(getContentDirectories()->getFileByID(f->getID()) == f){
                                        off_t length = read(fh->local.fd, buf, buflen);
                                        if(length >= 0){
                                            fh->pos += length;
                                            return length;
                                        }
                                    }
                                }
                            }
                            return -1;
                        }

                        off_t TVMSContentDirectory::httpWrite(UPnPHttpFileHandle* fh, char* buf, off_t buflen){
                            LoggerUtil_info(logger, "Entrando no metodo httpWrite()!");

                            return -1;
                        }

                        bool TVMSContentDirectory::httpSeek(UPnPHttpFileHandle* fh, off_t offset, int origin){
                            LoggerUtil_info(logger, "Entrando no metodo httpSeek()!");

                            if((fh != NULL) && (fh->type == UPnPHttpFileHandle::FILE_LOCAL)){
                                File* f = (File*) fh->local.file;
                                if(f != NULL){
                                    if(getContentDirectories()->getFileByID(f->getID()) == f){
                                        off_t newpos = (off_t) -1;
                                        switch(origin){
                                            case SEEK_SET:
                                                newpos = offset;
                                                break;
                                            case SEEK_CUR:
                                                newpos = fh->pos + offset;
                                                break;
                                            case SEEK_END:
                                                newpos = f->getLength() + offset;
                                                break;
                                        }
                                        if( (newpos >= 0) && (newpos <= f->getLength()) ){
                                            if(lseek(fh->local.fd, newpos, SEEK_SET) != -1){
                                                fh->pos = newpos;
                                                return true;
                                            }
                                        }
                                    }
                                }
                            }
                            return false;
                        }

                        bool TVMSContentDirectory::httpClose(UPnPHttpFileHandle* fh){
                            LoggerUtil_info(logger, "Entrando no metodo httpClose()!");

                            if((fh != NULL) && (fh->type == UPnPHttpFileHandle::FILE_LOCAL)){
                                File* f = (File*) fh->local.file;
                                if(f != NULL){
                                    if(getContentDirectories()->getFileByID(f->getID()) == f){
                                        close(fh->local.fd);
                                        delete fh;
                                        return true;
                                    }
                                }
                            }
                            return false;
                        }

                        bool TVMSContentDirectory::variableRequest(UPnPStateVariable *stateVar){
                            LoggerUtil_info(logger, "Entrando no metodo variableRequest(stateVar)!");

                            UPnPStateVariable* serviceStateVar = getService()->getUPnPStateVariable(stateVar->getName());
                            if (serviceStateVar == NULL)
                                return false;

                            stateVar->setValue(serviceStateVar->getValue());

                            return true;
                        }

                        void TVMSContentDirectory::setMediaServer(TVMediaServer* tvs) {
                            tvServer = tvs;
                        }

                        char* TVMSContentDirectory::getURI(UPnPAction* action, File* file) {
                            LoggerUtil_info(logger, "Entrando no metodo getURI(action, file)!");

                            ostringstream uri;
                            string controlHost = action->getControlHost();

                            LoggerUtil_debug(logger, "controlHost: " << controlHost.c_str());

                            if (controlHost.length() <= 0) {
                                uri << "http://" << Utils::getHostURL(getMediaServer()->getLocation()) << ":" << httpServer->getPort() << "/" << CONTENT_DIRECTORY << "/" << file->getID() << "." << file->getSuffix();
                            } else {
                                string ipURI;
                                int interfaces = Utils::getHostInterfaces();
                                for (int i = 0; i < interfaces; i++) {
                                    string ip = Utils::getHostAddress(i);
                                    if ((ipURI.length() <= 0) || (abs(controlHost.compare(ip)) < abs(controlHost.compare(ipURI))))
                                        ipURI = ip;
                                }
                                uri << "http://" << ipURI.c_str() << ":" << httpServer->getPort() << "/" << CONTENT_DIRECTORY << "/" << file->getID() << "." << file->getSuffix();
                            }
                            return strdup(uri.str().c_str());
                        }

                        void TVMSContentDirectory::addUPnPItem(NodePtr didl, File* file, bool restricted, char* uri, string filter) {
                            IXML_Node* didlNode = (IXML_Node*) didl;

                            IXML_Element *itemElem = (IXML_Element*) malloc(sizeof (IXML_Element));
                            memset(itemElem, 0, sizeof (IXML_Element));
                            IXML_Node* itemNode = (IXML_Node*) itemElem;
                            itemNode->nodeName = strdup("item");
                            itemNode->nodeType = eELEMENT_NODE;

                            if (restricted == true)
                                Utils::setAttribute(itemNode, "restricted", "true");
                            else
                                Utils::setAttribute(itemNode, "restricted", "false");
                            
                            Utils::setAttribute(itemNode, "parentID", file->getParentID());
                            Utils::setAttribute(itemNode, "id", file->getID());

                            Utils::setNode(itemNode, "upnp:class", file->getUPnPClass());
                            
                            Utils::setNode(itemNode, "dc:title", (file->getTitle() != NULL) && (strlen(file->getTitle()) > 0) ? file->getTitle() : file->getName());

                            if ( ((filter.find("*") != string::npos) || (filter.find("dc:creator") != string::npos)) && (file->getAuthor() != NULL) && (strlen(file->getAuthor()) > 0) )
                                Utils::setNode(itemNode, "dc:creator", file->getAuthor());

                            if ( ((filter.find("*") != string::npos) || (filter.find("dc:date") != string::npos)) && (file->getYear() > 0) )
                                Utils::setNode(itemNode, "dc:date", string(Utils::integerToString(file->getYear())).append("-01-01").c_str());

                            if ( ((filter.find("*") != string::npos) || (filter.find("upnp:artist") != string::npos)) && (file->getAuthor() != NULL) && (strlen(file->getAuthor()) > 0) )
                                Utils::setNode(itemNode, "upnp:artist", file->getAuthor());

                            if ( ((filter.find("*") != string::npos) || (filter.find("upnp:album") != string::npos)) && (file->getAlbum() != NULL) && (strlen(file->getAlbum()) > 0) )
                                Utils::setNode(itemNode, "upnp:album", file->getAlbum());

                            if ( ((filter.find("*") != string::npos) || (filter.find("upnp:genre") != string::npos)) && (file->getGenre() != NULL) && (strlen(file->getGenre()) > 0) )
                                Utils::setNode(itemNode, "upnp:genre", file->getGenre());

                            if ( ((filter.find("*") != string::npos) || (filter.find("upnp:originalTrackNumber") != string::npos)) && (file->getTrack() > 0) )
                                Utils::setNode(itemNode, "upnp:originalTrackNumber", Utils::integerToString(file->getTrack()));

                            ixmlNode_appendChild(didlNode, itemNode);

                            if ((filter.find("*") != string::npos) || (filter.find("res") != string::npos)) {
                                if (filter.find("ALLIP") != string::npos) {
                                    int interfaces = Utils::getHostInterfaces();
                                    for (int i = 0; i < interfaces; i++) {
                                        IXML_Element* resElem = (IXML_Element*) malloc(sizeof (IXML_Element));
                                        memset(resElem, 0, sizeof (IXML_Element));
                                        IXML_Node* resNode = (IXML_Node*) resElem;
                                        resNode->nodeName = strdup("res");
                                        resNode->nodeType = eELEMENT_NODE;

                                        if ((filter.find("*") != string::npos) || (filter.find("@size") != string::npos))
                                            Utils::setAttribute(resNode, "size", Utils::offtToString(file->getLength()));

                                        if ( ((filter.find("*") != string::npos) || (filter.find("@bitrate") != string::npos)) && (file->getBitrate() > 0) )
                                            Utils::setAttribute(resNode, "bitrate", Utils::integerToString(file->getBitrate()));
                                        
                                        if ( ((filter.find("*") != string::npos) || (filter.find("@sampleFrequency") != string::npos)) && (file->getSamplerate() > 0) )
                                            Utils::setAttribute(resNode, "sampleFrequency", Utils::integerToString(file->getSamplerate()));

                                        if ( ((filter.find("*") != string::npos) || (filter.find("@nrAudioChannels") != string::npos)) && (file->getChannels() > 0) )
                                            Utils::setAttribute(resNode, "nrAudioChannels", Utils::integerToString(file->getChannels()));

                                        if ( ((filter.find("*") != string::npos) || (filter.find("@duration") != string::npos)) && (file->getDuration() != NULL) && (strlen(file->getDuration()) > 0) )
                                            Utils::setAttribute(resNode, "duration", file->getDuration());

                                        if ( ((filter.find("*") != string::npos) || (filter.find("@resolution") != string::npos)) && (file->getWidth() > 0) && (file->getHeight() > 0) )
                                            Utils::setAttribute(resNode, "resolution", string(Utils::integerToString(file->getWidth())).append("x").append(Utils::integerToString(file->getHeight())).c_str());

                                        ostringstream protocolInfo;
                                        protocolInfo << "http-get:*:" << file->getMimeType() << ":" << file->getDLNAAdditionalInfo();
                                        Utils::setAttribute(resNode, "protocolInfo", protocolInfo.str().c_str());

                                        ostringstream resURI;
                                        resURI << "http://" << Utils::getHostAddress(i) << ":" << httpServer->getPort() << "/" << CONTENT_DIRECTORY << "/" << file->getID() << "." << file->getSuffix();

                                        Utils::setNodeValue(resNode, resURI.str().c_str());

                                        LoggerUtil_debug(logger, resURI.str().c_str());

                                        ixmlNode_appendChild(itemNode, resNode);
                                    }
                                } else {
                                    IXML_Element* resElem = (IXML_Element*) malloc(sizeof (IXML_Element));
                                    memset(resElem, 0, sizeof (IXML_Element));
                                    IXML_Node* resNode = (IXML_Node*) resElem;
                                    resNode->nodeName = strdup("res");
                                    resNode->nodeType = eELEMENT_NODE;

                                    if ((filter.find("*") != string::npos) || (filter.find("@size") != string::npos))
                                        Utils::setAttribute(resNode, "size", Utils::offtToString(file->getLength()));

                                    if (((filter.find("*") != string::npos) || (filter.find("@bitrate") != string::npos)) && (file->getBitrate() > 0))
                                        Utils::setAttribute(resNode, "bitrate", Utils::integerToString(file->getBitrate()));

                                    if (((filter.find("*") != string::npos) || (filter.find("@sampleFrequency") != string::npos)) && (file->getSamplerate() > 0))
                                        Utils::setAttribute(resNode, "sampleFrequency", Utils::integerToString(file->getSamplerate()));

                                    if (((filter.find("*") != string::npos) || (filter.find("@nrAudioChannels") != string::npos)) && (file->getChannels() > 0))
                                        Utils::setAttribute(resNode, "nrAudioChannels", Utils::integerToString(file->getChannels()));

                                    if (((filter.find("*") != string::npos) || (filter.find("@duration") != string::npos)) && (file->getDuration() != NULL) && (strlen(file->getDuration()) > 0))
                                        Utils::setAttribute(resNode, "duration", file->getDuration());

                                    if (((filter.find("*") != string::npos) || (filter.find("@resolution") != string::npos)) && (file->getWidth() > 0) && (file->getHeight() > 0))
                                        Utils::setAttribute(resNode, "resolution", string(Utils::integerToString(file->getWidth())).append("x").append(Utils::integerToString(file->getHeight())).c_str());

                                    ostringstream protocolInfo;
                                    protocolInfo << "http-get:*:" << file->getMimeType() << ":" << file->getDLNAAdditionalInfo();
                                    Utils::setAttribute(resNode, "protocolInfo", protocolInfo.str().c_str());

                                    Utils::setNodeValue(resNode, uri);

                                    LoggerUtil_debug(logger, uri);

                                    ixmlNode_appendChild(itemNode, resNode);
                                }
                            }
                        }

                        void TVMSContentDirectory::addUPnPContainer(NodePtr didl, Directory* dir, bool restricted, bool searchable) {
                            IXML_Node* didlNode = (IXML_Node*) didl;

                            IXML_Element *itemElem = (IXML_Element*) malloc(sizeof (IXML_Element));
                            memset(itemElem, 0, sizeof (IXML_Element));
                            IXML_Node* itemNode = (IXML_Node*) itemElem;
                            itemNode->nodeName = strdup("container");
                            itemNode->nodeType = eELEMENT_NODE;

                            if (searchable == true)
                                Utils::setAttribute(itemNode, "searchable", "true");
                            else
                                Utils::setAttribute(itemNode, "searchable", "false");

                            if (restricted == true)
                                Utils::setAttribute(itemNode, "restricted", "true");
                            else
                                Utils::setAttribute(itemNode, "restricted", "false");

                            Utils::setAttribute(itemNode, "childCount", Utils::integerToString(dir->getFiles()->size() + dir->getDirectories()->size()));
                            Utils::setAttribute(itemNode, "parentID", (dir->getParentID() != NULL ? dir->getParentID() : "-1"));
                            Utils::setAttribute(itemNode, "id", dir->getID());

                            Utils::setNode(itemNode, "upnp:class", "object.container.storageFolder");
                            Utils::setNode(itemNode, "dc:title", dir->getName());

                            ixmlNode_appendChild(didlNode, itemNode);
                        }

                    }
                }
            }
        }
    }
}
