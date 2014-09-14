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

#include "Vector3.hpp"
#include <new>

void constructVector3f(Vector3f* vector)
{
    new(vector) Vector3f;
}

void constructVector3fArgs(float x, float y, float z, Vector3f* vector)
{
    new(vector) Vector3f(x, y, z);
}

void destructVector3f(Vector3f* pointer)
{
    pointer->~Vector3f();
}

void constructVector3d(Vector3d* vector)
{
    new(vector) Vector3d;
}

void constructVector3dArgs(double x, double y, double z, Vector3d* vector)
{
    new(vector) Vector3d(x, y, z);
}

void destructVector3d(Vector3d* pointer)
{
    pointer->~Vector3d();
}

void constructVector3i(Vector3i* vector)
{
    new(vector) Vector3i;
}

void constructVector3iArgs(int x, int y, int z, Vector3i* vector)
{
    new(vector) Vector3i(x, y, z);
}

void destructVector3i(Vector3i* pointer)
{
    pointer->~Vector3i();
}

