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

#ifndef DUMMYMOUSEMANAGER_HPP
#define DUMMYMOUSEMANAGER_HPP

#include "IMouseManager.hpp"
#include <unordered_map>

class Event;
class DummyMouseManager : public IMouseManager
{
public:
    DummyMouseManager();
    void shutdown() {}
    bool buttonPressed(MouseButton)   {return false;}
    bool buttonReleased(MouseButton ) {return false;}

    Vector2i position()  { return Vector2i(); }
    Vector2i wheelDelta(){ return Vector2i(); }

    void setMousePosition(int, int)  {}
    void setMousePosition(Vector2i)  {}
protected:
    void onMouseButton(const Event&) {}
    void onMouseWheel(const Event&)  {}
};

#endif // DUMMYMOUSEMANAGER_HPP
