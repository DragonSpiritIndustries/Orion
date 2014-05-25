#include "SDLApplication.hpp"
#include "Global.hpp"
#include <SDL2/SDL.h>

SDLApplication::SDLApplication()
    : m_running(false)
{
}

SDLApplication::~SDLApplication()
{
    onExit();
}

int SDLApplication::exec()
{
    while (m_running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
            onEvent(&event);

        onUpdate();
        onDraw();
    }

    onExit();
    return 0;
}

bool SDLApplication::init(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        return false;


    if (!m_window.initialize())
        return false;

    if (!m_renderer.initialize(m_window))
        return false;

    m_running = true;
    return true;
}

void SDLApplication::onEvent(SDL_Event* event)
{
    if (event->type == SDL_QUIT)
        m_running = false;
}

void SDLApplication::onUpdate()
{
    m_updateSignal(this);
}

void SDLApplication::onDraw()
{
    m_renderer.clear();
    m_drawSignal(this);
    m_renderer.present();
}

void SDLApplication::onExit()
{
    //m_resourceManager.shutdown();
    //m_console.shutdown();;
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

Nano::Signal<void (IApplication*)>& SDLApplication::drawSignal()
{
    return m_drawSignal;
}

Nano::Signal<void (IApplication*)>&SDLApplication::updateSignal()
{
    return m_updateSignal;
}
