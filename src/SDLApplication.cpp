#include "SDLApplication.hpp"
#include "SDLKeyboardManager.hpp"
#include "SDLJoystickManager.hpp"
#include "ObjectManager.hpp"
#include "Global.hpp"
#include <SDL2/SDL.h>
#include <iostream>
#include <Athena/Utility.hpp>

SDLApplication::SDLApplication()
    : m_running(false)
{
}

SDLApplication::~SDLApplication()
{
}

int SDLApplication::exec()
{
    while (m_running)
    {
        pollEvents();
        onUpdate();
        onDraw();
        updateFPS();
    }

    onExit();
    return 0;
}

bool SDLApplication::init(int argc, char* argv[])
{
    memset(m_frameValues, 0, sizeof(m_frameValues));
    m_frameCount = 0;
    m_fps = 0.f;
    m_lastFrame = SDL_GetTicks();
    orDebug("Orion " orVERSION_STR " " orRELEASE_NAME " SDL Application\n");

    parseCommandLine(argc, argv);
    int code= 0;
    if ((code = SDL_Init(SDL_INIT_EVERYTHING)) < 0)
    {
        orDebug("SDL Failed to initalize: %s(%i)\n", SDL_GetError(), code);
        return false;
    }

    if (!m_window.initialize())
        return false;

    if (!m_renderer.initialize(m_window))
        return false;

    m_keyboardManager = std::shared_ptr<IKeyboardManager>(new SDLKeyboardManager);
    m_joystickManager = std::shared_ptr<IJoystickManager>(new SDLJoystickManager);
    m_running = true;
    return true;
}

void SDLApplication::close()
{
    m_running = false;
}

void SDLApplication::onUpdate()
{
    m_updateSignal(m_frameTime);
}

