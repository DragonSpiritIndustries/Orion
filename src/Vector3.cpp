#include "Vector3.hpp"
#include <new>

void constructVector3f(Vector3f* vector)
{
    new(vector) Vector3f;
}

void constructVector3fArgs(float x, float y, float z, Vector3f* vector)
{
    new(vector) Vector3f(x, y, z);
}

void destructVector3f(Vector3f* pointer)
{
    pointer->~Vector3f();
}

void constructVector3d(Vector3d* vector)
{
    new(vector) Vector3d;
}

void constructVector3dArgs(double x, double y, double z, Vector3d* vector)
{
    new(vector) Vector3d(x, y, z);
}

void destructVector3d(Vector3d* pointer)
{
    pointer->~Vector3d();
}

void constructVector3i(Vector3i* vector)
{
    new(vector) Vector3i;
}

void constructVector3iArgs(int x, int y, int z, Vector3i* vector)
{
    new(vector) Vector3i(x, y, z);
}

void destructVector3i(Vector3i* pointer)
{
    pointer->~Vector3i();
}

