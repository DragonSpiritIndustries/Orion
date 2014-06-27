#include "SDLTexture.hpp"
#include "physfsrwops.h"
#include <SDL2/SDL_image.h>
#include <ApplicationBase.hpp>
#include <SDL2/SDL_opengl.h>

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
    draw(Rectanglef(x, y, width(), height()), Rectanglef(), Vector2f(), false, false, 0.0f);
}

void SDLTextureResource::draw(const Vector2f& position)
{
    draw(position.x, position.y);
}

void SDLTextureResource::draw(float x, float y, const Rectanglef& subrect, const Vector2f& origin, bool flipH, bool flipV, float angle)
{
    draw(Rectanglef(x, y, width(), height()), subrect, origin, flipH, flipV, angle);
}

void SDLTextureResource::draw(const Rectanglef& rect, const Rectanglef& subrect, const Vector2f& origin, bool flipH, bool flipV, float angle)
{
    static SDL_Rect locRect;
    locRect.x = rect.x;
    locRect.y = rect.y;
    locRect.w = rect.w;
    locRect.h = rect.h;

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

    SDL_RenderCopyEx(reinterpret_cast<SDL_Renderer*>(orApplicationRef.rendererHandle()), m_texture, &partrect, &locRect, angle, &point, (SDL_RendererFlip)flip);
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
        orConsoleRef.print(orConsoleRef.Info, "Failed to convert %s to texture\n", path.c_str());
    }

    return nullptr;
}

void SDLTextureResource::setWrapH(bool val)
{
    SDL_GL_BindTexture(m_texture, NULL, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (val ? GL_REPEAT : GL_CLAMP));
    SDL_GL_UnbindTexture(m_texture);
}

void SDLTextureResource::setWrapV(bool val)
{
    SDL_GL_BindTexture(m_texture, NULL, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (val ? GL_REPEAT : GL_CLAMP));
    SDL_GL_UnbindTexture(m_texture);
}

