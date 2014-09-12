#include "Rectangle.hpp"
#include <new>

void constructRectanglef(Rectanglef* rectangle)
{
    new(rectangle) Rectanglef;
}

void constructRectanglefArgs(float x, float y, float w, float h, Rectanglef* rectangle)
{
    new(rectangle) Rectanglef(x, y, w, h);
}

void destructRectanglef(Rectanglef* pointer)
{
    pointer->~Rectanglef();
}

void constructRectangled(Rectangled* rectangle)
{
    new(rectangle) Rectangled;
}

void constructRectangledArgs(double x, double y, double w, double h, Rectangled* rectangle)
{
    new(rectangle) Rectangled(x, y, w, h);
}

void destructRectangled(Rectangled* pointer)
{
    pointer->~Rectangled();
}

void constructRectanglei(Rectanglei* rectangle)
{
    new(rectangle) Rectanglei;
}

void constructRectangleiArgs(int x, int y, int w, int h, Rectanglei* rectangle)
{
    new(rectangle) Rectanglei(x, y, w, h);
}

void destructRectanglei(Rectanglei* pointer)
{
    pointer->~Rectanglei();
}
