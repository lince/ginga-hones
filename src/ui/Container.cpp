#include "honesconfig.h"

#include <unistd.h>
#include "util/Utils.h"
#include "ui/Container.h"

using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace ui {

                        Container::Container(int x, int y, int width, int height, bool roundRect) {
                            this->parent = NULL;
                            this->x = x;
                            this->y = y;
                            this->width = width;
                            this->height = height;
                            this->backgroundRed = 255;
                            this->backgroundGreen = 255;
                            this->backgroundBlue = 255;
                            this->backgroundAlpha = 255;
                            this->imagePath = "";
                            this->borderWidth = 0;
                            this->borderRed = 255;
                            this->borderGreen = 255;
                            this->borderBlue = 255;
                            this->borderAlpha = 255;
                            this->roundRect = roundRect;
                        }

                        Container::~Container() {
                            if (parent != NULL)
                                parent->removeComponent(this);
                        }

                        bool Container::setParent(Container* parent) {
                            if (this->parent != parent) {
                                if (this->parent != NULL)
                                    this->parent->removeComponent(this);

                                this->parent = parent;

                                if (parent != NULL)
                                    parent->addComponent(this);

                                return true;
                            } else {
                                return false;
                            }
                        }

                        void Container::setBounds(int x, int y, int width, int height) {
                            this->x = x;
                            this->y = y;
                            this->width = width;
                            this->height = height;

                            validate();
                        }

                        int Container::getX() {
                            return x;
                        }

                        int Container::getY() {
                            return y;
                        }

                        int Container::getWidth() {
                            return width;
                        }

                        int Container::getHeight() {
                            return height;
                        }

                        void Container::setBackgroundColor(int r, int g, int b, int alpha) {
                            this->backgroundRed = r;
                            this->backgroundGreen = g;
                            this->backgroundBlue = b;
                            this->backgroundAlpha = alpha;

                            validate();
                        }

                        const char* Container::getBackgroundColor() {
                            backgroundColor = "r=";
                            backgroundColor.append(Utils::integerToString(backgroundRed));
                            backgroundColor.append(";g=");
                            backgroundColor.append(Utils::integerToString(backgroundGreen));
                            backgroundColor.append(";b=");
                            backgroundColor.append(Utils::integerToString(backgroundBlue));
                            backgroundColor.append(";a=");
                            backgroundColor.append(Utils::integerToString(backgroundAlpha));
                            return backgroundColor.c_str();
                        }

                        void Container::setBackgroundImage(const char* path) {
                            if (path == NULL)
                                this->imagePath = "";
                            else
                                this->imagePath = path;

                            validate();
                        }

                        const char* Container::getBackgroundImage() {
                            return imagePath.c_str();
                        }

                        void Container::setBorder(int r, int g, int b, int alpha, int borderWidth) {
                            this->borderRed = r;
                            this->borderGreen = g;
                            this->borderBlue = b;
                            this->borderAlpha = alpha;
                            this->borderWidth = borderWidth;

                            validate();
                        }

                        const char* Container::getBorderColor() {
                            borderColor = "r=";
                            borderColor.append(Utils::integerToString(borderRed));
                            borderColor.append(";g=");
                            borderColor.append(Utils::integerToString(borderGreen));
                            borderColor.append(";b=");
                            borderColor.append(Utils::integerToString(borderBlue));
                            borderColor.append(";a=");
                            borderColor.append(Utils::integerToString(borderAlpha));
                            return borderColor.c_str();
                        }

                        int Container::getBorderWidth() {
                            return borderWidth;
                        }

                        void Container::setTransparency(int alpha) {
                            this->backgroundAlpha = alpha;

                            validate();
                        }

                        int Container::getTransparency() {
                            return backgroundAlpha;
                        }

                        bool Container::isRoundRect() {
                            return roundRect;
                        }

                        void Container::validate() {
                            if (parent != NULL)
                                parent->validate();
                        }

                        bool Container::addComponent(TextArea* component){
                            if(component->setParent(this) == true) {
                                bool result = textAreaList.add(component);
                                validate();
                                return result;
                            } else {
                                return false;
                            }
                        }

                        bool Container::removeComponent(TextArea* component){
                            bool result = textAreaList.remove(component);
                            validate();
                            return result;
                        }

                        bool Container::addComponent(Container* component){
                            if(component->setParent(this) == true) {
                                bool result = containerList.add(component);
                                validate();
                                return result;
                            } else {
                                return false;
                            }
                        }

                        bool Container::removeComponent(Container* component){
                            bool result = containerList.remove(component);
                            validate();
                            return result;
                        }

                        Vector* Container::getTextAreaList(){
                            return &textAreaList;
                        }

                        Vector* Container::getContainerList(){
                            return &containerList;
                        }

                        void Container::clear() {
                            textAreaList.clear();
                            containerList.clear();
                            validate();
                        }

                    }
                }
            }
        }
    }
}
