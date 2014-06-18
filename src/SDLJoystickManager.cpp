#include "SDLJoystickManager.hpp"
#include "IApplication.hpp"
#include <algorithm>

SDLJoystickManager::SDLJoystickManager()
{
    orApplicationPtr->joystickAddedSignal().connect<SDLJoystickManager, &SDLJoystickManager::onJoystickAdded>(this);
    orApplicationPtr->joystickRemovedSignal().connect<SDLJoystickManager, &SDLJoystickManager::onJoystickRemoved>(this);
    orApplicationPtr->joystickSignal().connect<SDLJoystickManager, &SDLJoystickManager::onButtonReleased>(this);
    orApplicationPtr->updateSignal().connect<SDLJoystickManager, &SDLJoystickManager::update>(this);
}

SDLJoystickManager::~SDLJoystickManager()
{
}

void SDLJoystickManager::onJoystickAdded(int which)
{
    SDLJoystickDevice* device = new SDLJoystickDevice;
    device->device = SDL_JoystickOpen(which);
    if (!device->device)
    {
        delete device;
        orDebug("Error obtaining controller %i\n", which);
    }
    else
    {
        orDebug("Obtained controller:\n");
        device->name = std::string(SDL_JoystickNameForIndex(which));
        orDebug("Name: %s\n", device->name.c_str());
        device->id = SDL_JoystickInstanceID(device->device);
        orDebug("Device slot: %i\n", device->id);
        device->axisCount = SDL_JoystickNumAxes(device->device);
        orDebug("Axis count: %i\n", device->axisCount);
        device->buttonCount = SDL_JoystickNumButtons(device->device);
        orDebug("Button Count: %i\n", device->buttonCount);
        device->forceFeedback = SDL_HapticOpenFromJoystick(device->device);
        device->hasRumble = (device->forceFeedback != nullptr);
        orDebug("Force feedback: %s\n", (device->hasRumble ? "yes" : "no"));
        device->guid = SDL_JoystickGetGUID(device->device);
        char guidString[33];
        SDL_JoystickGetGUIDString(device->guid, guidString, sizeof(guidString));
        orDebug("GUID: %s\n", guidString);
        device->currentEffect = -1;
        m_devices.push_back(device);
    }
}

void SDLJoystickManager::onJoystickRemoved(int which)
{
    SDLJoystickDevice* device = joystick(which);
    if (!device)
        return;

    std::vector<SDLJoystickDevice*>::iterator iter = std::find(m_devices.begin(), m_devices.end(), device);

    if (device->hasRumble)
        SDL_HapticClose(device->forceFeedback);

    SDL_JoystickClose(device->device);

    orDebug("Controller removed: %s\n", device->name.c_str());
    m_devices.erase(iter);
    delete device;
    device = nullptr;
}

float SDLJoystickManager::axisPosition(int which, int axis)
{
    if (which < 0 || which >= (int)m_devices.size())
        return 0.0f;

    // SDL Stores the axis position as a signed int, we need a float value
    // fortunately it gets stored the the correct range for a simple division
    // Normally you should divide by 32768.f in order to get the correct value, however it seems to cap it
    // to between -0.9 and +0.9
    SDLJoystickDevice* device = m_devices[which];
    return SDL_JoystickGetAxis(device->device, axis)/32767.0f;
}

bool SDLJoystickManager::buttonPressed(int which, int button)
{
    if (which < 0 || which >= m_devices.size())
        return false;

    return SDL_JoystickGetButton(m_devices[which]->device, button);
}

bool SDLJoystickManager::buttonReleased(int which, int button)
{
    if (which < 0 || which >= m_devices.size())
        return false;
    SDLJoystickDevice* device = m_devices[which];
    return m_releasedButtons[device->id][button];
}

std::string SDLJoystickManager::name(int which)
{
    if (which < 0 || which >= m_devices.size())
        return std::string();

    return m_devices[which]->name;
}

int SDLJoystickManager::maxButtons(int which)
{
    if (which < 0 || which >= m_devices.size())
        return 0;

    return m_devices[which]->buttonCount;
}

int SDLJoystickManager::maxAxes(int which)
{
    if (!joystick(which))
        return 0;

    return m_devices[which]->buttonCount;
}

void SDLJoystickManager::motorOn(int which)
{
    if (which < 0 || which >= m_devices.size())
        return;

    SDLJoystickDevice* device = m_devices[which];

    if (device->hasRumble && device->currentEffect == -1)
    {
        SDL_HapticEffect effect;
        memset(&effect, 0, sizeof(SDL_HapticConstant));
        effect.type = SDL_HAPTIC_SINE;
        effect.periodic.direction.type = SDL_HAPTIC_POLAR; // Polar coordinates
        effect.periodic.direction.dir[0] = 18000; // Force comes from south
        effect.periodic.period = 1000*10; // 1000 ms
        effect.periodic.magnitude = 20000; // 20000/32767 strength
        effect.periodic.length = (unsigned)-1; // 5 seconds long
        effect.periodic.attack_length = 1000; // Takes 1 second to get max strength
        effect.periodic.fade_length = 1000; // Takes 1 second to fade away
        device->currentEffect = SDL_HapticNewEffect(device->forceFeedback, &effect);
        SDL_HapticRunEffect(device->forceFeedback, device->currentEffect, 1);
    }
}

void SDLJoystickManager::motorOff(int which)
{
    if (which < 0 || which >= m_devices.size())
        return;

    SDLJoystickDevice* device = m_devices[which];

    if (device->hasRumble && device->currentEffect != -1)
    {
        SDL_HapticDestroyEffect(device->forceFeedback, device->currentEffect);
        device->currentEffect = -1;
    }
}

void SDLJoystickManager::shutdown()
{
    orApplicationPtr->joystickAddedSignal().disconnect<SDLJoystickManager, &SDLJoystickManager::onJoystickAdded>(this);
    orApplicationPtr->joystickRemovedSignal().disconnect<SDLJoystickManager, &SDLJoystickManager::onJoystickRemoved>(this);
    orApplicationPtr->joystickSignal().disconnect<SDLJoystickManager, &SDLJoystickManager::onButtonReleased>(this);
    orApplicationPtr->updateSignal().disconnect<SDLJoystickManager, &SDLJoystickManager::update>(this);
    for (SDLJoystickDevice* device : m_devices)
    {
        if (device->hasRumble)
            SDL_HapticClose(device->forceFeedback);

        SDL_JoystickClose(device->device);
        delete device;
        device = nullptr;
    }
    m_devices.clear();
}

void SDLJoystickManager::update(float)
{
    // ensure the joysticks are getting updated properly
    SDL_JoystickUpdate();
}

SDLJoystickDevice* SDLJoystickManager::joystick(int which)
{
    std::vector<SDLJoystickDevice*>::iterator iter = std::find_if(m_devices.begin(), m_devices.end(),
                                                                  [&which](SDLJoystickDevice* d)->bool{ return d->id == which;});
    if (iter == m_devices.end())
        return nullptr;

    return *iter;
}

void SDLJoystickManager::onButtonReleased(Event joy)
{
    if (joy.type == Event::EV_JOY_BTN_RELEASED)
        m_releasedButtons[joy.eventData.joystickEvent.id][joy.eventData.joystickEvent.button] = true;
    else if (joy.type == Event::EV_JOY_BTN_PRESSED)
        m_releasedButtons[joy.eventData.joystickEvent.id][joy.eventData.joystickEvent.button] = false;
}
