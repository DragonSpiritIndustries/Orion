/**************************************************************************
**
**************************************************************************/
#include <cmath>

template<typename T>
Vector2<T>::Vector2()
    : x(0),
      y(0)
{
}

template<typename T>
Vector2<T>::Vector2(const T& x, const T& y)
    : x(static_cast<T>(x)),
      y(static_cast<T>(y))
{
}

template <typename T>
Vector2<T> Vector2<T>::normalize()
{
    Vector2<T> ret;
    T mag = std::sqrt(x*x + y*y);
    if (mag != 0)
    {
        ret.x = x/mag;
        ret.y = y/mag;
    }

    return ret;
}

template <typename T>
T Vector2<T>::crossProduct(Vector2<T> vec)
{

    return (T)((x*vec.y) - (vec.y*x));
}

template <typename T>
inline Vector2<T>& operator +=(Vector2<T>& left, const Vector2<T>& right)
{
    left.x += right.x;
    left.y += right.y;

    return left;
}

template <typename T>
inline Vector2<T>& operator -=(Vector2<T>& left, const Vector2<T>& right)
{
    left.x -= right.x;
    left.y -= right.y;

    return left;
}

template <typename T>
inline bool operator ==(const Vector2<T>& left, const Vector2<T>& right)
{
    return (left.x == right.x && left.y == right.y);
}

template <typename T>
inline bool operator !=(const Vector2<T>& left, const Vector2<T>& right)
{
    return (left.x != right.x) || (left.y != right.y);
}


template <typename T>
inline Vector2<T>& operator +(const Vector2<T>& left, const Vector2<T>& right)
{
    return Vector2<T>(left.y + right.x, left.y + right.y);
}

template <typename T>
inline Vector2<T>& operator -(const Vector2<T>& left, const Vector2<T>& right)
{
    return Vector2<T>(left.y - right.x, left.y - right.y);
}

template <typename T>
inline Vector2<T> operator -(const Vector2<T>& right)
{
    return Vector2<T>(-right.x, -right.y);
}

template <typename T>
inline Vector2<T> operator *(const Vector2<T>& left, T right)
{
    return Vector2<T>(left.x * right, left.y * right);
}

template <typename T>
inline Vector2<T>& operator *(T left, const Vector2<T>& right)
{
    return Vector2<T>(right.x * left, right.y * left);
}

template <typename T>
inline Vector2<T>& operator *=(Vector2<T>& left, T right)
{
    left.x *= right;
    left.y *= right;

    return left;
}

template <typename T>
inline Vector2<T>& operator /(const Vector2<T>& left, T right)
{
    return Vector2<T>(left.x / right, left.y / right);
}

template <typename T>
inline Vector2<T>& operator /=(Vector2<T>& left, T right)
{
    left.x /= right;
    left.y /= right;

    return left;
}
