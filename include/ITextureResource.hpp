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

#ifndef ITEXTURERESOURCE_HPP
#define ITEXTURERESOURCE_HPP

#include "IResource.hpp"
#include "ResourceManager.hpp"
#include "Rectangle.hpp"
#include "Vector2.hpp"

class ApplicationBase;
class ITextureResource : public IResource
{
public:
    ITextureResource(const std::string path)
        : IResource(path)
    {
    }

    virtual void draw(float x, float y)=0;
    virtual void draw(const Vector2f& position)=0;
    virtual void draw(float x, float y, const Rectanglef& subrect, const Vector2f& origin, bool flipH, bool flipV, float angle)=0;
    virtual void draw(const Rectanglef& rect, const Rectanglef& subrect, const Vector2f& origin, bool flipH, bool flipV, float angle)=0;
    virtual Vector2i size() const = 0;
    virtual int width()     const;
    virtual int height()    const;
    virtual void setRepeat(bool val);
protected:
    bool m_repeat;
    Vector2i     m_size;
};

#endif // ITEXTURERESOURCE_HPP
