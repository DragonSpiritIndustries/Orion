#include "SDLRenderer.hpp"
#include <SDL2/SDL_render.h>

SDLRenderer::SDLRenderer()
    : m_renderer(nullptr)
{
}

SDLRenderer::~SDLRenderer()
{
    SDL_DestroyRenderer(m_renderer);
}

void SDLRenderer::clear()
{
    SDL_RenderClear(m_renderer);
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

    SDL_RenderDrawRect(m_renderer, &rect);
}

bool SDLRenderer::initialize(SDLWindow& window)
{
    m_renderer = SDL_CreateRenderer(window.handle(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (m_renderer == nullptr)
    {
        orDebug(SDL_GetError());
        return false;
    }

    return true;
}

