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

#include "Rectangle.hpp"
#include <new>

void constructRectanglef(Rectanglef* rectangle)
{
    new(rectangle) Rectanglef;
}

void constructRectanglefArgs(float x, float y, float w, float h, Rectanglef* rectangle)
{
    new(rectangle) Rectanglef(x, y, w, h);
}

void destructRectanglef(Rectanglef* pointer)
{
    pointer->~Rectanglef();
}

void constructRectangled(Rectangled* rectangle)
{
    new(rectangle) Rectangled;
}

void constructRectangledArgs(double x, double y, double w, double h, Rectangled* rectangle)
{
    new(rectangle) Rectangled(x, y, w, h);
}

void destructRectangled(Rectangled* pointer)
{
    pointer->~Rectangled();
}

void constructRectanglei(Rectanglei* rectangle)
{
    new(rectangle) Rectanglei;
}

void constructRectangleiArgs(int x, int y, int w, int h, Rectanglei* rectangle)
{
    new(rectangle) Rectanglei(x, y, w, h);
}

void destructRectanglei(Rectanglei* pointer)
{
    pointer->~Rectanglei();
}
