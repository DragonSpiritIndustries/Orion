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

#ifndef IEVENT_HPP
#define IEVENT_HPP

#include "Global.hpp"
#include "IKeyboardManager.hpp"

struct KeyboardEvent
{
    bool pressed;
    char  keyCode;
    Key  scanCode;
    short modifier;
};

struct TextEvent
{
    char* string;
};

struct MouseButtonEvent
{
    char button;
    unsigned  x,y;
};

struct MouseMoveEvent
{
    unsigned x, y;
};

//!< Fired when the mouse wheel is moved
struct MouseWheelEvent
{
    int vDelta; // Negative is down, Positive is up
    int hDelta; // Negative is left, Positive is right
    unsigned x, y;
};

struct JoystickEvent
{
    int   id;
    char  button;
    char  axis;
    float position;
};

struct ResizeEvent
{
    int width, height;
};

struct FocusEvent
{
    bool focus;
};

class Event
{
public:
    enum Type : char
    {
        EV_QUIT,
        EV_KEY_PRESSED,
        EV_KEY_RELEASED,
        EV_TEXT_ENTERED,
        EV_MOUSE_BTN_PRESSED,
        EV_MOUSE_BTN_RELEASED,
        EV_MOUSE_MOVED,
        EV_MOUSE_WHEEL,
        EV_WINDOW_RESIZED,
        EV_JOY_BTN_PRESSED,
        EV_JOY_BTN_RELEASED,
        EV_JOY_AXIS,
        EV_FOCUS_LOST,
        EV_FOCUS_GAINED
    };

    // To be filled in by the class Implementing IEvent
    union
    {
        KeyboardEvent    keyboardEvent;
        TextEvent        textEvent;
        MouseButtonEvent mouseButtonEvent;
        MouseMoveEvent   mouseMoveEvent;
        MouseWheelEvent  mouseWheelEvent;
        JoystickEvent    joystickEvent;
        ResizeEvent      resizeEvent;
        FocusEvent       focusEvent;
    } eventData;

    Type type;
};

#endif // IEVENT_HPP
