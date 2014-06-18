#ifndef IJOYSTICKMANAGER_HPP
#define IJOYSTICKMANAGER_HPP

#include <string>

class IJoystickManager
{
public:
    virtual ~IJoystickManager() {}
    virtual void onJoystickAdded(int which)=0;
    virtual void onJoystickRemoved(int which)=0;
    virtual float axisPosition(int which, int axis)   =0;
    virtual bool buttonPressed(int which, int button) =0;
    virtual bool buttonReleased(int which, int button)=0;
    virtual std::string name(int which)=0;
    virtual int maxButtons(int which)=0;
    virtual int maxAxes(int which)=0;
    virtual void shutdown()=0;
    virtual void motorOn(int which)=0;
    virtual void motorOff(int which)=0;
};

#endif // IJOYSTICKMANAGER_HPP
