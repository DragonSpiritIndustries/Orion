/**************************************************************************
**
**************************************************************************/

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
