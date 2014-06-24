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
    virtual void draw(IApplication *app, Vector2f position);
    virtual void draw(IApplication* app, int x, int y, Rectanglef subrect, Vector2f origin, bool flipH, bool flipV, float angle);
    virtual void draw(IApplication* app, Vector2f position, Rectanglef subrect, Vector2f origin, bool flipH, bool flipV, float angle);
    Vector2i size() const;
    int width()     const;
    int height()    const;
    static IResource* loadTexture(const std::string& path);

private:
    SDL_Texture* m_texture;
    Vector2i     m_size;
};

#endif // SDLTEXTURE_HPP
