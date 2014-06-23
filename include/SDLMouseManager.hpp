#ifndef SDLMOUSEMANAGER_HPP
#define SDLMOUSEMANAGER_HPP

#include "IMouseManager.hpp"
#include "Event.hpp"

#include <unordered_map>

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
    void onMouseButton(Event e);
    void onMouseWheel(Event e);
};

#endif // SDLMOUSEMANAGER_HPP
