#include "SDLRenderer.hpp"
#include "SDLWindow.hpp"
#include <SDL2/SDL_render.h>

SDLRenderer::SDLRenderer()
    : m_renderer(nullptr)
{
}

SDLRenderer::~SDLRenderer()
{
    SDL_DestroyRenderer(m_renderer);
}

void SDLRenderer::setClearColor(const Colorb& color)
{
    m_clearColor = color;
}

void SDLRenderer::clear()
{
    // clear to our color
    SDL_SetRenderDrawColor(m_renderer, m_clearColor.r, m_clearColor.g, m_clearColor.b, m_clearColor.a);
    SDL_RenderClear(m_renderer);
    // Set back to black
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
}

void SDLRenderer::present()
{
    SDL_RenderPresent(m_renderer);
}

void SDLRenderer::drawRect(int w, int h, int x, int y, bool fill)
{
    static SDL_Rect rect;
    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;

    if (fill)
        SDL_RenderFillRect(m_renderer, &rect);
    else
        SDL_RenderDrawRect(m_renderer, &rect);
}

void* SDLRenderer::handle()
{
    return (void*)m_renderer;
}

bool SDLRenderer::initialize(IWindow& window)
{
    m_renderer = SDL_CreateRenderer(reinterpret_cast<SDL_Window*>(window.handle()), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (m_renderer == nullptr)
    {
        orDebug("%s\n", SDL_GetError());
        return false;
    }

    return true;
}

