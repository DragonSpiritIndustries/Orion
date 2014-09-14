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

#ifndef DUMMYWINDOW_HPP
#define DUMMYWINDOW_HPP

#include "Global.hpp"
#include <string>
#include <IWindow.hpp>

class DummyWindow : public IWindow
{
public:
    DummyWindow();
    virtual ~DummyWindow() {}

    bool initialize();

    void setTitle(const std::string& title) {}
    std::string title() const { return std::string(); }

    void* handle() const { return nullptr; }

    Vector2i windowSize() {return Vector2i();}
    void setWindowSize(int, int) {}
    void setWindowSize(const Vector2i&){}
    void setWindowWidth(int) {}
    int windowWidth() {return 0;}
    void setWindowHeight(int) {}
    int windowHeight() { return 0; }
};

#endif // DUMMYWINDOW_HPP
