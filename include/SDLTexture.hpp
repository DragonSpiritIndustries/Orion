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
    virtual void draw(const Vector2f& position);
    virtual void draw(float x, float y, const Rectanglef& subrect, const Vector2f& origin, bool flipH, bool flipV, float angle);
    virtual void draw(const Rectanglef& rect, const Rectanglef& subrect, const Vector2f& origin, bool flipH, bool flipV, float angle);
    Vector2i size() const;
    int width()     const;
    int height()    const;
    static IResource* loadTexture(const std::string& path);

    void setWrapH(bool val);
    void setWrapV(bool val);
private:
    SDL_Texture* m_texture;
};

#endif // SDLTEXTURE_HPP
