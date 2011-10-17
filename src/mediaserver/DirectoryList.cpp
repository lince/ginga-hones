#include "honesconfig.h"

#include <string>
#include "mediaserver/Directory.h"
#include "mediaserver/DirectoryList.h"
using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediaserver {

                            DirectoryList::DirectoryList(){
                            }

                            DirectoryList::~DirectoryList(){
                                clear();
                            }

                            void DirectoryList::addDirectory(Directory* dir){
                                Vector::add(dir);
                            }

                            Directory* DirectoryList::getDirectory(int index){
                                return (Directory*)get(index);
                            }

                            void DirectoryList::removeDirectory(Directory* dir){
                                Vector::remove(dir);
                            }

                            void DirectoryList::clear(){
                                int size = Vector::size();
                                for (int i=0; i < size; i++) {
                                    Directory* value = getDirectory(i);
                                    if(value != NULL){
                                        delete value;
                                        value = NULL;
                                    }
                                }
                                Vector::clear();
                            }

                            File* DirectoryList::getFileByID(const char* id){
                                int size = Vector::size();
                                for (int i=0; i < size; i++) {
                                    File* file = getDirectory(i)->getFileByID(id);
                                    if(file != NULL)
                                        return file;
                                }
                                return NULL;
                            }
                            
                            Directory* DirectoryList::getDirectoryByID(const char* id) {
                                if (id != NULL) {
                                    string tmp = id;
                                    int size = Vector::size();
                                    for (int i = 0; i < size; i++) {
                                        const char* dir_id = getDirectory(i)->getID();
                                        if (tmp.compare(dir_id) == 0) {
                                            return getDirectory(i);
                                        } else {
                                            Directory* dir = getDirectory(i)->getDirectoryByID(tmp.c_str());
                                            if (dir != NULL)
                                                return dir;
                                        }
                                    }
                                }
                                return NULL;
                            }

                    }
                }
            }
        }
    }
}
