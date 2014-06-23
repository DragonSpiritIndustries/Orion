#include "SDLMouseManager.hpp"
#include "IApplication.hpp"
#include "SDL2/SDL.h"

SDLMouseManager::SDLMouseManager()
{
    orDebug("MouseManager INITIALIZED\n");
    orApplicationRef.mouseButtonSignal().connect<SDLMouseManager, &SDLMouseManager::onMouseButton>(this);
    orApplicationRef.mouseWheelSignal().connect<SDLMouseManager, &SDLMouseManager::onMouseWheel>(this);
}

void SDLMouseManager::shutdown()
{
    orDebug("MouseManager SHUTDOWN\n");
    orApplicationRef.mouseButtonSignal().disconnect<SDLMouseManager, &SDLMouseManager::onMouseButton>(this);
    orApplicationRef.mouseWheelSignal().connect<SDLMouseManager, &SDLMouseManager::onMouseWheel>(this);
}

bool SDLMouseManager::buttonPressed(MouseButton button)
{
    return m_pressed[button];
}

bool SDLMouseManager::buttonReleased(MouseButton button)
{
    return m_released[button];
}

Vector2i SDLMouseManager::position()
{
    int x,y;
    SDL_GetMouseState(&x, &y);

    return Vector2i(x, y);
}

Vector2i SDLMouseManager::wheelDelta()
{
    return m_mouseWheelDelta;
}

void SDLMouseManager::setMousePosition(int x, int y)
{
    // No need to manually set m_mousePosition
    // SDL_WarpMouseInWindow generates a MouseMove event
    SDL_WarpMouseInWindow(nullptr, x, y);
}

void SDLMouseManager::setMousePosition(Vector2i position)
{
    setMousePosition(position.x, position.y);
}

void SDLMouseManager::onMouseButton(Event e)
{
    MouseButton button;

    switch (e.eventData.mouseButtonEvent.button)
    {
        case SDL_BUTTON_LEFT:
            button = MouseButton::Left;
            break;
        case SDL_BUTTON_MIDDLE:
            button = MouseButton::Middle;
            break;
        case SDL_BUTTON_RIGHT:
            button = MouseButton::Right;
            break;
        case SDL_BUTTON_X1:
            button = MouseButton::X1;
            break;
        case SDL_BUTTON_X2:
            button = MouseButton::X2;
            break;
    }

    if (e.type == Event::EV_MOUSE_BTN_PRESSED)
    {
        m_pressed[button] = true;
        m_released[button] = false;
    }
    else
    {
        m_pressed[button] = false;
        m_released[button] = true;
    }
}

void SDLMouseManager::onMouseWheel(Event e)
{
    m_mouseWheelDelta.x = e.eventData.mouseWheelEvent.hDelta;
    m_mouseWheelDelta.y = e.eventData.mouseWheelEvent.vDelta;
}
