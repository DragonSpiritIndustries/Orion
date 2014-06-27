#ifndef SDLRENDERER_HPP
#define SDLRENDERER_HPP

#include "IRenderer.hpp"
#include "SDLWindow.hpp"
#include "SDL2/SDL.h"

class SDLRenderer : public IRenderer
{
public:
    SDLRenderer();
    ~SDLRenderer();

    void setClearColor(const Colorf& color = Colorf::black);
    bool initialize(IWindow& window);
    void clear();
    void present();
    void drawRect(int w, int h, int x, int y, bool fill = false, Colorb col=Colorb::white);
    void* handle();
private:
    SDL_Renderer*  m_renderer;
    SDL_GLContext  m_context;
    SDL_Window*    m_windowHandle;
};

#endif // SDLRENDERER_HPP
