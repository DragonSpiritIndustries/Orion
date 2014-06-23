/**************************************************************************
**
**************************************************************************/
#include <cmath>

template<typename T>
Rectangle<T>::Rectangle()
    : x(0),
      y(0),
      w(0),
      h(0)
{
}

template<typename T>
Rectangle<T>::Rectangle(const T& x, const T& y, const T& w, const T& h)
    : x(static_cast<T>(x)),
      y(static_cast<T>(y)),
      w(static_cast<T>(w)),
      h(static_cast<T>(h))
{
}

template <typename T>
Rectangle<T> Rectangle<T>::normalize()
{
    Rectangle<T> ret;
    T mag = std::sqrt(x*x + y*y + w*w + h*h);
    if (mag != 0)
    {
        ret.x = x/mag;
        ret.y = y/mag;
        ret.w = w/mag;
        ret.h = h/mag;
    }

    return ret;
}

template <typename T>
inline Rectangle<T>& operator +=(const Rectangle<T>& left, const Rectangle<T>& right)
{
    left.x += right.x;
    left.y += right.y;
    left.w += right.w;
    left.h += right.h;

    return left;
}

template <typename T>
inline Rectangle<T>& operator -=(const Rectangle<T>& left, const Rectangle<T>& right)
{
    left.x -= right.x;
    left.y -= right.y;
    left.w -= right.w;
    left.h -= right.h;

    return left;
}

template <typename T>
inline bool operator ==(const Rectangle<T>& left, const Rectangle<T>& right)
{
    return (left.x == right.x && left.y == right.y && left.w == right.w && left.h == right.h);
}

template <typename T>
inline bool operator !=(const Rectangle<T>& left, const Rectangle<T>& right)
{
    return (left.x != right.x) || (left.y != right.y) || (left.w != right.w) || (left.h != right.h);
}


template <typename T>
inline Rectangle<T>& operator +(const Rectangle<T>& left, const Rectangle<T>& right)
{
    return Rectangle<T>(left.y + right.x, left.y + right.y, left.w + right.w, left.h + right.h);
}

template <typename T>
inline Rectangle<T>& operator -(const Rectangle<T>& left, const Rectangle<T>& right)
{
    return Rectangle<T>(left.y - right.x, left.y - right.y, left.w - right.w, left.h - right.h);
}

template <typename T>
inline Rectangle<T>& operator -(const Rectangle<T>& right)
{
    return Rectangle<T>(-right.x, -right.y, -right.w, -right.h);
}

template <typename T>
inline Rectangle<T>& operator *(const Rectangle<T>& left, T right)
{
    return Rectangle<T>(left.y * right, left * right, left.w * right, left.h * right);
}

template <typename T>
inline Rectangle<T>& operator *(T left, const Rectangle<T>& right)
{
    return Rectangle<T>(right.x * left, right.y * left, right.w * left, right.h * left);
}

template <typename T>
inline Rectangle<T>& operator *=(const Rectangle<T>& left, T right)
{
    left.x *= right;
    left.y *= right;
    left.w *= right;
    left.h *= right;

    return left;
}


template <typename T>
inline Rectangle<T>& operator /(const Rectangle<T>& left, T right)
{
    return Rectangle<T>(left.y / right, left / right, left.w / right, left.h / right);
}

template <typename T>
inline Rectangle<T>& operator /=(const Rectangle<T>& left, T right)
{
    left.x /= right;
    left.y /= right;
    left.w /= right;
    left.h /= right;

    return left;
}
