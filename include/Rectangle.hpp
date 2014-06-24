/**************************************************************************
**
**************************************************************************/

#ifndef VECTOR4_HPP
#define VECTOR4_HPP

template <typename T>
class Rectangle
{
public:
    T x, y, w, h;

    Rectangle<T>();
    Rectangle<T>(const T& x, const T& y, const T& width, const T& w);

    Rectangle<T> normalize();
};

typedef Rectangle<float>  Rectanglef;
typedef Rectangle<int>    Rectanglei;
typedef Rectangle<double> Rectangled;

template <typename T>
inline Rectangle<T>& operator +=(const Rectangle<T>& left, const Rectangle<T>& right);
template <typename T>
inline Rectangle<T>& operator -=(const Rectangle<T>& left, const Rectangle<T>& right);
template <typename T>
inline bool operator ==(const Rectangle<T>& left, const Rectangle<T>& right);
template <typename T>
inline bool operator !=(const Rectangle<T>& left, const Rectangle<T>& right);
template <typename T>
inline Rectangle<T>& operator +(const Rectangle<T>& left, const Rectangle<T>& right);
template <typename T>
inline Rectangle<T>& operator -(const Rectangle<T>& left, const Rectangle<T>& right);
template <typename T>
inline Rectangle<T>& operator -(const Rectangle<T>& right);
template <typename T>
inline Rectangle<T>& operator *(const Rectangle<T>& left, T right);
template <typename T>
inline Rectangle<T>& operator *(T left, const Rectangle<T>& right);
template <typename T>
inline Rectangle<T>& operator *=(const Rectangle<T>& left, T right);
template <typename T>
inline Rectangle<T>& operator /(const Rectangle<T>& left, T right);
template <typename T>
inline Rectangle<T>& operator /=(const Rectangle<T>& left, T right);

#include "Rectangle.inl"
#endif // VECTOR4_HPP
