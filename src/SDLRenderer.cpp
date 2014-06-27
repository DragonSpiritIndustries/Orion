#include "SDLRenderer.hpp"
#include "SDLWindow.hpp"
#include "Console.hpp"
#include <SDL2/SDL_render.h>
#include "SDL2/SDL_opengl.h"
#include <limits.h>

float toFloat(int val)
{
    return (float)(val*(1.f/255.f));
}

SDLRenderer::SDLRenderer()
    : m_context(nullptr)
{
}

SDLRenderer::~SDLRenderer()
{
    SDL_GL_DeleteContext(m_context);
}

void SDLRenderer::setClearColor(const Colorf& color)
{
    m_clearColor = color;
    glClearColor(m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
}

void SDLRenderer::clear()
{
    //Clear color buffer
    glClear(GL_COLOR_BUFFER_BIT);
}

void SDLRenderer::present()
{
    SDL_GL_SwapWindow(m_windowHandle);
}

void SDLRenderer::drawRect(int w, int h, int x, int y, bool fill, Colorf col)
{
    static SDL_Rect rect;
    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;

    SDL_SetRenderDrawColor(m_renderer, (int)(col.r*255), (int)(col.g*255), (int)(col.b*255), (int)(col.a*255));
    if (fill)
        SDL_RenderFillRect(m_renderer, &rect);
    else
        SDL_RenderDrawRect(m_renderer, &rect);
}

void* SDLRenderer::handle()
{
    return reinterpret_cast<void*>(m_renderer);
}

bool SDLRenderer::initialize(IWindow& window)
{
    m_windowHandle =reinterpret_cast<SDL_Window*>(window.handle());
    m_context = SDL_GL_CreateContext(m_windowHandle);
    if (m_context == nullptr || m_renderer)
    {
        orConsoleRef.print(orConsoleRef.Fatal, "%s", SDL_GetError());
        return false;
    }
    m_renderer = SDL_CreateRenderer(m_windowHandle, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer)
    {
        orConsoleRef.print(orConsoleRef.Fatal, "%s", SDL_GetError());
        return false;
    }
    //Use Vsync
    if( SDL_GL_SetSwapInterval( 1 ) < 0 )
    {
        orConsoleRef.print(orConsoleRef.Warning, "Unable to set VSync! SDL Error: %s\n", SDL_GetError() );
    }
    return true;
}

