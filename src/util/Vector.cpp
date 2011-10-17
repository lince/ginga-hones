#include "honesconfig.h"

#include <cstddef>
#include "util/Vector.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace util {

                        struct element {
                            void* data;
                            element* prev;
                            element* next;
                        };

                        Vector::Vector(){
                            vectorPtr = NULL;
                        }

                        bool Vector::add(void *object){
                            if (indexOf(object) >= 0){
                                return false;
                            } else {
                                element* elem = new element;
                                elem->data = object;
                                elem->prev = NULL;
                                elem->next = NULL;

                                if(vectorPtr == NULL){
                                    vectorPtr = (void*) elem;
                                } else {
                                    element* last = (element*) vectorPtr;
                                    while(last->next != NULL){
                                        last = last->next;
                                    }
                                    last->next = elem;
                                    elem->prev = last;
                                }
                                return true;
                            }
                        }

                        bool Vector::remove(void *object){
                            int index = indexOf(object);
                            if (index < 0){
                                return false;
                            } else {
                                element* elem = (element*) vectorPtr;
                                while( (elem->data != object) && (elem->next != NULL) ){
                                    elem = elem->next;
                                }

                                if(elem->next != NULL){
                                    elem->next->prev = elem->prev;
                                }
                                if(elem->prev != NULL){
                                    elem->prev->next = elem->next;
                                } else {
                                    vectorPtr = (void*) elem->next;
                                }

                                delete elem;
                                elem = NULL;
                                return true;
                            }
                        }

                        int Vector::indexOf(void *object){
                            if (vectorPtr != NULL) {
                                int i = 0;
                                element* elem = (element*) vectorPtr;
                                while ((elem->data != object) && (elem->next != NULL)) {
                                    elem = elem->next;
                                    i++;
                                }
                                if (elem->data == object)
                                    return i;
                            }
                            return -1;
                        }

                        void* Vector::get(int index){
                            if((index >= 0) && (index < size())){
                                int i = 0;
                                element* elem = (element*) vectorPtr;
                                while ( (i < index) && (elem->next != NULL) ) {
                                    elem = elem->next;
                                    i++;
                                }
                                if(i == index)
                                    return elem->data;
                            }
                            return NULL;
                        }

                        bool Vector::insertAt(void *object, int index){
                            if ( (indexOf(object) == -1) && (index >= 0) && (index < size()) ){
                                int i = 0;
                                element* elem = (element*) vectorPtr;
                                while ( (i < index) && (elem->next != NULL) ) {
                                    elem = elem->next;
                                    i++;
                                }
                                if(i == index) {
                                    elem->data = object;
                                    return true;
                                }
                            }
                            return false;
                        }

                        int Vector::size(){
                            int i = 0;
                            if (vectorPtr != NULL) {
                                i = 1;
                                element* elem = (element*) vectorPtr;
                                while (elem->next != NULL) {
                                    elem = elem->next;
                                    i++;
                                }
                            }
                            return i;
                        }

                        void Vector::clear(void* elemPtr){
                            if( elemPtr != NULL ){
                                element* elem = (element*) elemPtr;
                                element* next = elem->next;
                                element* prev = elem->prev;

                                if(next != NULL){
                                    next->prev = prev;
                                }
                                if(prev != NULL){
                                    prev->next = next;
                                } else {
                                    vectorPtr = (void*) next;
                                }
                                
                                delete elem;
                                elem = NULL;

                                clear((void*)next);
                                clear((void*)prev);
                            }
                        }

                        void Vector::clear(){
                            clear(vectorPtr);
                        }

                    }
                }
            }
        }
    }
}
