// This file is part of Orion.
//
// Orion is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Orion is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Orion.  If not, see <http://www.gnu.org/licenses/>

#include "IKeyboardManager.hpp"
#include KEYBOARDMANAGER_IMPL_HEADER
#include "ScriptEngine.hpp"
#include "ApplicationBase.hpp"
#include "EnumToString.hpp"

template<>
const char* enumStrings<Key>::data[] =
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
    orConsoleRef.print(orConsoleRef.Info, "KEYBOARDMANAGER: Intializing\n");
    orConsoleRef.print(orConsoleRef.Info, "KEYBOARDMANAGER: Connecting vital signals\n");
    orApplicationPtr->keyboardSignal().connect<IKeyboardManager, &IKeyboardManager::translateEvent>(this);
    orApplicationPtr->updateSignal().connect<IKeyboardManager, &IKeyboardManager::onUpdate>(this);
    orConsoleRef.print(orConsoleRef.Info, "KEYBOARDMANAGER: Initialized\n");
}

void IKeyboardManager::onUpdate(float)
{
    for (std::pair<const Key, bool>& key : m_releasedKeys)
        key.second = false;
}

IKeyboardManager& IKeyboardManager::instanceRef()
{
    return *instancePtr();
}

IKeyboardManager* IKeyboardManager::instancePtr()
{
    static std::shared_ptr<IKeyboardManager> instance = std::shared_ptr<IKeyboardManager>(new KEYBOARDMANAGER_IMPL);
    return instance.get();
}

static void registerKeyManager()
{
    orScriptEngineRef.handle()->SetDefaultNamespace("Keyboard");
    orScriptEngineRef.handle()->RegisterEnum("Key");
#define K(k) enumToStdString((Key)k).c_str()
    for (int i = 0; i <= (int)Key::KEYCOUNT; i++)
        orScriptEngineRef.handle()->RegisterEnumValue("Key", K(i), i);
    orScriptEngineRef.handle()->SetDefaultNamespace("");

    orScriptEngineRef.handle()->RegisterObjectType("KeyboardManager", 0, asOBJ_REF | asOBJ_NOHANDLE);
    orScriptEngineRef.handle()->RegisterGlobalProperty("KeyboardManager orKeyboardManager", orKeyboardManagerPtr);
    orScriptEngineRef.handle()->RegisterObjectMethod("KeyboardManager", "bool keyPressed(Keyboard::Key key)", asMETHOD(IKeyboardManager, keyPressed), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod("KeyboardManager", "bool keyReleased(Keyboard::Key key)", asMETHOD(IKeyboardManager, keyPressed), asCALL_THISCALL);
}

REGISTER_SCRIPT_FUNCTION(KeyboardManager, registerKeyManager);
