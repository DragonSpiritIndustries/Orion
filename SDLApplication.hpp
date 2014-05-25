#ifndef SDLAPPLICATION_HPP
#define SDLAPPLICATION_HPP

#include <IApplication.hpp>
#include "SDLWindow.hpp"
#include "SDLRenderer.hpp"
#include "nano-signal-slot/nano_signal_slot.hpp"

class SDLApplication : public IApplication
{
public:
    SDLApplication();
    ~SDLApplication();

    int exec();
    bool init(int argc, char* argv[]);
    void onEvent(SDL_Event* event);
    void onUpdate();
    void onDraw();
    void onExit();
    void drawRectangle(int w, int h, int x, int y, bool fill = false);
    void setTitle(const std::string &title);
    Nano::Signal<void(IApplication*)>& drawSignal();
    Nano::Signal<void(IApplication*)>& updateSignal();

private:
    bool          m_running;
    SDLWindow     m_window;
    SDLRenderer   m_renderer;
    Nano::Signal<void(IApplication*)> m_drawSignal;
    Nano::Signal<void(IApplication*)> m_updateSignal;
};

#endif // SDLAPPLICATION_HPP
