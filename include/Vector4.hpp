/**************************************************************************
**
**************************************************************************/

#ifndef VECTOR4_HPP
#define VECTOR4_HPP

template <typename T>
struct Vector4
{
    T X, Y, Z, W;

    Vector4<T>();
    Vector4<T>(const T& x, const T& y, const T& z, const T& w);

    Vector4<T> normalize();
};

typedef Vector4<float>  Vector4f;
typedef Vector4<int>    Vector4i;
typedef Vector4<double> Vector4d;

template <typename T>
inline Vector4<T>& operator +=(const Vector4<T>& left, const Vector4<T>& right);
template <typename T>
inline Vector4<T>& operator -=(const Vector4<T>& left, const Vector4<T>& right);
template <typename T>
inline bool operator ==(const Vector4<T>& left, const Vector4<T>& right);
template <typename T>
inline bool operator !=(const Vector4<T>& left, const Vector4<T>& right);
template <typename T>
inline Vector4<T>& operator +(const Vector4<T>& left, const Vector4<T>& right);
template <typename T>
inline Vector4<T>& operator -(const Vector4<T>& left, const Vector4<T>& right);
template <typename T>
inline Vector4<T>& operator -(const Vector4<T>& right);
template <typename T>
inline Vector4<T>& operator *(const Vector4<T>& left, T right);
template <typename T>
inline Vector4<T>& operator *(T left, const Vector4<T>& right);
template <typename T>
inline Vector4<T>& operator *=(const Vector4<T>& left, T right);
template <typename T>
inline Vector4<T>& operator /(const Vector4<T>& left, T right);
template <typename T>
inline Vector4<T>& operator /=(const Vector4<T>& left, T right);

#include "Vector4.inl"
#endif // VECTOR4_HPP
