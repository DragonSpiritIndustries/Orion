#include <new>

template <typename T>
Color<T>::Color()
    : r((T)0),
      g((T)0),
      b((T)0),
      a((T)255)
{
}

template <typename T>
Color<T>::Color(T r, T g, T b, T a)
    : r(static_cast<T>(r)),
      g(static_cast<T>(g)),
      b(static_cast<T>(b)),
      a(static_cast<T>(a))
{
}

// Common colors
template <typename T>
const Color<T> Color<T>::black  (  0,   0,   0);
template <typename T>
const Color<T> Color<T>::white  (255, 255, 255);
template <typename T>
const Color<T> Color<T>::grey   (128, 128, 128);
template <typename T>
const Color<T> Color<T>::red    (255,   0,   0);
template <typename T>
const Color<T> Color<T>::green  (  0, 255,   0);
template <typename T>
const Color<T> Color<T>::blue   (  0,   0, 255);
template <typename T>
const Color<T> Color<T>::cyan   (  0, 255, 255);
template <typename T>
const Color<T> Color<T>::magenta(255,   0, 255);
template <typename T>
const Color<T> Color<T>::yellow (255, 255,   0);

// ADDITION
template <typename T>
inline Color<T> operator +(const Color<T>& left, const Color<T>& right)
{
    return Color<T>(left.r + right.r, left.g + right.g, left.b + right.b, left.a + right.a);
}

template <typename T>
inline Color<T> operator +(const Color<T>& left, T right)
{
    return Color<T>(left.r + right, left.g + right, left.b + right, left.a + right);
}

template <typename T>
inline Color<T> operator +(T left, const Color<T>& right)
{
    return Color<T>(right.r + left, right.g + left, right.b + left, right.a + left);
}

template <typename T>
inline Color<T> operator +=(const Color<T>& left, const Color<T>& right)
{
    left.r += right.r;
    left.g += right.g;
    left.b += right.b;
    left.a += right.a;

    return left;
}

template <typename T>
inline Color<T> operator +=(const Color<T>& left, T right)
{
    left.r += right;
    left.g += right;
    left.b += right;
    left.a += right;

    return left;
}

// SUBRACTION
template <typename T>
inline Color<T> operator -(const Color<T>& left, const Color<T>& right)
{
    return Color<T>(left.r - right.r, left.g - right.g, left.b - right.b, left.a - right.a);
}


template <typename T>
inline Color<T> operator -(const Color<T>& left, T right)
{
    return Color<T>(left.r - right, left.g - right, left.b - right, left.a - right);
}

template <typename T>
inline Color<T> operator -(T left, const Color<T>& right)
{
    return Color<T>(right.r - left, right.g - left, right.b - left, right.a - left);
}

template <typename T>
inline Color<T> operator -=(const Color<T>& left, const Color<T>& right)
{
    left.r -= right.r;
    left.g -= right.g;
    left.b -= right.b;
    left.a -= right.a;

    return left;
}

template <typename T>
inline Color<T> operator -=(const Color<T>& left, T right)
{
    left.r -= right;
    left.g -= right;
    left.b -= right;
    left.a -= right;

    return left;
}

// MULTIPLICATION
template <typename T>
inline Color<T> operator *(const Color<T>& left, const Color<T>& right)
{
    return Color<T>(left.r * right.r, left.g * right.g, left.b * right.b, left.a * right.a);
}

template <typename T>
inline Color<T> operator *(const Color<T>& left, T right)
{
    return Color<T>(left.r * right, left.g * right, left.b * right, left.a * right);
}

template <typename T>
inline Color<T> operator *(T left, const Color<T>& right)
{
    return Color<T>(right.r * left, right.g * left, right.b * left, right.a * left);
}

template <typename T>
inline Color<T> operator *=(const Color<T>& left, const Color<T>& right)
{
    left.r *= right.r;
    left.g *= right.g;
    left.b *= right.b;
    left.a *= right.a;

    return left;
}

template <typename T>
inline Color<T> operator *=(const Color<T>& left, T right)
{
    left.r *= right;
    left.g *= right;
    left.b *= right;
    left.a *= right;

    return left;
}

//DIVISION
template <typename T>
inline Color<T> operator /(const Color<T>& left, const Color<T>& right)
{
    return Color<T>(left.r / right.r, left.g / right.g, left.b / right.b, left.a / right.a);
}

template <typename T>
inline Color<T> operator /(const Color<T>& left, T right)
{
    return Color<T>(left.r / right, left.g / right, left.b / right, left.a / right);
}

template <typename T>
inline Color<T>&operator /(T left, const Color<T>& right)
{
    return Color<T>(right.r / left, right.g / left, right.b / left, right.a / left);
}

template <typename T>
inline Color<T> operator /=(const Color<T>& left, const Color<T>& right)
{
    left.r /= right.r;
    left.g /= right.g;
    left.b /= right.b;
    left.a /= right.a;

    return left;
}

template <typename T>
inline Color<T> operator /=(const Color<T>& left, T right)
{
    left.r /= right;
    left.g /= right;
    left.b /= right;
    left.a /= right;

    return left;
}

// COMPARE
template <typename T>
inline bool operator ==(const Color<T>& left, const Color<T>& right)
{
    return ((left.r == right.r) &&
            (left.g == right.g) &&
            (left.b == right.b) &&
            (left.a == right.a));
}

template <typename T>
inline bool operator !=(const Color<T>& left, const Color<T>& right)
{
    return ((left.r != right.r) ||
            (left.g != right.g) ||
            (left.b != right.b) ||
            (left.a != right.a));
}

template <typename T>
void constructColor(Color<T>* vector)
{
    new(vector) Color<T>;
}

template <typename T>
void constructColorArgs(T r, T g, T b, T a, Color<T>* color)
{
    new(color) Color<T>(r, g, b, a);
}

template <typename T>
void destructColor(Color<T>* pointer)
{
    pointer->~Color<T>();
}
