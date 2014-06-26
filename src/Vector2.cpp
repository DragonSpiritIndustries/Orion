#include "Vector2.hpp"
#include <new>

void constructVector2f(Vector2f* vector)
{
    new(vector) Vector2f;
}

void constructVector2fArgs(float x, float y, Vector2f* vector)
{
    new(vector) Vector2f(x, y);
}

void destructVector2f(Vector2f* pointer)
{
    pointer->~Vector2f();
}

void constructVector2d(Vector2d* vector)
{
    new(vector) Vector2d;
}

void constructVector2dArgs(double x, double y, Vector2d* vector)
{
    new(vector) Vector2d(x, y);
}

void destructVector2d(Vector2d* pointer)
{
    pointer->~Vector2d();
}

void constructVector2i(Vector2i* vector)
{
    new(vector) Vector2i;
}

void constructVector2iArgs(int x, int y, Vector2i* vector)
{
    new(vector) Vector2i(x, y);
}

void destructVector2i(Vector2i* pointer)
{
    pointer->~Vector2i();
}
