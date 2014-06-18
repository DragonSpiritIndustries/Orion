#ifndef MATRIX3X3_HPP
#define MATRIX3X3_HPP

#include <Vector3.hpp>

template <typename T>
struct Matrix3x3
{
    Vector3<T> X, Y, Z;
    Matrix3x3<T>();
    Matrix3x3<T>(const Vector2<T>& x, const Vector2<T>& y, const Vector2<T>& z);
};


typedef Matrix3x3<float>  Matrix3x3f;
typedef Matrix3x3<int>    Matrix3x3i;
typedef Matrix3x3<double> Matrix3x3d;

#include "Matrix3x3.inl"
#endif // MATRIX3X3_HPP
