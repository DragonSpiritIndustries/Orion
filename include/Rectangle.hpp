// This file is part of Orion.
//
// Orion is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Orion is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Orion.  If not, see <http://www.gnu.org/licenses/>

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
