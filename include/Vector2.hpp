/**************************************************************************
**
**************************************************************************/

#ifndef VECTOR2_HPP
#define VECTOR2_HPP

template <typename T>
class Vector2
{
public:
    T x, y;

    Vector2<T>();
    Vector2<T>(const T& x, const T& y);

    Vector2<T> normalize();
    T crossProduct(Vector2<T> vec);
};

typedef Vector2<float>  Vector2f;
typedef Vector2<int>    Vector2i;
typedef Vector2<double> Vector2d;

template <typename T>
inline Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right);
template <typename T>
inline Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right);
template <typename T>
inline bool operator ==(const Vector2<T>& left, const Vector2<T>& right);
template <typename T>
inline bool operator !=(const Vector2<T>& left, const Vector2<T>& right);
template <typename T>
inline Vector2<T>& operator +(const Vector2<T>& left, const Vector2<T>& right);
template <typename T>
inline Vector2<T>& operator -(const Vector2<T>& left, const Vector2<T>& right);
template <typename T>
inline Vector2<T>& operator -(const Vector2<T>& right);
template <typename T>
inline Vector2<T> operator *(const Vector2<T>& left, T right);
template <typename T>
inline Vector2<T> operator *(T left, const Vector2<T>& right);
template <typename T>
inline Vector2<T> operator *=(Vector2<T>& left, T right);
template <typename T>
inline Vector2<T>& operator /(const Vector2<T>& left, T right);
template <typename T>
inline Vector2<T>& operator /=(Vector2<T>& left, T right);

void constructVector2f(Vector2f* vector);

void constructVector2fArgs(float x, float y, Vector2f* vector);

void destructVector2f(Vector2f* pointer);

void constructVector2d(Vector2d* vector);

void constructVector2dArgs(double x, double y, Vector2d* vector);

void destructVector2d(Vector2d* pointer);

void constructVector2i(Vector2i* vector);

void constructVector2iArgs(int x, int y, Vector2i* vector);

void destructVector2i(Vector2i* pointer);

#include "Vector2.inl"
#endif // VECTOR2_HPP
