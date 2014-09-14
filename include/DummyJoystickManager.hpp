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

#ifndef DUMMYJOYSTICKMANAGER_HPP
#define DUMMYJOYSTICKMANAGER_HPP

#include "IJoystickManager.hpp"

class DummyJoystickManager : public IJoystickManager
{
public:
    DummyJoystickManager()      {}
    ~DummyJoystickManager()     {}
    void onJoystickAdded(int)   {}
    void onJoystickRemoved(int) {}

    float axisPosition (int, int){return 0.0f;}
    bool buttonPressed (int, int){return false;}
    bool buttonReleased(int, int){return false;}

    std::string name(int) { return std::string(); }

    int maxButtons(int ) { return 0; }
    int maxAxes(int )    { return 0; }

    void motorOn(int)  {}
    void motorOff(int) {}
    void shutdown()    {}
    bool isPluggedIn(int) { return false; }

private:
};

#endif // DUMMYJOYSTICKMANAGER_HPP
