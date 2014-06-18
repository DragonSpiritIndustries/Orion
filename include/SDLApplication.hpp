#ifndef SDLAPPLICATION_HPP
#define SDLAPPLICATION_HPP

#include <IApplication.hpp>
#include "SDLWindow.hpp"
#include "SDLRenderer.hpp"
#include <SDL2/SDL_joystick.h>

class SDLApplication : public IApplication
{
public:
    SDLApplication();
    ~SDLApplication();

    int exec();
    bool init(int argc, char* argv[]);
    void close();
    void onUpdate();
    void onDraw();
    void onExit();
    void drawRectangle(int w, int h, int x, int y, bool fill = false);
    void setTitle(const std::string &title);
    virtual void setClearColor(const Colorb& color = Colorb::black);
    float fps() const;
protected:
    void pollEvents();
private:
    void updateFPS();
    enum {MaxFrameValues=32};
    bool          m_running;
    SDLWindow     m_window;
    SDLRenderer   m_renderer;
    unsigned      m_lastFrame;
    unsigned      m_frameCount;
    unsigned      m_frameValues[MaxFrameValues];
    float         m_fps;
    float         m_frameTime;
};

#endif // SDLAPPLICATION_HPP
