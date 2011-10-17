#include "honesconfig.h"

#include <unistd.h>
#include "ui/Container.h"
#include "ui/TextArea.h"
#include "util/Utils.h"

using namespace br::ufscar::lince::ginga::hones::util;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace ui {

                        TextArea::TextArea(int x, int y, int width, int height, const char* text, bool roundRect) {
                            this->parent = NULL;
                            this->x = x;
                            this->y = y;
                            this->width = width;
                            this->height = height;
                            this->backgroundRed = 255;
                            this->backgroundGreen = 255;
                            this->backgroundBlue = 255;
                            this->backgroundAlpha = 255;
                            this->foregroundRed = 0;
                            this->foregroundGreen = 0;
                            this->foregroundBlue = 0;
                            this->foregroundAlpha = 255;
                            this->imagePath = "";
                            this->text = text;
                            this->value = "";
                            this->fontSize = 12;
                            this->borderWidth = 0;
                            this->borderRed = 255;
                            this->borderGreen = 255;
                            this->borderBlue = 255;
                            this->borderAlpha = 255;
                            this->roundRect = roundRect;
                        }

                        TextArea::TextArea(const TextArea& textArea) {
                            this->parent = textArea.parent;
                            this->x = textArea.x;
                            this->y = textArea.y;
                            this->width = textArea.width;
                            this->height = textArea.height;
                            this->backgroundRed = textArea.backgroundRed;
                            this->backgroundGreen = textArea.backgroundGreen;
                            this->backgroundBlue = textArea.backgroundBlue;
                            this->backgroundAlpha = textArea.backgroundAlpha;
                            this->foregroundRed = textArea.foregroundRed;
                            this->foregroundGreen = textArea.foregroundGreen;
                            this->foregroundBlue = textArea.foregroundBlue;
                            this->foregroundAlpha = textArea.foregroundAlpha;
                            this->imagePath = textArea.imagePath;
                            this->text = textArea.text;
                            this->value = textArea.value;
                            this->fontSize = textArea.fontSize;
                            this->borderWidth = textArea.borderWidth;
                            this->borderRed = textArea.borderRed;
                            this->borderGreen = textArea.borderGreen;
                            this->borderBlue = textArea.borderBlue;
                            this->borderAlpha = textArea.borderAlpha;
                            this->roundRect = textArea.roundRect;
                        }

                        TextArea::~TextArea() {
                            if (parent != NULL)
                                parent->removeComponent(this);
                        }

                        bool TextArea::setParent(Container* parent) {
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

                        void TextArea::setBounds(int x, int y, int width, int height) {
                            this->x = x;
                            this->y = y;
                            this->width = width;
                            this->height = height;

                            validate();
                        }

                        int TextArea::getX() {
                            return x;
                        }

                        int TextArea::getY() {
                            return y;
                        }

                        int TextArea::getWidth() {
                            return width;
                        }

                        int TextArea::getHeight() {
                            return height;
                        }

                        void TextArea::setText(const char* text) {
                            if (text == NULL)
                                this->text = "";
                            else
                                this->text = text;

                            validate();
                        }

                        const char* TextArea::getText() {
                            return text.c_str();
                        }

                        void TextArea::setValue(const char* text) {
                            if (text == NULL)
                                this->value = "";
                            else
                                this->value = text;
                        }

                        const char* TextArea::getValue() {
                            return value.c_str();
                        }

                        void TextArea::setFontSize(int size){
                            this->fontSize = size;
                        }

                        int TextArea::getFontSize(){
                            return fontSize;
                        }

                        void TextArea::setBackgroundColor(int r, int g, int b, int alpha) {
                            this->backgroundRed = r;
                            this->backgroundGreen = g;
                            this->backgroundBlue = b;
                            this->backgroundAlpha = alpha;

                            validate();
                        }

                        const char* TextArea::getBackgroundColor() {
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

                        void TextArea::setForegroundColor(int r, int g, int b, int alpha) {
                            this->foregroundRed = r;
                            this->foregroundGreen = g;
                            this->foregroundBlue = b;
                            this->foregroundAlpha = alpha;

                            validate();
                        }

                        const char* TextArea::getForegroundColor() {
                            foregroundColor = "r=";
                            foregroundColor.append(Utils::integerToString(foregroundRed));
                            foregroundColor.append(";g=");
                            foregroundColor.append(Utils::integerToString(foregroundGreen));
                            foregroundColor.append(";b=");
                            foregroundColor.append(Utils::integerToString(foregroundBlue));
                            foregroundColor.append(";a=");
                            foregroundColor.append(Utils::integerToString(foregroundAlpha));
                            return foregroundColor.c_str();
                        }

                        void TextArea::setBackgroundImage(const char* path) {
                            if (path == NULL)
                                this->imagePath = "";
                            else
                                this->imagePath = path;

                            validate();
                        }

                        const char* TextArea::getBackgroundImage() {
                            return imagePath.c_str();
                        }

                        void TextArea::setBorder(int r, int g, int b, int alpha, int borderWidth) {
                            this->borderRed = r;
                            this->borderGreen = g;
                            this->borderBlue = b;
                            this->borderAlpha = alpha;
                            this->borderWidth = borderWidth;

                            validate();
                        }

                        const char* TextArea::getBorderColor() {
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

                        int TextArea::getBorderWidth() {
                            return borderWidth;
                        }

                        void TextArea::setTransparency(int alpha) {
                            this->backgroundAlpha = alpha;

                            validate();
                        }

                        int TextArea::getTransparency() {
                            return backgroundAlpha;
                        }

                        bool TextArea::isRoundRect() {
                            return roundRect;
                        }

                        void TextArea::validate() {
                            if (parent != NULL)
                                parent->validate();
                        }

                    }
                }
            }
        }
    }
}