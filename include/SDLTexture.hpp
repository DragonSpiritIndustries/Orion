#ifndef SDLTEXTURE_HPP
#define SDLTEXTURE_HPP

#include <ITextureResource.hpp>
#include <SDL2/SDL.h>

class SDLTextureResource : public ITextureResource
{
public:
    SDLTextureResource(const std::string& path, SDL_Texture* data);
    ~SDLTextureResource();

    virtual void draw(float x, float y);
    virtual void draw(Vector2f position);
    virtual void draw(float x, float y, Rectanglef subrect, Vector2f origin, bool flipH, bool flipV, float angle);
    virtual void draw(Vector2f position, Rectanglef subrect, Vector2f origin, bool flipH, bool flipV, float angle);
    Vector2i size() const;
    int width()     const;
    int height()    const;
    static IResource* loadTexture(const std::string& path);

private:
    SDL_Texture* m_texture;
    Vector2i     m_size;
};

#endif // SDLTEXTURE_HPP
