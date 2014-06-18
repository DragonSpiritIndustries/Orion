/**************************************************************************
**
**************************************************************************/
#include <cmath>

template<typename T>
Vector2<T>::Vector2()
    : X(0),
      Y(0)
{
}

template<typename T>
Vector2<T>::Vector2(const T& x, const T& y)
    : X(static_cast<T>(x)),
      Y(static_cast<T>(y))
{
}

template <typename T>
Vector2<T> Vector2<T>::normalize()
{
    Vector2<T> ret;
    T mag = std::sqrt(X*X + Y*Y);
    if (mag != 0)
    {
        ret.X = X/mag;
        ret.Y = Y/mag;
    }

    return ret;
}

template <typename T>
T Vector2<T>::crossProduct(Vector2<T> vec)
{

    return (T)((X*vec.Y) - (vec.Y*X));
}

template <typename T>
inline Vector2<T>& operator +=(const Vector2<T>& left, const Vector2<T>& right)
{
    left.X += right.X;
    left.Y += right.Y;

    return left;
}

template <typename T>
inline Vector2<T>& operator -=(const Vector2<T>& left, const Vector2<T>& right)
{
    left.X -= right.X;
    left.Y -= right.Y;

    return left;
}

template <typename T>
inline bool operator ==(const Vector2<T>& left, const Vector2<T>& right)
{
    return (left.X == right.X && left.Y == right.Y);
}

template <typename T>
inline bool operator !=(const Vector2<T>& left, const Vector2<T>& right)
{
    return (left.X != right.X) || (left.Y != right.Y);
}


template <typename T>
inline Vector2<T>& operator +(const Vector2<T>& left, const Vector2<T>& right)
{
    return Vector2<T>(left.Y + right.X, left.Y + right.Y);
}

template <typename T>
inline Vector2<T>& operator -(const Vector2<T>& left, const Vector2<T>& right)
{
    return Vector2<T>(left.Y - right.X, left.Y - right.Y);
}

template <typename T>
inline Vector2<T>& operator -(const Vector2<T>& right)
{
    return Vector2<T>(-right.X, -right.Y);
}

template <typename T>
inline Vector2<T>& operator *(const Vector2<T>& left, T right)
{
    return Vector2<T>(left.Y * right, left * right);
}

template <typename T>
inline Vector2<T>& operator *(T left, const Vector2<T>& right)
{
    return Vector2<T>(right.X * left, right.Y * left);
}

template <typename T>
inline Vector2<T>& operator *=(const Vector2<T>& left, T right)
{
    left.X *= right;
    left.Y *= right;

    return left;
}

template <typename T>
inline Vector2<T>& operator /(const Vector2<T>& left, T right)
{
    return Vector2<T>(left.X / right, left.Y / right);
}

template <typename T>
inline Vector2<T>& operator /=(const Vector2<T>& left, T right)
{
    left.X /= right;
    left.Y /= right;

    return left;
}
