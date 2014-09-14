// This file is part of Orion.
//
// Orion is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Orion is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Orion.  If not, see <http://www.gnu.org/licenses/>

#ifndef IRENDERER_HPP
#define IRENDERER_HPP

#include "Color.hpp"
class IWindow;
class IRenderer
{
public:
    virtual bool initialize(IWindow*)=0;
    virtual void setClearColor(const Colorf& color = Colorf::black)=0;
    virtual void clear()=0;
    virtual void present()=0;
    virtual void drawRect(int, int, int, int, bool=false, Colorb=Colorb::white)=0;
    virtual void* handle()=0;
    virtual void setVSync(bool)=0;
protected:
    Colorf         m_clearColor;
};

#endif // IRENDERER_HPP
