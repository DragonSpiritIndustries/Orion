#include <iostream>
#include <SDLApplication.hpp>

void testDraw1(IApplication* app)
{
    app->drawRectangle(32, 32, 100, 200);
}

void testDraw2(IApplication* app)
{
    app->drawRectangle(32, 64, 16, 200);
}

int main(int argc, char* argv[])
{
    IApplication* game = new SDLApplication();
    IApplication::instanceByPtr()->init(argc, argv);
    IApplication::instanceByRef().setTitle("SDLTest");
    IApplication::instanceByPtr()->drawSignal().connect<testDraw1>();
    IApplication::instanceByRef().drawSignal().connect<testDraw2>();
    int ret = IApplication::instanceByRef().exec();
    delete IApplication::instanceByPtr();
    return ret;
}
