#ifndef IKEYBOARDMANAGER_HPP
#define IKEYBOARDMANAGER_HPP

#include "Global.hpp"
#include <unordered_map>

enum class KeyModifier : short
{
    NONE     = 0,
    LSHIFT   = (1 <<  0),
    RSHIFT   = (1 <<  1),
    LCONTROL = (1 <<  2),
    RCONTROL = (1 <<  3),
    LALT     = (1 <<  4),
    RALT     = (1 <<  5),
    LGUI     = (1 <<  6),
    RGUI     = (1 <<  7),
    NUM      = (1 <<  8),
    CAPS     = (1 <<  9),
    MODE     = (1 << 10),
    SHIFT    = LSHIFT | RSHIFT,
    CONTROL  = LCONTROL | RCONTROL,
    ALT      = LALT | RALT,
    GUI      = LGUI | RGUI
};

enum class Key : int
{
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    NUM0,
    NUM1,
    NUM2,
    NUM3,
    NUM4,
    NUM5,
    NUM6,
    NUM7,
    NUM8,
    NUM9,
    ESCAPE,
    LCONTROL,
    LSHIFT,
    LALT,
    LSYSTEM,
    RCONTROL,
    RSHIFT,
    RALT,
    RSYSTEM,
    MENU,
    LBRACKET,
    RBRACKET,
    SEMICOLON,
    COMMA,
    PERIOD,
    APOSTROPHE,
    FORARDSLASH,
    BACKSLASH,
    TILDE,
    EQUAL,
    DASH,
    SPACE,
    RETURN,
    BACKSPACE,
    TAB,
    PAGEUP,
    PAGEDOWN,
    END,
    HOME,
    INSERT,
    DELETE,
    PLUS,
    MINUS,
    MULTIPLY,
    DIVIDE,
    LEFT,
    RIGHT,
    UP,
    DOWN,
    NUMPAD0,
    NUMPAD1,
    NUMPAD2,
    NUMPAD3,
    NUMPAD4,
    NUMPAD5,
    NUMPAD6,
    NUMPAD7,
    NUMPAD8,
    NUMPAD9,
    CAPSLOCK,
    PRINTSCREEN,
    SCROLLLOCK,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    PAUSE,

    KEYCOUNT,
    UNKNOWN = KEYCOUNT
};


// For std::unordered_map
namespace std
{
template<>
struct hash< ::Key >
{
    typedef ::Key argument_type;
    typedef std::underlying_type< argument_type >::type underlying_type;
    typedef std::hash< underlying_type >::result_type result_type;
    result_type operator()( const argument_type& arg ) const
    {
        std::hash< underlying_type > hasher;
        return hasher( static_cast< underlying_type >( arg ) );
    }
};
}

Key orFromScanCode(int scan);

class Event;
class IKeyboardManager
{
public:
    IKeyboardManager();
    virtual void onUpdate(float);
    virtual bool keyPressed(Key)=0;
    virtual bool keyReleased(Key)=0;
    virtual bool controlPressed()=0;
    virtual bool altPressed()=0;
    virtual bool shiftPressed()=0;
    virtual void shutdown()=0;

    static IKeyboardManager& instanceRef();
    static IKeyboardManager* instancePtr();
protected:
    virtual void translateEvent(const Event&)=0;
    std::unordered_map<Key, bool> m_pressedKeys;
    std::unordered_map<Key, bool> m_releasedKeys;
};

#endif // IKEYBOARDMANAGER_HPP
