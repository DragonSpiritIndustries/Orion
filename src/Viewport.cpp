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

#include "Viewport.hpp"

Viewport::Viewport()
{
}

Viewport::Viewport(const float x, const float y, const int width, const int height)
{
    setLocation(x, y);
    setSize(width, height);
}

Viewport::Viewport(const Vector2f& location, const Vector2i& size)
    : m_location(location),
      m_size(size)
{
}

void Viewport::setLocation(float x, float y)
{
    setLocation(Vector2f(x, y));
}

void Viewport::setLocation(const Vector2f& location)
{
    m_location = location;
}

Vector2f Viewport::location() const
{
    return m_location;
}

float Viewport::x() const
{
    return m_location.x;
}

float Viewport::y() const
{
    return m_location.y;
}

void Viewport::setSize(const int w, const int h)
{
    setSize(Vector2i(w, h));
}

void Viewport::setSize(const Vector2i& size)
{
    m_size = size;
}

void Viewport::setWidth(const int w)
{
    setSize(w, height());
}

void Viewport::setHeight(const int h)
{
    setSize(width(), h);
}

int Viewport::width() const
{
    return m_size.x;
}

int Viewport::height() const
{
    return m_size.y;
}

Vector2i Viewport::size() const
{
    return m_size;
}


Vector2i Viewport::center()
{
    return Vector2i(m_size.x / 2, m_size.y / 2);
}
