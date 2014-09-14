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

#ifndef DUMMYAPPLICATION_HPP
#define DUMMYAPPLICATION_HPP

#include "ApplicationBase.hpp"

class DummyApplication : public ApplicationBase
{
public:

    void setTitle(const std::string&) {}
    std::string title() const {return std::string();}

    void* rendererHandle() { return nullptr; }

    void drawDebugText(const std::string&, float, float, Colorb =Colorb::white) {}
    void drawDebugText(const std::string&, const Vector2f&, Colorb =Colorb::white) {}
    void setClearColor(const Colorf& = Colorf::black) {}
    void drawRectangle(int, int , int , int , bool = false, Colorb =Colorb::white) {}
    void close() {}
protected:
    void pollEvents() {}
    void applyViewport() {}
};

#endif // DUMMYAPPLICATION_HPP
