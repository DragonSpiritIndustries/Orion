#ifndef IEVENT_HPP
#define IEVENT_HPP

class IMouseEvent;
class IKeyboardEvent;
class IJoystickEvent;
class IResizeEvent;
class IQuitEvent;

class IEvent
{
public:
    enum class Type : char
    {
        EV_QUIT,
        EV_KEY_PRESSED,
        EV_KEY_RELEASED,
        EV_MOUSE_PRESSED,
        EV_MOUSE_RELEASED,
        EV_MOUSE_MOVED,
        EV_RESIZE
    };

    virtual Type type()=0;
    virtual IMouseEvent& mouseEvent()=0;
};

#endif // IEVENT_HPP
