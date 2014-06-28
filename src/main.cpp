#include <iostream>
#include <ObjectManager.hpp>
#include <TestObject.hpp>

int main(int argc, char* argv[])
{
    // seed random number generator
    srand(time(nullptr));
    if (orApplicationPtr->init(argc, argv))
    {
        orApplicationPtr->setClearColor(Colorf::cyan);
        return orApplicationPtr->exec();
    }
    else
    {
        orConsoleRef.print(orConsoleRef.Fatal, "Orion failed to start\n");
        return 1;
    }
}
