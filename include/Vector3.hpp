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

#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include <Athena/Global.hpp>

template <typename T>
struct Vector3
{
    T x, y, z;

    Vector3<T>();
    Vector3<T>(const T& x, const T& y, const T& z);

    Vector3<T> normalize();
    Vector3<T> crossProduct(Vector3<T> vec);
};

typedef Vector3<float>  Vector3f;
typedef Vector3<int>    Vector3i;
typedef Vector3<double> Vector3d;

template <typename T>
inline Vector3<T>& operator +=(const Vector3<T>& left, const Vector3<T>& right);
template <typename T>
inline Vector3<T>& operator -=(const Vector3<T>& left, const Vector3<T>& right);
template <typename T>
inline bool operator ==(const Vector3<T>& left, const Vector3<T>& right);
template <typename T>
inline bool operator !=(const Vector3<T>& left, const Vector3<T>& right);
template <typename T>
inline Vector3<T>& operator +(const Vector3<T>& left, const Vector3<T>& right);
template <typename T>
inline Vector3<T>& operator -(const Vector3<T>& left, const Vector3<T>& right);
template <typename T>
inline Vector3<T>& operator -(const Vector3<T>& right);
template <typename T>
inline Vector3<T>& operator *(const Vector3<T>& left, T right);
template <typename T>
inline Vector3<T>& operator *(T left, const Vector3<T>& right);
template <typename T>
inline Vector3<T>& operator *=(const Vector3<T>& left, T right);
template <typename T>
inline Vector3<T>& operator /(const Vector3<T>& left, T right);
template <typename T>
inline Vector3<T>& operator /=(const Vector3<T>& left, T right);


void constructVector3f(Vector3f* vector);
void constructVector3fArgs(float x, float y, float z, Vector3f* vector);
void destructVector3f(Vector3f* pointer);
void constructVector3d(Vector3d* vector);
void constructVector3dArgs(double x, double y, double z, Vector3d* vector);
void destructVector3d(Vector3d* pointer);
void constructVector3i(Vector3i* vector);
void constructVector3iArgs(int x, int y, int z, Vector3i* vector);
void destructVector3i(Vector3i* pointer);

#include "Vector3.inl"
#endif // VECTOR3_HPP
