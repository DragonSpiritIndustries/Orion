#ifndef IJOYSTICKMANAGER_HPP
#define IJOYSTICKMANAGER_HPP

#include <string>
#include <unordered_map>
#include <vector>

#include "Config.hpp"

struct JoyButtonInfo
{
    int         id;
    std::string name;
    std::string image;
};

struct JoyMetaData
{
    std::string name;
    std::vector<JoyButtonInfo> buttonInfo;
};


class IJoystickManager
{
public:
    enum {MaxJoysticks = MAX_JOYSTICKS};
    IJoystickManager();
    virtual ~IJoystickManager() {}
    virtual void onUpdate(float);
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
    virtual bool isPluggedIn(int which)=0;
    bool hasMetaData(const std::string& joyName);
    void loadMetaData(const std::string& joyName);
protected:
    std::unordered_map<int, std::unordered_map<int, bool> > m_releasedButtons;
    std::vector<JoyMetaData>                                m_metaData;
};

#endif // IJOYSTICKMANAGER_HPP
