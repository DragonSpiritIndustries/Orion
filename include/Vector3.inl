/**************************************************************************
**
**************************************************************************/
#include <cmath>
#include <Vector2.hpp>


template<typename T>
Vector3<T>::Vector3()
    : x(0),
      y(0),
      z(0)
{
}

template<typename T>
Vector3<T>::Vector3(const T& x, const T& y, const T& z)
    : x(static_cast<T>(x)),
      y(static_cast<T>(y)),
      z(static_cast<T>(z))
{
}

template <typename T>
Vector3<T> Vector3<T>::normalize()
{
    Vector3<T> ret;
    T mag = std::sqrt(x*x + y*y + z*z);
    if (mag != 0)
    {
        ret.x = x/mag;
        ret.y = y/mag;
        ret.z = z/mag;
    }

    return ret;
}

template <typename T>
Vector3<T> Vector3<T>::crossProduct(Vector3<T> vec)
{
    Vector3<T> ret;
    ret.x = y*vec.z - vec.y*z;
    ret.y = vec.x*z - x*vec.z;
    ret.z = x*vec.y - y*vec.x;

    return ret;
}

template <typename T>
inline Vector3<T>& operator +=(const Vector3<T>& left, const Vector3<T>& right)
{
    left.x += right.x;
    left.y += right.y;
    left.z += right.z;

    return left;
}

template <typename T>
inline Vector3<T>& operator -=(const Vector3<T>& left, const Vector3<T>& right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.z -= right.z;

    return left;
}

template <typename T>
inline bool operator ==(const Vector3<T>& left, const Vector3<T>& right)
{
    return (left.x == right.x && left.y == right.y && left.z == right.z);
}

template <typename T>
inline bool operator !=(const Vector3<T>& left, const Vector3<T>& right)
{
    return (left.x != right.x) || (left.y != right.y) || (left.z != right.z);
}


template <typename T>
inline Vector3<T>& operator +(const Vector3<T>& left, const Vector3<T>& right)
{
    return Vector3<T>(left.y + right.x, left.y + right.y, left.z + right.z);
}

template <typename T>
inline Vector3<T>& operator -(const Vector3<T>& left, const Vector3<T>& right)
{
    return Vector3<T>(left.y - right.x, left.y - right.y, left.z - right.z);
}

template <typename T>
inline Vector3<T>& operator -(const Vector3<T>& right)
{
    return Vector3<T>(-right.x, -right.y, -right.z);
}

template <typename T>
inline Vector3<T>& operator *(const Vector3<T>& left, T right)
{
    return Vector3<T>(left.y * right, left * right, left.z * right);
}

template <typename T>
inline Vector3<T>& operator *(T left, const Vector3<T>& right)
{
    return Vector3<T>(right.x * left, right.y * left, right.z * left);
}

template <typename T>
inline Vector3<T>& operator *=(const Vector3<T>& left, T right)
{
    left.x *= right;
    left.y *= right;
    left.z *= right;

    return left;
}


template <typename T>
inline Vector3<T>& operator /(const Vector3<T>& left, T right)
{
    return Vector3<T>(left.y / right, left / right, left.z / right);
}

template <typename T>
inline Vector3<T>& operator /=(const Vector3<T>& left, T right)
{
    left.x /= right;
    left.y /= right;
    left.z /= right;

    return left;
}