void SDLApplication::pollEvents()
{
    SDL_Event sdlEvent;
    // TODO: Create Key,Mouse, and Joystick mapping enumerations
    // TODO: Add a more complete event list
    while (SDL_PollEvent(&sdlEvent))
    {
        Event oEvent;
        if (sdlEvent.type == SDL_QUIT)
        {
            oEvent.type = Event::EV_QUIT;
            m_eventSignal(oEvent);
            m_running = false;
        }
        else if (sdlEvent.type == SDL_KEYDOWN || sdlEvent.type == SDL_KEYUP)
        {
            oEvent.type = (sdlEvent.type == SDL_KEYDOWN ? Event::EV_KEY_PRESSED
                                                             : Event::EV_KEY_RELEASED);
            oEvent.eventData.keyboardEvent.pressed  = (oEvent.type == Event::EV_KEY_PRESSED);
            oEvent.eventData.keyboardEvent.keyCode  = sdlEvent.key.keysym.sym;
            oEvent.eventData.keyboardEvent.scanCode = sdlEvent.key.keysym.scancode;
            oEvent.eventData.keyboardEvent.modifier = sdlEvent.key.keysym.mod;
            m_eventSignal(oEvent);
            m_keyboardSignal(oEvent);
        }
        // TODO: Should text editing be handled separately? could be useful for a GUI implementation
        else if (sdlEvent.type == SDL_TEXTINPUT || sdlEvent.type == SDL_TEXTEDITING)
        {
            oEvent.type = Event::EV_TEXT_ENTERED;
            oEvent.eventData.textEvent.string = sdlEvent.text.text;
            m_textSignal(oEvent);
        }
        else if (sdlEvent.type == SDL_JOYDEVICEADDED)
        {
            m_joystickAddedSignal(sdlEvent.cdevice.which);
        }
        else if (sdlEvent.type == SDL_JOYDEVICEREMOVED)
        {
            m_joystickRemovedSignal(sdlEvent.cdevice.which);
        }
        else if (sdlEvent.type == SDL_JOYAXISMOTION)
        {
            oEvent.type = Event::EV_JOY_AXIS;
            oEvent.eventData.joystickEvent.id = sdlEvent.caxis.which;
            oEvent.eventData.joystickEvent.axis = sdlEvent.caxis.axis;
            oEvent.eventData.joystickEvent.position = sdlEvent.caxis.value/32768.f;
            m_eventSignal(oEvent);
            m_joystickSignal(oEvent);
        }
        else if (sdlEvent.type == SDL_JOYBUTTONDOWN || sdlEvent.type == SDL_JOYBUTTONUP)
        {
            oEvent.type = (sdlEvent.type == SDL_JOYBUTTONDOWN ? Event::EV_JOY_BTN_PRESSED
                                                                 : Event::EV_JOY_BTN_RELEASED);
            oEvent.eventData.joystickEvent.id = sdlEvent.cbutton.which;
            oEvent.eventData.joystickEvent.button = sdlEvent.cbutton.button;
            m_eventSignal(oEvent);
            m_joystickSignal(oEvent);
        }
        else if (sdlEvent.type == SDL_MOUSEBUTTONDOWN || sdlEvent.type == SDL_MOUSEBUTTONUP)
        {
            oEvent.type = (sdlEvent.type == SDL_MOUSEBUTTONDOWN ? Event::EV_MOUSE_BTN_PRESSED
                                                             : Event::EV_MOUSE_BTN_RELEASED);
            oEvent.eventData.mouseButtonEvent.button = sdlEvent.button.button;
            oEvent.eventData.mouseButtonEvent.x      = sdlEvent.button.x;
            oEvent.eventData.mouseButtonEvent.y      = sdlEvent.button.y;
            m_eventSignal(oEvent);
            m_mouseButtonSignal(oEvent);
        }
        else if (sdlEvent.type == SDL_MOUSEWHEEL)
        {
            oEvent.type = Event::EV_MOUSE_WHEEL;
            int x,y;
            SDL_GetMouseState(&x, &y);
            oEvent.eventData.mouseWheelEvent.x = x;
            oEvent.eventData.mouseWheelEvent.y = y;
            oEvent.eventData.mouseWheelEvent.vDelta = sdlEvent.wheel.y;
            oEvent.eventData.mouseWheelEvent.hDelta = sdlEvent.wheel.x;
            m_eventSignal(oEvent);
            m_mouseWheelSignal(oEvent);
        }
        else if (sdlEvent.type == SDL_MOUSEMOTION)
        {
            oEvent.type = Event::EV_MOUSE_MOVED;
            oEvent.eventData.mouseMoveEvent.x = sdlEvent.motion.x;
            oEvent.eventData.mouseMoveEvent.y = sdlEvent.motion.y;
            m_eventSignal(oEvent);
            m_mouseMoveSignal(oEvent);
        }
        else if (sdlEvent.type == SDL_WINDOWEVENT)
        {
            switch (sdlEvent.window.event)
            {
                case SDL_WINDOWEVENT_RESIZED:
                {
                    oEvent.type = Event::EV_WINDOW_RESIZE;
                    oEvent.eventData.resizeEvent.width = sdlEvent.window.data1;
                    oEvent.eventData.resizeEvent.height = sdlEvent.window.data2;
                    m_eventSignal(oEvent);
                    m_resizeSignal(oEvent);
                }
                    break;
                case SDL_WINDOWEVENT_FOCUS_GAINED:
                {
                    oEvent.type = Event::EV_FOCUS_GAINED;
                    oEvent.eventData.focusEvent.focus = true;
                    m_eventSignal(oEvent);
                    m_focusSignal(oEvent);
                }
                    break;
                case SDL_WINDOWEVENT_FOCUS_LOST:
                {
                    oEvent.type = Event::EV_FOCUS_LOST;
                    oEvent.eventData.focusEvent.focus = false;
                    m_eventSignal(oEvent);
                    m_focusSignal(oEvent);
                }
                    break;
            }
        }
    }
}

void SDLApplication::updateFPS()
{
    unsigned frameIndex = m_frameCount % MaxFrameValues;

    unsigned ticks = SDL_GetTicks();

    m_frameValues[frameIndex] = ticks - m_lastFrame;
    m_frameTime = (m_frameValues[frameIndex]) / 1000.f;
    m_lastFrame = ticks;

    m_frameCount++;

    unsigned count = 0;
    if (m_frameCount < MaxFrameValues)
        count = m_frameCount;
    else
        count = MaxFrameValues;

    // Should never happen buuut.
    if (count == 0)
        count = 1;

    m_fps = 0;
    for (int i = 0; i < count; i++)
        m_fps += m_frameValues[i];

    m_fps /= count;
    m_fps  = 1000.f / m_fps;
}

void SDLApplication::onDraw()
{
    m_renderer.clear();
    m_drawSignal(this);
    m_renderer.present();
}

void SDLApplication::onExit()
{
    orObjectManagerRef.shutdown();
    m_joystickManager.get()->shutdown();
    m_keyboardManager.get()->shutdown();
    SDL_Quit();
}

void SDLApplication::drawRectangle(int w, int h, int x, int y, bool fill)
{
    m_renderer.drawRect(w, h, x, y, fill);
}

void SDLApplication::setTitle(const std::string& title)
{
    m_window.setTitle(title);
}

void SDLApplication::setClearColor(const Colorb& color)
{
    m_renderer.setClearColor(color);
}

float SDLApplication::fps() const
{
    return m_fps;
}
