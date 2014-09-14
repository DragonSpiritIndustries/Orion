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

#ifndef DUMMYTEXTURERESOURCE_HPP
#define DUMMYTEXTURERESOURCE_HPP

#include "ITextureResource.hpp"

class DummyTextureResource : public ITextureResource
{
public:
    DummyTextureResource(const std::string& path)
        : ITextureResource(path)
    {}

    virtual ~DummyTextureResource() {}

    void draw(float, float) {}
    void draw(const Vector2f& position)=0;
    void draw(float, float, const Rectanglef&, const Vector2f&, bool, bool, float) {}
    void draw(const Rectanglef&, const Rectanglef&, const Vector2f&, bool, bool, float){}
    Vector2i size() const { return Vector2i(); }

    static IResource* loadTexture(const std::string&) {return nullptr;}
};

#endif // DUMMYTEXTURERESOURCE_HPP
