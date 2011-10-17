#include "honesconfig.h"

#include <dirent.h>
#include "mediaserver/TVMSContentDirectory.h"
#include "mediaserver/File.h"
#include "mediaserver/Directory.h"
#include "util/Utils.h"
using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediaserver {

                            Directory::Directory(TVMSContentDirectory* contentDirectory, const char* path){
                                if(path != NULL){
                                    dirent** list;
                                    string dirpath = path;
                                    if(dirpath[dirpath.length() - 1] == '/')
                                        dirpath = dirpath.substr(0, dirpath.length() - 1);

                                    setPath(dirpath.c_str());
                                    setName(dirpath.substr(dirpath.find_last_of('/') + 1).c_str());
                                    setID(Utils::integerToString(contentDirectory->getNextContentID()));
                                    setParentID(NULL);

                                    int length = scandir(dirpath.c_str(), &list, 0, alphasort);
                                    for(int i=0; i < length; i++){
                                        if(list[i]->d_name[0] != '.'){
                                            string filepath = dirpath;
                                            filepath.append("/");
                                            filepath.append(list[i]->d_name);
                                            File* file = new File(filepath.c_str());
                                            if(file->isDirectory()){
                                                Directory* dir = new Directory(contentDirectory, filepath.c_str());
                                                if(!dir->isEmpty()){
                                                    dir->setParentID(getID());
                                                    directoryList.addDirectory(dir);
                                                } else {
                                                    delete dir;
                                                }
                                                delete file;
                                            } else if((file->isReadable()) && (file->getMimeType() != NULL)){
                                                file->setParentID(getID());
                                                file->setID(Utils::integerToString(contentDirectory->getNextContentID()));
                                                fileList.addFile(file);
                                            } else {
                                                delete file;
                                            }
                                        }
                                        delete list[i];
                                    }
                                    delete list;
                                }
                            }

                            Directory::~Directory(){
                                if(parentId != NULL)
                                    delete parentId;
                            }

                            const char* Directory::getName(){
                                return name.c_str();
                            }

                            void Directory::setName(const char* name){
                                this->name = name;
                            }

                            void Directory::setPath(const char* path){
                                this->path = path;
                            }

                            const char* Directory::getPath(){
                                return path.c_str();
                            }

                            void Directory::setID(const char* id){
                                this->id = id;
                            }

                            const char* Directory::getID(){
                                return id.c_str();
                            }

                            void Directory::setParentID(const char* id){
                                if(id != NULL)
                                    this->parentId = strdup(id);
                                else
                                    this->parentId = NULL;
                            }

                            const char* Directory::getParentID(){
                                return parentId;
                            }

                            FileList* Directory::getFiles(){
                                return &fileList;
                            }

                            DirectoryList* Directory::getDirectories(){
                                return &directoryList;
                            }

                            bool Directory::isEmpty(){
                                return !((getFiles()->size() > 0) || (getDirectories()->size() > 0));
                            }

                            File* Directory::getFileByID(const char* id){
                                File* file = fileList.getFileByID(id);
                                if(file != NULL)
                                    return file;
                                else
                                    return directoryList.getFileByID(id);
                            }

                            Directory* Directory::getDirectoryByID(const char* id){
                                return directoryList.getDirectoryByID(id);
                            }

                    }
                }
            }
        }
    }
}