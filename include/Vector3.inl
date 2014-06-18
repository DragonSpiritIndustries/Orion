/**************************************************************************
**
**************************************************************************/
#include <cmath>
#include <Vector2.hpp>


template<typename T>
Vector3<T>::Vector3()
    : X(0),
      Y(0),
      Z(0)
{
}

template<typename T>
Vector3<T>::Vector3(const T& x, const T& y, const T& z)
    : X(static_cast<T>(x)),
      Y(static_cast<T>(y)),
      Z(static_cast<T>(z))
{
}

template <typename T>
Vector3<T> Vector3<T>::normalize()
{
    Vector3<T> ret;
    T mag = std::sqrt(X*X + Y*Y + Z*Z);
    if (mag != 0)
    {
        ret.X = X/mag;
        ret.Y = Y/mag;
        ret.Z = Z/mag;
    }

    return ret;
}

template <typename T>
Vector3<T> Vector3<T>::crossProduct(Vector3<T> vec)
{
    Vector3<T> ret;
    ret.X = Y*vec.Z - vec.Y*Z;
    ret.Y = vec.X*Z - X*vec.Z;
    ret.Z = X*vec.Y - Y*vec.X;

    return ret;
}

template <typename T>
inline Vector3<T>& operator +=(const Vector3<T>& left, const Vector3<T>& right)
{
    left.X += right.X;
    left.Y += right.Y;
    left.Z += right.Z;

    return left;
}

template <typename T>
inline Vector3<T>& operator -=(const Vector3<T>& left, const Vector3<T>& right)
{
    left.X -= right.X;
    left.Y -= right.Y;
    left.Z -= right.Z;

    return left;
}

template <typename T>
inline bool operator ==(const Vector3<T>& left, const Vector3<T>& right)
{
    return (left.X == right.X && left.Y == right.Y && left.Z == right.Z);
}

template <typename T>
inline bool operator !=(const Vector3<T>& left, const Vector3<T>& right)
{
    return (left.X != right.X) || (left.Y != right.Y) || (left.Z != right.Z);
}


template <typename T>
inline Vector3<T>& operator +(const Vector3<T>& left, const Vector3<T>& right)
{
    return Vector3<T>(left.Y + right.X, left.Y + right.Y, left.Z + right.Z);
}

template <typename T>
inline Vector3<T>& operator -(const Vector3<T>& left, const Vector3<T>& right)
{
    return Vector3<T>(left.Y - right.X, left.Y - right.Y, left.Z - right.Z);
}

template <typename T>
inline Vector3<T>& operator -(const Vector3<T>& right)
{
    return Vector3<T>(-right.X, -right.Y, -right.Z);
}

template <typename T>
inline Vector3<T>& operator *(const Vector3<T>& left, T right)
{
    return Vector3<T>(left.Y * right, left * right, left.Z * right);
}

template <typename T>
inline Vector3<T>& operator *(T left, const Vector3<T>& right)
{
    return Vector3<T>(right.X * left, right.Y * left, right.Z * left);
}

template <typename T>
inline Vector3<T>& operator *=(const Vector3<T>& left, T right)
{
    left.X *= right;
    left.Y *= right;
    left.Z *= right;

    return left;
}


template <typename T>
inline Vector3<T>& operator /(const Vector3<T>& left, T right)
{
    return Vector3<T>(left.Y / right, left / right, left.Z / right);
}

template <typename T>
inline Vector3<T>& operator /=(const Vector3<T>& left, T right)
{
    left.X /= right;
    left.Y /= right;
    left.Z /= right;

    return left;
}
