#ifndef SDLWINDOW_HPP
#define SDLWINDOW_HPP

#include "Global.hpp"
#include <string>
#include <IWindow.hpp>

struct SDL_Window;
class SDLWindow : public IWindow
{
public:
    SDLWindow();
    ~SDLWindow();

    bool initialize();

    void setTitle(const std::string& title);
    std::string title() const;

    void* handle() const;

    Vector2i windowSize();
    void setWindowSize(int w, int h);
    void setWindowSize(const Vector2i& size);
    void setWindowWidth(int w);
    int windowWidth();
    void setWindowHeight(int h);
    int windowHeight();
private:
    SDL_Window* m_window;
};

#endif // SDLWINDOW_HPP
