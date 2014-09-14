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

#ifndef MATRIX3X3_HPP
#define MATRIX3X3_HPP

#include <Vector3.hpp>

template <typename T>
struct Matrix3x3
{
    Vector3<T> X, Y, Z;
    Matrix3x3<T>();
    Matrix3x3<T>(const Vector2<T>& x, const Vector2<T>& y, const Vector2<T>& z);
};


typedef Matrix3x3<float>  Matrix3x3f;
typedef Matrix3x3<int>    Matrix3x3i;
typedef Matrix3x3<double> Matrix3x3d;

#include "Matrix3x3.inl"
#endif // MATRIX3X3_HPP
