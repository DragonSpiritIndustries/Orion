#ifndef IEVENT_HPP
#define IEVENT_HPP

#include "Global.hpp"

struct KeyboardEvent
{
    bool pressed;
    int  keyCode;
    int  scanCode;
    char modifier;
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
