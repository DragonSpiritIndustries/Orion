#include "SDLKeyboardManager.hpp"
#include "IApplication.hpp"
#include "SDL2/SDL_scancode.h"

SDLKeyboardManager::SDLKeyboardManager()
{
    orDebug("KEYBOARDMANAGER: Intializing\n");
    orDebug("KEYBOARDMANAGER: Connecting vital signals\n");
    orApplicationPtr->keyboardSignal().connect<SDLKeyboardManager, &SDLKeyboardManager::translateEvent>(this);
    orApplicationPtr->updateSignal().connect<IKeyboardManager, &IKeyboardManager::onUpdate>(this);
    orDebug("KEYBOARDMANAGER: Initialized\n");
}

SDLKeyboardManager::~SDLKeyboardManager()
{
}

bool SDLKeyboardManager::keyPressed(Key key)
{
    return m_pressedKeys[key];
}

bool SDLKeyboardManager::keyReleased(Key key)
{
    return m_releasedKeys[key];
}

Key SDLKeyboardManager::fromScanCode(int scan)
{
    switch(scan)
    {
        case SDL_SCANCODE_A:      return Key::A;
        case SDL_SCANCODE_B:      return Key::B;
        case SDL_SCANCODE_C:      return Key::C;
        case SDL_SCANCODE_D:      return Key::D;
        case SDL_SCANCODE_E:      return Key::E;
        case SDL_SCANCODE_F:      return Key::F;
        case SDL_SCANCODE_G:      return Key::G;
        case SDL_SCANCODE_H:      return Key::H;
        case SDL_SCANCODE_I:      return Key::I;
        case SDL_SCANCODE_J:      return Key::J;
        case SDL_SCANCODE_K:      return Key::K;
        case SDL_SCANCODE_L:      return Key::L;
        case SDL_SCANCODE_M:      return Key::M;
        case SDL_SCANCODE_N:      return Key::N;
        case SDL_SCANCODE_O:      return Key::O;
        case SDL_SCANCODE_P:      return Key::P;
        case SDL_SCANCODE_Q:      return Key::Q;
        case SDL_SCANCODE_R:      return Key::R;
        case SDL_SCANCODE_S:      return Key::S;
        case SDL_SCANCODE_T:      return Key::T;
        case SDL_SCANCODE_U:      return Key::U;
        case SDL_SCANCODE_V:      return Key::V;
        case SDL_SCANCODE_W:      return Key::W;
        case SDL_SCANCODE_X:      return Key::X;
        case SDL_SCANCODE_Y:      return Key::Y;
        case SDL_SCANCODE_Z:      return Key::Z;
        case SDL_SCANCODE_1:      return Key::NUM1;
        case SDL_SCANCODE_2:      return Key::NUM2;
        case SDL_SCANCODE_3:      return Key::NUM3;
        case SDL_SCANCODE_4:      return Key::NUM4;
        case SDL_SCANCODE_5:      return Key::NUM5;
        case SDL_SCANCODE_6:      return Key::NUM6;
        case SDL_SCANCODE_7:      return Key::NUM7;
        case SDL_SCANCODE_8:      return Key::NUM8;
        case SDL_SCANCODE_9:      return Key::NUM9;
        case SDL_SCANCODE_0:      return Key::NUM0;
        case SDL_SCANCODE_ESCAPE: return Key::ESCAPE;
        case SDL_SCANCODE_LCTRL:  return Key::LCONTROL;
        case SDL_SCANCODE_LSHIFT: return Key::LSHIFT;
        case SDL_SCANCODE_LALT:   return Key::LALT;
        case SDL_SCANCODE_LGUI:   return Key::LSYSTEM;
        case SDL_SCANCODE_RCTRL:  return Key::RCONTROL;
        case SDL_SCANCODE_RSHIFT: return Key::RSHIFT;
        case SDL_SCANCODE_RALT:   return Key::RALT;
        case SDL_SCANCODE_RGUI:   return Key::RSYSTEM;
        case SDL_SCANCODE_RETURN: return Key::RETURN;
        default:
            return Key::UNKNOWN;
    }
}

void SDLKeyboardManager::shutdown()
{
    orApplicationPtr->keyboardSignal().disconnect<SDLKeyboardManager, &SDLKeyboardManager::translateEvent>(this);
    orApplicationPtr->updateSignal().disconnect<IKeyboardManager, &IKeyboardManager::onUpdate>(this);
    orDebug("KEYBOARDMANAGER: Shutdown\n");
}

void SDLKeyboardManager::translateEvent(Event keyEvent)
{
    Key key = fromScanCode(keyEvent.eventData.keyboardEvent.scanCode);

    if (keyEvent.type == Event::EV_KEY_PRESSED)
        m_pressedKeys[key] = true;
    else if (keyEvent.type == Event::EV_KEY_RELEASED)
    {
        m_pressedKeys[key] = false;
        m_releasedKeys[key] = true;
    }
}
