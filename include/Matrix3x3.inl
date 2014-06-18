template<typename T>
Matrix3x3<T>::Matrix3x3()
{
}

template<typename T>
Matrix3x3<T>::Matrix3x3(const Vector3<T>& x, const Vector3<T>& y, const Vector3<T>& z)
    : X(x),
      Y(y),
      Z(z)
{
}
