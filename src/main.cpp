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
