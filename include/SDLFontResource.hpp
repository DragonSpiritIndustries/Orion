#ifndef SDLFONTRESOURCE_HPP
#define SDLFONTRESOURCE_HPP

#include <IFontResource.hpp>
#include <SDL2/SDL_ttf.h>
#include <unordered_map>

class SDLFontResource : public IFontResource
{
public:
    SDLFontResource(const std::string& path);
    ~SDLFontResource();
    Vector2i measureString(const std::string &str, int size=16);
    int glyphAdvance(atUint16 ch, int size=16);
    void draw(float x, float y, const std::string& text, int size=16, const Colorb& color=Colorb::white);
    static IResource* loadFont(const std::string& path);
private:
    void cacheFont(int size);
    std::unordered_map<int, TTF_Font*> m_cachedFonts;
    Uint32 m_fontLength;
    void* m_fontData;
};

#endif // SDLFONTRESOURCE_HPP
