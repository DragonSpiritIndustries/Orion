#ifndef KEYBOARDMANAGER_HPP
#define KEYBOARDMANAGER_HPP

#include "IKeyboardManager.hpp"
#include <unordered_map>

class SDLKeyboardManager : public IKeyboardManager
{
public:
    SDLKeyboardManager();
    ~SDLKeyboardManager();

    bool keyPressed(Key key);
    bool keyReleased(Key key);
    bool controlPressed();
    bool altPressed();
    bool shiftPressed();

    void shutdown();
protected:
    void translateEvent(const Event& ev);
private:
};

#endif // KEYBOARDMANAGER_HPP
