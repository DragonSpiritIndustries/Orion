/**************************************************************************
**
**************************************************************************/

#ifndef RECTANGLE_HPP
#define RECTANGLE_HPP
#include <Vector2.hpp>

template <typename T>
class Rectangle
{
public:
    T x, y, w, h;

    Rectangle<T>();
    Rectangle<T>(const T& x, const T& y, const T& width, const T& w);

    bool contains(T x, T y);
    bool contains(const Vector2<T>& pos);
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

void constructRectanglef(Rectanglef* vector);
void constructRectanglefArgs(float x, float y, float h, float w, Rectanglef* rectangle);
void destructRectanglef(Rectanglef* pointer);
void constructRectangled(Rectangled* vector);
void constructRectangledArgs(double x, double y, double w, double h, Rectangled* rectangle);
void destructRectangled(Rectangled* pointer);
void constructRectanglei(Rectanglei* vector);
void constructRectangleiArgs(int x, int y, int h, int w, Rectanglei* rectangle);
void destructRectanglei(Rectanglei* pointer);
#include "Rectangle.inl"
#endif // RECTANGLE_HPP
