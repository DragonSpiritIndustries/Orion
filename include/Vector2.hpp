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
    void rotate(float angle, Vector2<T> origin);
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
inline const Vector2<T>& operator -(const Vector2<T>& right);
template <typename T>
inline Vector2<T> operator *(Vector2<T>& left, T right);
template <typename T>
inline Vector2<T> operator *(T left, const Vector2<T>& right);
template <typename T>
inline Vector2<T> operator *=(Vector2<T>& left, T right);
template <typename T>
inline Vector2<T>& operator /(const Vector2<T>& left, T right);
template <typename T>
inline Vector2<T>& operator /=(Vector2<T>& left, T right);

template <typename T>
void constructVector2(Vector2<T>* vector);
template <typename T>
void constructVector2Args(T x, T y, Vector2<T>* vector);

template <typename T>
void destructVector2(Vector2<T>* pointer);

#include "Vector2.inl"
#endif // VECTOR2_HPP
