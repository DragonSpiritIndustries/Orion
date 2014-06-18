/**************************************************************************
**
**************************************************************************/
#include <cmath>


template<typename T>
Vector4<T>::Vector4()
    : X(0),
      Y(0),
      Z(0),
      W(0)
{
}

template<typename T>
Vector4<T>::Vector4(const T& x, const T& y, const T& z, const T& w)
    : X(static_cast<T>(x)),
      Y(static_cast<T>(y)),
      Z(static_cast<T>(z)),
      W(static_cast<T>(w))
{
}

template <typename T>
Vector4<T> Vector4<T>::normalize()
{
    Vector4<T> ret;
    T mag = std::sqrt(X*X + Y*Y + Z*Z + W*W);
    if (mag != 0)
    {
        ret.X = X/mag;
        ret.Y = Y/mag;
        ret.Z = Z/mag;
        ret.W = W/mag;
    }

    return ret;
}

template <typename T>
inline Vector4<T>& operator +=(const Vector4<T>& left, const Vector4<T>& right)
{
    left.X += right.X;
    left.Y += right.Y;
    left.Z += right.Z;
    left.W += right.W;

    return left;
}

template <typename T>
inline Vector4<T>& operator -=(const Vector4<T>& left, const Vector4<T>& right)
{
    left.X -= right.X;
    left.Y -= right.Y;
    left.Z -= right.Z;
    left.W -= right.W;

    return left;
}

template <typename T>
inline bool operator ==(const Vector4<T>& left, const Vector4<T>& right)
{
    return (left.X == right.X && left.Y == right.Y && left.Z == right.Z && left.W == right.W);
}

template <typename T>
inline bool operator !=(const Vector4<T>& left, const Vector4<T>& right)
{
    return (left.X != right.X) || (left.Y != right.Y) || (left.Z != right.Z) || (left.W != right.W);
}


template <typename T>
inline Vector4<T>& operator +(const Vector4<T>& left, const Vector4<T>& right)
{
    return Vector4<T>(left.Y + right.X, left.Y + right.Y, left.Z + right.Z, left.W + right.W);
}

template <typename T>
inline Vector4<T>& operator -(const Vector4<T>& left, const Vector4<T>& right)
{
    return Vector4<T>(left.Y - right.X, left.Y - right.Y, left.Z - right.Z, left.W - right.W);
}

template <typename T>
inline Vector4<T>& operator -(const Vector4<T>& right)
{
    return Vector4<T>(-right.X, -right.Y, -right.Z, -right.W);
}

template <typename T>
inline Vector4<T>& operator *(const Vector4<T>& left, T right)
{
    return Vector4<T>(left.Y * right, left * right, left.Z * right, left.W * right);
}

template <typename T>
inline Vector4<T>& operator *(T left, const Vector4<T>& right)
{
    return Vector4<T>(right.X * left, right.Y * left, right.Z * left, right.W * left);
}

template <typename T>
inline Vector4<T>& operator *=(const Vector4<T>& left, T right)
{
    left.X *= right;
    left.Y *= right;
    left.Z *= right;
    left.W *= right;

    return left;
}


template <typename T>
inline Vector4<T>& operator /(const Vector4<T>& left, T right)
{
    return Vector4<T>(left.Y / right, left / right, left.Z / right, left.W / right);
}

template <typename T>
inline Vector4<T>& operator /=(const Vector4<T>& left, T right)
{
    left.X /= right;
    left.Y /= right;
    left.Z /= right;
    left.W /= right;

    return left;
}
