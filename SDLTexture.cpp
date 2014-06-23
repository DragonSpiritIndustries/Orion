#include "SDLTexture.hpp"
#include "physfsrwops.h"
#include <SDL2/SDL_image.h>
#include <IApplication.hpp>

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

void SDLTextureResource::draw(IApplication* app, int x, int y)
{
    static SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = m_size.x;
    rect.h = m_size.y;

    SDL_RenderCopy((SDL_Renderer*)app->rendererHandle(), m_texture, NULL, &rect);
}

void SDLTextureResource::drawPart(IApplication* app, int x, int y, Rectanglef rectangle)
{

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


REGISTER_RESOURCE(SDLTextureResource, "TextureResource", loadTexture);
