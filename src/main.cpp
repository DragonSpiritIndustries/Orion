#include <iostream>
#include <SDLApplication.hpp>
#include <ObjectManager.hpp>
#include <TestObject.hpp>
#include <memory>

int main(int argc, char* argv[])
{
    orCreateApplication(new SDLApplication);
    if (orApplicationPtr->init(argc, argv))
    {
        orApplicationPtr->setClearColor(Colorb::cyan);
        orObjectManagerPtr->addObject(new TestObject);
        return IApplication::instanceRef().exec();
    }
    else
    {
        orDebug("Orion failed to start\n");
        return 1;
    }
}
