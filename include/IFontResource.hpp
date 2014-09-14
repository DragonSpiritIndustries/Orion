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

#ifndef IFONTRESOURCE_HPP
#define IFONTRESOURCE_HPP

#include "IResource.hpp"
#include "Vector2.hpp"
#include <Color.hpp>
#include <Athena/Types.hpp>
class IFontResource : public IResource
{
public:
    IFontResource(const std::string& path);
    virtual ~IFontResource() {}
    virtual Vector2i measureString(const std::string& str, int size=16)=0;
    virtual int glyphAdvance(atUint16 ch, int size = 16)=0;
    virtual void draw(float x, float y, const std::string& str, int size=16, const Colorb& color=Colorb::white)=0;
};

#endif // IFONTRESOURCE_HPP
