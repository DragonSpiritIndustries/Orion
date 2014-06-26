#include "SDLTexture.hpp"
#include "physfsrwops.h"
#include <SDL2/SDL_image.h>
#include <ApplicationBase.hpp>

SDLTextureResource::SDLTextureResource(const std::string& path, SDL_Texture* data)
    : ITextureResource(path),
      m_texture(data)
{
    Uint32 format;
    int access, w, h;
    SDL_QueryTexture(m_texture, &format, &access, &w, &h);
    m_size = Vector2i(w, h);
}

SDLTextureResource::~SDLTextureResource()
{
    SDL_DestroyTexture(m_texture);
}

void SDLTextureResource::draw(float x, float y)
{
    static SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = m_size.x;
    rect.h = m_size.y;

    SDL_RenderCopy(reinterpret_cast<SDL_Renderer*>(orApplicationRef.rendererHandle()), m_texture, NULL, &rect);
}

void SDLTextureResource::draw(Vector2f position)
{
    draw(position.x, position.y);
}

void SDLTextureResource::draw(float x, float y, Rectanglef subrect, Vector2f origin, bool flipH, bool flipV, float angle)
{
    static SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = subrect.w;
    rect.h = subrect.h;

    static SDL_Rect partrect;
    partrect.x = subrect.x;
    partrect.y = subrect.y;
    partrect.w = subrect.w;
    partrect.h = subrect.h;

    static SDL_Point point;
    point.x = origin.x;
    point.y = origin.y;


    int flip = SDL_FLIP_NONE;
    if (flipH)
        flip |= SDL_FLIP_HORIZONTAL;
    if (flipV)
        flip |= SDL_FLIP_VERTICAL;

    SDL_RenderCopyEx(reinterpret_cast<SDL_Renderer*>(orApplicationRef.rendererHandle()), m_texture, &partrect, &rect, angle, &point, (SDL_RendererFlip)flip);

}

void SDLTextureResource::draw(Vector2f position, Rectanglef subrect, Vector2f origin, bool flipH, bool flipV, float angle)
{
    draw(position.x, position.y, subrect, origin, flipH, flipV, angle);
}

Vector2i SDLTextureResource::size() const
{
    return m_size;
}

int SDLTextureResource::width() const
{
    return m_size.x;
}

int SDLTextureResource::height() const
{
    return m_size.y;
}

IResource* SDLTextureResource::loadTexture(const std::string& path)
{
    SDL_Surface* imgSurf = IMG_Load_RW(PHYSFSRWOPS_openRead(path.c_str()), 1);
    if (imgSurf)
    {
        SDL_Texture* texture = SDL_CreateTextureFromSurface((SDL_Renderer*)orApplicationRef.rendererHandle(), imgSurf);
        SDL_FreeSurface(imgSurf);
        if(texture)
            return new SDLTextureResource(path, texture);
        orDebug("Failed to convert %s to texture\n", path.c_str());
    }

    return nullptr;
}


REGISTER_RESOURCE(SDLTextureResource, loadTexture);
