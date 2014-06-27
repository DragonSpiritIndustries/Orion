#ifndef SDLAPPLICATION_HPP
#define SDLAPPLICATION_HPP

#include "ApplicationBase.hpp"
#include "SDLWindow.hpp"
#include "SDLRenderer.hpp"
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_ttf.h>

class SDLApplication : public ApplicationBase
{
public:
    SDLApplication();
    ~SDLApplication();

    int exec();
    bool init(int argc, char* argv[]);
    void close();
    void onDraw();
    void onExit();
    void* rendererHandle();
    void drawDebugText(const std::string& text, float x, float y);
    void drawDebugText(const std::string& text, const Vector2f& position);
    void drawRectangle(int w, int h, int x, int y, bool fill = false, Colorb col=Colorb::white);
    void setTitle(const std::string &title);
    std::string title() const;

    Vector2i windowSize();
    int windowWidth();
    int windowHeight();
    virtual void setClearColor(const Colorf& color = Colorf::black);
    float fps() const;
protected:
    void pollEvents();
private:
    TTF_Font*    m_debugFont;
    void updateFPS();
    enum {MaxFrameValues=32};
    bool          m_running;
    SDLWindow     m_window;
    SDLRenderer   m_renderer;
    unsigned      m_lastFrame;
    unsigned      m_frameCount;
    unsigned      m_frameValues[MaxFrameValues];
};

#endif // SDLAPPLICATION_HPP
