#include "SDLWindow.hpp"
#include "Console.hpp"
#include <SDL2/SDL.h>

SDLWindow::SDLWindow()
    : m_window(nullptr)
{
}

SDLWindow::~SDLWindow()
{
    SDL_DestroyWindow(m_window);
}

bool SDLWindow::initialize()
{
    m_window = SDL_CreateWindow("UnnamedOrionApplication", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

    if (m_window == nullptr)
    {
        orConsoleRef.print(orConsoleRef.Fatal, SDL_GetError());
        return false;
    }

    return true;
}

void SDLWindow::setTitle(const std::string& title)
{
    SDL_SetWindowTitle(m_window, title.c_str());
}

std::string SDLWindow::title() const
{
    return std::string(SDL_GetWindowTitle(m_window));
}

void* SDLWindow::handle() const
{
    return (void*)m_window;
}

Vector2i SDLWindow::windowSize()
{
    int w, h;
    SDL_GetWindowSize(m_window, &w, &h);

    return Vector2i(w, h);
}

int SDLWindow::windowWidth()
{
    return windowSize().x;
}

int SDLWindow::windowHeight()
{
    return windowSize().y;
}
