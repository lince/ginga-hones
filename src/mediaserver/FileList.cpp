#include "honesconfig.h"

#include <string>
#include "mediaserver/FileList.h"
using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace mediaserver {

                            FileList::FileList(){
                            }

                            FileList::~FileList(){
                                clear();
                            }

                            void FileList::addFile(File* file){
                                this->add(file);
                            }

                            File* FileList::getFile(int index){
                                return (File*)get(index);
                            }

                            void FileList::removeFile(File* file){
                                this->remove(file);
                            }

                            void FileList::clear(){
                                int size = this->size();
                                for (int i=0; i < size; i++) {
                                    File* value = getFile(i);
                                    if(value != NULL){
                                        delete value;
                                        value = NULL;
                                    }
                                }
                                Vector::clear();
                            }

                            File* FileList::getFileByID(const char* id){
                                if(id != NULL){
                                    string tmp = id;
                                    int size = this->size();
                                    for(int i=0; i < size; i++) {
                                        const char* fileId = getFile(i)->getID();
                                        if(tmp.compare(fileId) == 0){
                                            return getFile(i);
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
