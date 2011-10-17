/******************************************************************************
Este arquivo eh parte da implementacao do ambiente declarativo do middleware
Ginga (Ginga-NCL).

Copyright (C) 2009 UFSCar/Lince, Todos os Direitos Reservados.

Este programa eh software livre; voce pode redistribui-lo e/ou modificah-lo sob
os termos da Licenca Publica Geral GNU versao 2 conforme publicada pela Free
Software Foundation.

Este programa eh distribuido na expectativa de que seja util, porem, SEM
NENHUMA GARANTIA; nem mesmo a garantia implicita de COMERCIABILIDADE OU
ADEQUACAO A UMA FINALIDADE ESPECIFICA. Consulte a Licenca Publica Geral do
GNU versao 2 para mais detalhes.

Voce deve ter recebido uma copia da Licenca Publica Geral do GNU versao 2 junto
com este programa; se nao, escreva para a Free Software Foundation, Inc., no
endereco 59 Temple Street, Suite 330, Boston, MA 02111-1307 USA.

Para maiores informacoes:
lince@dc.ufscar.br
http://www.ncl.org.br
http://www.ginga.org.br
http://lince.dc.ufscar.br
******************************************************************************
This file is part of the declarative environment of middleware Ginga (Ginga-NCL)

Copyright (C) 2009 UFSCar/Lince, Todos os Direitos Reservados.

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License version 2 as published by
the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License version 2 for more
details.

You should have received a copy of the GNU General Public License version 2
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA

For further information contact:
lince@dc.ufscar.br
http://www.ncl.org.br
http://www.ginga.org.br
http://lince.dc.ufscar.br
*******************************************************************************/

#ifndef _TEXTAREA_H
#define	_TEXTAREA_H

#include <string>
using namespace std;

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace hones {
                    namespace ui {

                        class Container;

                        class TextArea {
                        private:
                            Container* parent;
                            int x;
                            int y;
                            int width;
                            int height;
                            int backgroundRed;
                            int backgroundGreen;
                            int backgroundBlue;
                            int backgroundAlpha;
                            int foregroundRed;
                            int foregroundGreen;
                            int foregroundBlue;
                            int foregroundAlpha;
                            string imagePath;
                            string text;
                            string value;
                            int fontSize;
                            int borderWidth;
                            int borderRed;
                            int borderGreen;
                            int borderBlue;
                            int borderAlpha;
                            bool roundRect;
                            string backgroundColor;
                            string foregroundColor;
                            string borderColor;

                        public:
                            TextArea(int x, int y, int width, int height, const char* text = "", bool roundRect = true);
                            TextArea(const TextArea& textArea);
                            virtual ~TextArea();

                            virtual bool setParent(Container* parent);
                            virtual void setBounds(int x, int y, int width, int height);
                            int getX();
                            int getY();
                            int getWidth();
                            int getHeight();
                            virtual void setText(const char* text);
                            const char* getText();
                            virtual void setValue(const char* text);
                            const char* getValue();
                            virtual void setFontSize(int size);
                            int getFontSize();
                            virtual void setBackgroundColor(int r, int g, int b, int alpha = 255);
                            const char* getBackgroundColor();
                            virtual void setForegroundColor(int r, int g, int b, int alpha = 255);
                            const char* getForegroundColor();
                            virtual void setBackgroundImage(const char* path);
                            const char* getBackgroundImage();
                            virtual void setBorder(int r, int g, int b, int alpha = 255, int borderWidth = 1);
                            const char* getBorderColor();
                            int getBorderWidth();
                            virtual void setTransparency(int alpha);
                            int getTransparency();
                            bool isRoundRect();
                            virtual void validate();

                        };

                    }
                }
            }
        }
    }
}

#endif	/* _TEXTAREA_H */
