#include "IKeyboardManager.hpp"
#include "ScriptEngine.hpp"
#include "EnumToString.hpp"

template<> const char* enumStrings<Key>::data[] =
{
    "A",
    "B",
    "C",
    "D",
    "E",
    "F",
    "G",
    "H",
    "I",
    "J",
    "K",
    "L",
    "M",
    "N",
    "O",
    "P",
    "Q",
    "R",
    "S",
    "T",
    "U",
    "V",
    "W",
    "X",
    "Y",
    "Z",
    "NUM0",
    "NUM1",
    "NUM2",
    "NUM3",
    "NUM4",
    "NUM5",
    "NUM6",
    "NUM7",
    "NUM8",
    "NUM9",
    "ESCAPE",
    "LCONTROL",
    "LSHIFT",
    "LALT",
    "LSYSTEM",
    "RCONTROL",
    "RSHIFT",
    "RALT",
    "RSYSTEM",
    "MENU",
    "LBRACKET",
    "RBRACKET",
    "SEMICOLON",
    "COMMA",
    "PERIOD",
    "APOSTROPHE",
    "FORARDSLASH",
    "BACKSLASH",
    "TILDE",
    "EQUAL",
    "DASH",
    "SPACE",
    "RETURN",
    "BACKSPACE",
    "TAB",
    "PAGEUP",
    "PAGEDOWN",
    "END",
    "HOME",
    "INSERT",
    "DELETE",
    "PLUS",
    "MINUS",
    "MULTIPLY",
    "DIVIDE",
    "LEFT",
    "RIGHT",
    "UP",
    "DOWN",
    "NUMPAD0",
    "NUMPAD1",
    "NUMPAD2",
    "NUMPAD3",
    "NUMPAD4",
    "NUMPAD5",
    "NUMPAD6",
    "NUMPAD7",
    "NUMPAD8",
    "NUMPAD9",
    "CAPSLOCK",
    "PRINTSCREEN",
    "SCROLLLOCK",
    "F1",
    "F2",
    "F3",
    "F4",
    "F5",
    "F6",
    "F7",
    "F8",
    "F9",
    "F10",
    "F11",
    "F12",
    "F13",
    "F14",
    "F15",
    "PAUSE",
    "UNKNOWN"
};

IKeyboardManager::IKeyboardManager()
{
    orScriptEngineRef.handle()->RegisterEnum("Key");
#define K(k) enumToStdString((Key)k).c_str()
    for (int i = 0; i <= (int)Key::KEYCOUNT; i++)
        orScriptEngineRef.handle()->RegisterEnumValue("Key", K(i), i);


    orScriptEngineRef.handle()->RegisterObjectType("KeyboardManager", 0, asOBJ_REF | asOBJ_NOHANDLE);
    orScriptEngineRef.handle()->RegisterGlobalProperty("KeyboardManager orKeyboardManager", this);
    orScriptEngineRef.handle()->RegisterObjectMethod("KeyboardManager", "bool keyPressed(Key key)", asMETHOD(IKeyboardManager, keyPressed), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod("KeyboardManager", "bool keyReleased(Key key)", asMETHOD(IKeyboardManager, keyPressed), asCALL_THISCALL);
}

void IKeyboardManager::onUpdate(float)
{
    for (std::pair<const Key, bool>& key : m_releasedKeys)
        key.second = false;
}
