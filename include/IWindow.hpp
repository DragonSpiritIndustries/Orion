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

#ifndef IWINDOW_HPP
#define IWINDOW_HPP

#include "Event.hpp"
#include <string>
#include "Vector2.hpp"

//! This should remain a private member
//! refrain from making it publicly accessible
//! doing so will violate Orions ability to abstract
//! core window functionality.
//! your IApplication implementation is required to expose
//! setTitle, title, windowSize, and window* functions anyway
class IWindow
{
public:
    virtual bool initialize()=0;
    virtual void setTitle(const std::string&)=0;
    virtual std::string title() const=0;


    virtual void* handle() const=0;

    virtual Vector2i windowSize()=0;
    virtual void setWindowSize(int w, int h)=0;
    virtual void setWindowSize(const Vector2i& size)=0;
    virtual void setWindowWidth(int w)=0;
    virtual int windowWidth()=0;
    virtual void setWindowHeight(int h)=0;
    virtual int windowHeight()=0;
};

#endif // IWINDOW_HPP
