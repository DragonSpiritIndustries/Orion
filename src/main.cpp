#include <iostream>
#include <ApplicationBase.hpp>

int main(int argc, char* argv[])
{
    // seed random number generator
    srand(time(nullptr));
    if (orApplicationPtr->init(argc, argv))
    {
        Viewport vp(0, 0, 256, 224);
        orApplicationRef.setViewport(vp);
        return orApplicationPtr->exec();
    }
    else
    {
        orConsoleRef.print(orConsoleRef.Fatal, "Orion failed to start\n");
        return 1;
    }
}
