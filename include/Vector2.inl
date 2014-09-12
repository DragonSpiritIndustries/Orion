/**************************************************************************
**
**************************************************************************/
#include <cmath>
#include <new>

inline float degToRad(float angle)
{
    return angle*(M_PI/180);
}

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


template<typename T>
void Vector2<T>::rotate(float angle, Vector2<T> origin)
{
    float ct = cos(degToRad(angle));
    float st = sin(degToRad(angle));
    float tx;

    tx = ((x - origin.x) * ct) - ((origin.y - y) * st) + origin.x;
    y = ((origin.y - y) * ct) - ((x - origin.x) * st) + origin.y;
    x = tx;
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

template <typename T>
void constructVector2(Vector2<T>* vector)
{
    new(vector) Vector2<T>;
}

template <typename T>
void constructVector2Args(T x, T y, Vector2<T>* vector)
{
    new(vector) Vector2<T>(x, y);
}

template <typename T>
void destructVector2(Vector2<T>* pointer)
{
    pointer->~Vector2<T>();
}
