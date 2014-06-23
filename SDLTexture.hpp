#ifndef SDLTEXTURE_HPP
#define SDLTEXTURE_HPP

#include <ITextureResource.hpp>
#include <SDL2/SDL.h>

class SDLTextureResource : public ITextureResource
{
    DEFINE_RESOURCE();
public:
    SDLTextureResource(const std::string& path, SDL_Texture* data);
    ~SDLTextureResource();

    virtual void draw(IApplication* app, int x, int y);
    virtual void drawPart(IApplication* app, int x, int y, Rectanglef rectangle);
    Vector2i size() const;
    int width()     const;
    int height()    const;
    static IResource* loadTexture(const std::string& path);

private:
    SDL_Texture* m_texture;
    Vector2i     m_size;
};

#endif // SDLTEXTURE_HPP
