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

#ifndef DUMMYFONTRESOURCE_HPP
#define DUMMYFONTRESOURCE_HPP

#include <IFontResource.hpp>

class DummyFontResource : public IFontResource
{
public:
    DummyFontResource(const std::string &path)
        : IFontResource(path)
    {
    }

    virtual ~DummyFontResource() {}
    Vector2i measureString(const std::string&, int =16) {return Vector2i();}
    int glyphAdvance(atUint16, int) { return 0;}
    void draw(float, float, const std::string&, int =16, const Colorb& =Colorb::white) {}

    static IResource* loadFont(const std::string&) { return nullptr; }
};

#endif // DUMMYFONTRESOURCE_HPP
