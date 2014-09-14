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

#ifndef COLOR_HPP
#define COLOR_HPP

template <typename T>
class Color
{
public:
    T r,g,b,a;

    Color();
    Color(T r, T g, T b, T a=255);

    static const Color<T> black;
    static const Color<T> white;
    static const Color<T> grey;
    static const Color<T> red;
    static const Color<T> green;
    static const Color<T> blue;
    static const Color<T> cyan;
    static const Color<T> magenta;
    static const Color<T> yellow;
};

typedef Color<char>       Colorb;
typedef Color<float>      Colorf;
typedef Color<int>        Colori;
typedef Color<signed int> Colors;

template <typename T>
inline Color<T> operator +(const Color<T>& left, const Color<T>& right);

template <typename T>
inline Color<T> operator +(const Color<T>& left, T right);

template <typename T>
inline Color<T> operator +(T left, const Color<T>& right);

template <typename T>
inline bool operator +=(const Color<T>& left, const Color<T>& right);

template <typename T>
inline Color<T> operator -(const Color<T>& left, const Color<T>& right);

template <typename T>
inline Color<T> operator -(const Color<T>& left, T right);

template <typename T>
inline Color<T> operator -(T left, const Color<T>& right);

template <typename T>
inline Color<T> operator -=(const Color<T>& left, const Color<T>& right);

template <typename T>
inline Color<T> operator *(const Color<T>& left, const Color<T>& right);

template <typename T>
inline Color<T> operator *(const Color<T>& left, T right);

template <typename T>
inline Color<T> operator *(T left, const Color<T>& right);

template <typename T>
inline Color<T> operator *=(const Color<T>& left, const Color<T>& right);

template <typename T>
inline Color<T> operator *=(const Color<T>& left, T right);

template <typename T>
inline Color<T> operator /(const Color<T>& left, const Color<T>& right);

template <typename T>
inline Color<T> operator /(const Color<T>& left, T right);

template <typename T>
inline Color<T> operator /(T left, const Color<T>& right);

template <typename T>
inline Color<T> operator /=(const Color<T>& left, const Color<T>& right);

template <typename T>
inline Color<T> operator /=(const Color<T>& left, T right);

template <typename T>
inline bool operator ==(const Color<T>& left, const Color<T>& right);

template <typename T>
inline bool operator !=(const Color<T>& left, const Color<T>& right);

template <typename T>
void constructColor(Color<T>* vector);

template <typename T>
void constructColorArgs(T r, T g, T b, T a, Color<T>* color);

template <typename T>
void destructColor(Color<T>* pointer);

#include "Color.inl"

#endif // COLOR_HPP
