#include "honesconfig.h"

#include <unistd.h>
#include "ui/SelectableTextArea.h"

using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace ui {

                        SelectableTextArea::SelectableTextArea(int x, int y, int width, int height, const char* text, bool roundRect) : TextArea(x, y, width, height, text, roundRect) {
                            selectedTextArea = NULL;
                            selected = false;
                            focusable = false;
                        }

                        void SelectableTextArea::setSelected(bool value) {
                            this->selected = value;

                            validate();
                        }

                        bool SelectableTextArea::isSelected() {
                            return selected;
                        }

                        void SelectableTextArea::setFocusable(bool value) {
                            this->focusable = value;
                        }

                        bool SelectableTextArea::isFocusable() {
                            return focusable;
                        }

                        void SelectableTextArea::setSelectedTextArea(TextArea* textArea){
                            this->selectedTextArea = textArea;
                        }

                        TextArea* SelectableTextArea::getSelectedTextArea(){
                            return selectedTextArea;
                        }

                    }
                }
            }
        }
    }
}
