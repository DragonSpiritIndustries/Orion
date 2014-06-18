#ifndef SDLRENDERER_HPP
#define SDLRENDERER_HPP

#include "IRenderer.hpp"
#include "SDLWindow.hpp"
struct SDL_Renderer;

class SDLRenderer : public IRenderer<SDLWindow>
{
public:
    SDLRenderer();
    ~SDLRenderer();

    void setClearColor(const Colorb& color = Colorb::black);
    bool initialize(SDLWindow& window);
    void clear();
    void present();
    void drawRect(int w, int h, int x, int y, bool fill = false);
private:
    SDL_Renderer* m_renderer;
    Colorb         m_clearColor;
};

#endif // SDLRENDERER_HPP
