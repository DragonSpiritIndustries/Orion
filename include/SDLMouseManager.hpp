#ifndef SDLMOUSEMANAGER_HPP
#define SDLMOUSEMANAGER_HPP

#include "IMouseManager.hpp"
#include <unordered_map>

class Event;
class SDLMouseManager : public IMouseManager
{
public:
    SDLMouseManager();
    void shutdown();
    bool buttonPressed(MouseButton button);
    bool buttonReleased(MouseButton button);

    Vector2i position();
    Vector2i wheelDelta();

    void setMousePosition(int x, int y);
    void setMousePosition(Vector2i position);
protected:
    void onMouseButton(const Event& e);
    void onMouseWheel(const Event& e);
};

#endif // SDLMOUSEMANAGER_HPP
