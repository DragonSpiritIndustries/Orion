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

#ifndef VIEWPORT_HPP
#define VIEWPORT_HPP

#include <Vector2.hpp>
class Viewport
{
public:
    Viewport();
    Viewport(const float x, const float y, const int width, const int height);
    Viewport(const Vector2f& location, const Vector2i& size);

    void setLocation(float x, float y);
    void setLocation(const Vector2f& location);
    Vector2f location() const;
    float x() const;
    float y() const;

    void setSize(const int w, const int h);
    void setSize(const Vector2i& size);

    void setWidth(const int w);
    void setHeight(const int h);

    int  width() const;
    int  height() const;

    Vector2i size() const;
    Vector2i center();
private:
    Vector2f m_location;
    Vector2i m_size;
};

#endif // VIEWPORT_HPP
