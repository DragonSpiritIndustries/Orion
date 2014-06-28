#include "SDLTexture.hpp"
#include "physfsrwops.h"
#include <SDL2/SDL_image.h>
#include <ApplicationBase.hpp>
#include <SDL2/SDL_opengl.h>
#include <GL/gl.h>

SDLTextureResource::SDLTextureResource(const std::string& path, Uint32 width, Uint32 height,  Uint32 data)
    : ITextureResource(path),
      m_texture(data)
{
    m_size = Vector2i(width, height);
}

SDLTextureResource::~SDLTextureResource()
{
    glDeleteTextures(1, &m_texture);
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
    glPushMatrix();
    glTranslatef(rect.x, rect.y, 0.0f);
    glRotatef(angle, 0.0f, 0.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBegin(GL_QUADS);
    {
        glTexCoord2f(subrect.x, subrect.y);
        glVertex2f(0 - origin.x , 0 - origin.y);
        glTexCoord2f(subrect.x, subrect.y + subrect.h);
        glVertex2f(0 - origin.y, rect.h - origin.y);
        glTexCoord2f(subrect.x + subrect.w, subrect.y + subrect.h);
        glVertex2f(rect.w - origin.y, rect.h - origin.y);
        glTexCoord2f(subrect.x + subrect.w, subrect.y);
        glVertex2f(rect.w - origin.y, 0 - origin.y);
    }
    glEnd();
    glPopMatrix();
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
    if (PHYSFS_exists(path.c_str()))
    {
        SDL_Surface* imgSurf = IMG_Load_RW(PHYSFSRWOPS_openRead(path.c_str()), 1);
        if (imgSurf)
        {
            Uint32 mode;
            // work out what format to tell glTexImage2D to use...
            orConsoleRef.print(orConsoleRef.Info, "%i", imgSurf->format->BytesPerPixel);
            if (imgSurf->format->BytesPerPixel == 3)
                mode = GL_RGB;
            else if (imgSurf->format->BytesPerPixel == 4)
                mode = GL_RGBA;
            else
            {
                SDL_FreeSurface(imgSurf);
                return nullptr;
            }

            Uint32 width = imgSurf->w;
            Uint32 height = imgSurf->h;
            Uint32 textureId;
            // create one texture name
            glGenTextures(1, &textureId);

            // tell opengl to use the generated texture name
            glBindTexture(GL_TEXTURE_2D, textureId);

            // this reads from the sdl surface and puts it into an opengl texture
            glTexImage2D(GL_TEXTURE_2D, 0, mode, imgSurf->w, imgSurf->h, 0, mode, GL_UNSIGNED_BYTE, imgSurf->pixels);

            SDL_FreeSurface(imgSurf);
            return new SDLTextureResource(path, width, height, textureId);
        }
    }

    return nullptr;
}

void SDLTextureResource::setWrapH(bool val)
{
    m_wrapH = val;
}

void SDLTextureResource::setWrapV(bool val)
{
    m_wrapV = val;
}

