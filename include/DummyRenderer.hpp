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

#ifndef DUMMYRENDERER_HPP
#define DUMMYRENDERER_HPP

#include "IRenderer.hpp"

class DummyRenderer : public IRenderer
{
public:
    DummyRenderer()
    {}
    virtual ~DummyRenderer()
    {}

    void setClearColor(const Colorf& = Colorf::black){}
    bool initialize(IWindow*) {return false;}
    void clear() {}
    void present() {}
    void drawRect(int, int, int, int, bool= false, Colorb =Colorb::white) {}
    void* handle() { return nullptr; }
    void setVSync(bool) {}
};

#endif // DUMMYRENDERER_HPP
