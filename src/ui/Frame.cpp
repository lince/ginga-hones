#include "honesconfig.h"

#include <typeinfo>
#include <unistd.h>
#include <stdio.h>
#include <directfb/directfb.h>
#include "ui/GraphicsHandler.h"
#include "ui/SelectableTextArea.h"
#include "ui/Frame.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace ui {

                        Frame::Frame(int x, int y, int width, int height, bool roundRect, bool modal) : Container(x, y, width, height, roundRect) {
                            this->win = NULL;
                            this->backgroundRed = 0;
                            this->backgroundGreen = 0;
                            this->backgroundBlue = 0;
                            this->backgroundAlpha = 255;
                            imagePath = "";
                            this->visible = false;
                            this->borderRed = 0;
                            this->borderGreen = 0;
                            this->borderBlue = 0;
                            this->borderAlpha = 255;
                            this->borderWidth = 0;

                            DFBWindowDescription desc;
                            desc.flags = (DFBWindowDescriptionFlags) (DWDESC_POSX | DWDESC_POSY | DWDESC_WIDTH | DWDESC_HEIGHT | DWDESC_CAPS);
                            desc.width = width;
                            desc.height = height;
                            desc.posx = x;
                            desc.posy = y;
                            desc.caps = DWCAPS_ALPHACHANNEL;

                            win = GraphicsHandler::createFrame(&desc);

                            IDirectFBWindow* winPtr = (IDirectFBWindow*) win;

                            winPtr->SetOpacity(winPtr, 0x00);

                            winPtr->RaiseToTop(winPtr);

                            DFBWindowOptions opt;
                            winPtr->GetOptions(winPtr, &opt);
                            // Adiciona as opções ghost e alphachannel.
                            winPtr->SetOptions(winPtr, (DFBWindowOptions) (opt | DWOP_ALPHACHANNEL | DWOP_GHOST));
                            // Coloca a janela sobre todas as outras (modal).
                            if (modal == true)
                                winPtr->SetStackingClass(winPtr, DWSC_UPPER);

                            setBackgroundColor(backgroundRed, backgroundGreen, backgroundBlue, backgroundAlpha);
                        }

                        Frame::~Frame() {
                            if (win != NULL) {
                                IDirectFBWindow* winPtr = (IDirectFBWindow*) win;
                                winPtr->Destroy(winPtr);
                                winPtr->Release(winPtr);
                                win = NULL;
                            }

                            clear();
                        }

                        void Frame::setTransparency(int alpha) {
                            this->backgroundAlpha = alpha;

                            if (win != NULL) {
                                IDirectFBWindow* winPtr = (IDirectFBWindow*) win;
                                winPtr->SetOpacity(winPtr, alpha);
                            }

                            if (alpha == 0) {
                                this->visible = false;
                            } else {
                                this->visible = true;
                            }
                        }

                        void Frame::moveTo(int x, int y) {
                            this->x = x;
                            this->y = y;

                            if (win != NULL) {
                                IDirectFBWindow* winPtr = (IDirectFBWindow*) win;
                                winPtr->MoveTo(winPtr, x, y);
                            }
                        }

                        void Frame::moveToTop() {
                            if (win != NULL) {
                                IDirectFBWindow* winPtr = (IDirectFBWindow*) win;
                                winPtr->RaiseToTop(winPtr);
                            }
                        }

                        void Frame::moveToBottom() {
                            if (win != NULL) {
                                IDirectFBWindow* winPtr = (IDirectFBWindow*) win;
                                winPtr->LowerToBottom(winPtr);
                            }
                        }

                        void Frame::setVisible(bool value) {
                            if (value == true) {
                                setTransparency(backgroundAlpha);
                            } else {
                                if (win != NULL) {
                                    IDirectFBWindow* winPtr = (IDirectFBWindow*) win;
                                    winPtr->SetOpacity(winPtr, 0x00);
                                }

                                this->visible = false;
                            }
                        }

                        bool Frame::isVisible() {
                            return visible;
                        }

                        void drawRoundRect(IDirectFBSurface* surface, int x, int y, int width, int height) {
                            int arc = height > width ? width * 0.05 : height * 0.05;
                            int x1 = x + arc;
                            int y1 = y + arc;
                            int x2 = x + width - arc - 1;
                            int y2 = y + height - arc - 1;
                            int dec = 3 - 2 * arc;

                            // top
                            surface->DrawLine(surface, x + arc, y, x + width - arc - 1, y);
                            // bottom
                            surface->DrawLine(surface, x + arc, y + height - 1, x + width - arc - 1, y + height - 1);
                            // left
                            surface->DrawLine(surface, x, y + arc, x, y + height - arc - 1);
                            // right
                            surface->DrawLine(surface, x + width - 1, y + arc, x + width - 1, y + height - arc - 1);

                            for (int xx = 0, yy = arc; xx <= yy; xx++) {
                                surface->DrawLine(surface, x2 + xx, y2 + yy, x2 + xx, y2 + yy);
                                surface->DrawLine(surface, x2 + xx, y1 - yy, x2 + xx, y1 - yy);
                                surface->DrawLine(surface, x1 - xx, y2 + yy, x1 - xx, y2 + yy);
                                surface->DrawLine(surface, x1 - xx, y1 - yy, x1 - xx, y1 - yy);

                                surface->DrawLine(surface, x2 + yy, y2 + xx, x2 + yy, y2 + xx);
                                surface->DrawLine(surface, x2 + yy, y1 - xx, x2 + yy, y1 - xx);
                                surface->DrawLine(surface, x1 - yy, y2 + xx, x1 - yy, y2 + xx);
                                surface->DrawLine(surface, x1 - yy, y1 - xx, x1 - yy, y1 - xx);
                                
                                if (dec >= 0) {
                                    dec += -4 * yy + 4;
                                    yy--;
                                }
                                dec += 4 * xx + 6;
                            }
                        }
                        
                        void fillRoundRect(IDirectFBSurface* surface, int x, int y, int width, int height) {
                            int arc = height > width ? width * 0.05 : height * 0.05;
                            int x1 = x + arc;
                            int y1 = y + arc;
                            int x2 = x + width - arc - 1;
                            int y2 = y + height - arc - 1;
                            int dec = 3 - 2 * arc;

                            surface->FillRectangle(surface, x, y1, width, height - (2 * arc));
                            
                            for (int xx = 0, yy = arc; xx <= yy; xx++) {
                                surface->DrawLine(surface, x1 - xx, y1 - yy, x2 + xx, y1 - yy);
                                surface->DrawLine(surface, x1 - xx, y2 + yy, x2 + xx, y2 + yy);

                                surface->DrawLine(surface, x1 - yy, y1 - xx, x2 + yy, y1 - xx);
                                surface->DrawLine(surface, x1 - yy, y2 + xx, x2 + yy, y2 + xx);

                                if (dec >= 0) {
                                    dec += -4 * yy + 4;
                                    yy--;
                                }
                                dec += 4 * xx + 6;
                            }
                        }

                        void paintComponent(IDirectFBSurface* tmp,
                                int x,
                                int y,
                                int width,
                                int height,
                                const char* backgroundColor,
                                const char* foregroundColor,
                                const char* borderColor,
                                int borderWidth,
                                string imagePath,
                                string text,
                                int fontSize,
                                bool isRoundRect) {

                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.ui.frame");
                            LoggerUtil_debug(logger, "paintComponent(...)");

                            int backgroundRed = 255;
                            int backgroundGreen = 255;
                            int backgroundBlue = 255;
                            int backgroundAlpha = 255;
                            int foregroundRed = 0;
                            int foregroundGreen = 0;
                            int foregroundBlue = 0;
                            int foregroundAlpha = 255;
                            int borderRed = 255;
                            int borderGreen = 255;
                            int borderBlue = 255;
                            int borderAlpha = 255;

                            sscanf(backgroundColor, "r=%d;g=%d;b=%d;a=%d", &backgroundRed, &backgroundGreen, &backgroundBlue, &backgroundAlpha);
                            sscanf(foregroundColor, "r=%d;g=%d;b=%d;a=%d", &foregroundRed, &foregroundGreen, &foregroundBlue, &foregroundAlpha);
                            sscanf(borderColor, "r=%d;g=%d;b=%d;a=%d", &borderRed, &borderGreen, &borderBlue, &borderAlpha);

                            if (borderWidth < 0)
                                borderWidth = borderWidth * (-1);

                            DFBRectangle surfaceRect;
                            surfaceRect.x = x;
                            surfaceRect.y = y;
                            surfaceRect.w = width;
                            surfaceRect.h = height;

                            IDirectFBSurface* surface;
                            tmp->GetSubSurface(tmp, &surfaceRect, &surface);
                            // background color
                            surface->SetBlittingFlags(surface, (DFBSurfaceBlittingFlags)DSBLIT_BLEND_ALPHACHANNEL);
                            surface->SetColor(surface, backgroundRed, backgroundGreen, backgroundBlue, backgroundAlpha);
                            if (isRoundRect == true) {
                                fillRoundRect(surface, borderWidth, borderWidth, width - (2 * borderWidth), height - (2 * borderWidth));
                            } else {
                                surface->FillRectangle(surface, borderWidth, borderWidth, width - (2 * borderWidth), height - (2 * borderWidth));
                            }
                            // background image
                            if (imagePath.size() != 0) {
                                IDirectFBImageProvider* imgProvider = (IDirectFBImageProvider*) GraphicsHandler::createImageProvider((char*) (imagePath.c_str()));
                                if (imgProvider != NULL) {
                                    imgProvider->RenderTo(imgProvider, surface, NULL);
                                    surface->Blit(surface, surface, NULL, 0, 0);
                                    imgProvider->Release(imgProvider);
                                }
                            }
                            // border
                            surface->SetColor(surface, borderRed, borderGreen, borderBlue, borderAlpha);
                            for (int i = 0; i < borderWidth; i++) {
                                if (isRoundRect == true) {
                                    drawRoundRect(surface, i, i, width - (2 * i), height - (2 * i));
                                } else {
                                    surface->DrawRectangle(surface, i, i, width - (2 * i), height - (2 * i));
                                }
                            }
                            // text
                            if (text.size() != 0) {
                                LoggerUtil_debug(logger, "text: " << text.c_str());

                                IDirectFBFont* font;
                                DFBFontDescription fdsc;
                                fdsc.flags = (DFBFontDescriptionFlags) (DFDESC_HEIGHT | DFDESC_ATTRIBUTES);
                                fdsc.attributes = DFFA_NONE;
                                fdsc.height = fontSize;
                                IDirectFB* dfb = (IDirectFB*) GraphicsHandler::getGraphicsRoot();
                                if (dfb->CreateFont(dfb, NULL, &fdsc, &font) == DFB_OK) {
                                    LoggerUtil_debug(logger, "font created!");
                                    // foreground
                                    surface->SetColor(surface, foregroundRed, foregroundGreen, foregroundBlue, foregroundAlpha);
                                    surface->SetFont(surface, font);
                                    
                                    int textWidth = 0;
                                    int textHeight = 0;
                                    font->GetStringWidth(font, text.c_str(), -1, &textWidth);
                                    font->GetHeight(font, &textHeight);
                                    int charWidth = (int)(textWidth / text.length()) + 1;

                                    if (textWidth < (width - 2 * borderWidth)) {
                                        surface->DrawString(surface, text.c_str(), -1, borderWidth + 1, borderWidth + 1, DSTF_TOPLEFT);
                                    } else {
                                        int line = borderWidth + 1;

                                        try {
                                            while ((text.length() > 0) && ((line + fontSize) <= (height - borderWidth))) {
                                                string aux = text;
                                                int pos = 0;
                                                while ( (aux.length() * charWidth) >= (width - 2 * borderWidth) ) {
                                                    pos = aux.find_last_of(' ');
                                                    if (pos == string::npos) {
                                                        aux = aux.substr(0, aux.length() - 1);
                                                    } else {
                                                        aux = aux.substr(0, pos);
                                                    }
                                                }
                                                if (pos == string::npos)
                                                    text = text.substr(aux.length());
                                                else
                                                    text = text.substr(aux.length() + 1);

                                                LoggerUtil_debug(logger, "line: " << line << " text: " << aux.c_str() << " textWidth: " << (aux.length() * charWidth) << " width: " << width);

                                                if ((text.length() > 0) && ((line + (int)(0.8 * textHeight) + fontSize) > (height - borderWidth)))
                                                    surface->DrawString(surface, aux.append("...").c_str(), -1, borderWidth + 1, line, DSTF_TOPLEFT);
                                                else
                                                    surface->DrawString(surface, aux.c_str(), -1, borderWidth + 1, line, DSTF_TOPLEFT);

                                                line += (int)(0.8 * textHeight);
                                            }
                                        } catch(...){}
                                    }
                                }
                            }
                        }
                        
                        void paintTextAreaList(IDirectFBSurface* surface, Vector* taList){
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.ui.frame");
                            LoggerUtil_debug(logger, "paintTextAreaList(surface, taList)");

                            int listSize = taList->size();
                            for (int i = 0; i < listSize; i++) {
                                TextArea* component = (TextArea*) taList->get(i);

                                try {
                                    if (typeid(*component) == typeid(SelectableTextArea)) {
                                        SelectableTextArea* aux = (SelectableTextArea*) component;
                                        if ((aux->isFocusable() == true) && (aux->isSelected() == true) && (aux->getSelectedTextArea() != NULL))
                                            component = aux->getSelectedTextArea();
                                    }
                                } catch(...) {}

                                paintComponent(surface, component->getX(),
                                        component->getY(),
                                        component->getWidth(),
                                        component->getHeight(),
                                        component->getBackgroundColor(),
                                        component->getForegroundColor(),
                                        component->getBorderColor(),
                                        component->getBorderWidth(),
                                        component->getBackgroundImage(),
                                        component->getText(),
                                        component->getFontSize(),
                                        component->isRoundRect());
                            }
                        }

                        void paintContainerList(IDirectFBSurface* surface, Vector* cList){
                            HLoggerPtr logger = LoggerUtil::getLogger("br.ufscar.lince.ginga.hones.ui.frame");
                            LoggerUtil_debug(logger, "paintTextAreaList(surface, taList)");

                            int listSize = cList->size();
                            for (int i = 0; i < listSize; i++) {
                                Container* component = (Container*) cList->get(i);

                                paintComponent(surface, component->getX(),
                                        component->getY(),
                                        component->getWidth(),
                                        component->getHeight(),
                                        component->getBackgroundColor(),
                                        "",
                                        component->getBorderColor(),
                                        component->getBorderWidth(),
                                        component->getBackgroundImage(),
                                        "",
                                        0,
                                        component->isRoundRect());

                                DFBRectangle surfaceRect;
                                surfaceRect.x = component->getX();
                                surfaceRect.y = component->getY();
                                surfaceRect.w = component->getWidth();
                                surfaceRect.h = component->getHeight();

                                IDirectFBSurface* tmp;
                                surface->GetSubSurface(surface, &surfaceRect, &tmp);

                                paintTextAreaList(tmp, component->getTextAreaList());
                                paintContainerList(tmp, component->getContainerList());
                            }
                        }

                        void Frame::validate() {
                            if (win != NULL) {
                                IDirectFBWindow* winPtr = (IDirectFBWindow*) win;
                                if (borderWidth < 0)
                                    borderWidth = borderWidth * (-1);
                                // transparency
                                if (isVisible() == true)
                                    winPtr->SetOpacity(winPtr, backgroundAlpha);
                                // bounds
                                winPtr->SetBounds(winPtr, x, y, width, height);
                                // background color
                                IDirectFBSurface* tmp;
                                winPtr->GetSurface(winPtr, &tmp);
                                tmp->SetBlittingFlags(tmp, (DFBSurfaceBlittingFlags)DSBLIT_BLEND_ALPHACHANNEL);
                                tmp->Clear(tmp, 0, 0, 0, 0x00);
                                tmp->SetColor(tmp, backgroundRed, backgroundGreen, backgroundBlue, 0xFF);
                                if (roundRect == true) {
                                    fillRoundRect(tmp, borderWidth, borderWidth, width - (2 * borderWidth), height - (2 * borderWidth));
                                } else {
                                    tmp->FillRectangle(tmp, borderWidth, borderWidth, width - (2 * borderWidth), height - (2 * borderWidth));
                                }
                                // background image
                                if (imagePath.size() != 0) {
                                    IDirectFBImageProvider* imgProvider = (IDirectFBImageProvider*) GraphicsHandler::createImageProvider((char*) (imagePath.c_str()));
                                    if (imgProvider != NULL) {
                                        imgProvider->RenderTo(imgProvider, tmp, NULL);
                                        tmp->Blit(tmp, tmp, NULL, 0, 0);
                                        imgProvider->Release(imgProvider);
                                    }
                                }
                                // border
                                tmp->SetColor(tmp, borderRed, borderGreen, borderBlue, borderAlpha);
                                for (int i = 0; i < borderWidth; i++) {
                                    if (roundRect == true) {
                                        drawRoundRect(tmp, i, i, width - (2 * i), height - (2 * i));
                                    } else {
                                        tmp->DrawRectangle(tmp, i, i, width - (2 * i), height - (2 * i));
                                    }
                                }
                                
                                paintTextAreaList(tmp, getTextAreaList());
                                paintContainerList(tmp, getContainerList());

                                tmp->Flip(tmp, NULL, DSFLIP_WAITFORSYNC);
                            }
                        }

                    }
                }
            }
        }
    }
}
