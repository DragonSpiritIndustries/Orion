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

#include "IMouseManager.hpp"
#include "ApplicationBase.hpp"
#include "ScriptEngine.hpp"
#include "EnumToString.hpp"

template<> const char* enumStrings<MouseButton>::data[] =
{
    "LEFT",
    "MIDDLE",
    "RIGHT",
    "X1",
    "X2"
    "UNKNOWN"
};

IMouseManager::IMouseManager()
{
    orScriptEngineRef.handle()->RegisterEnum("MouseButton");
#define K(k) enumToStdString((MouseButton)k).c_str()
    for (int i = 0; i < (int)MouseButton::COUNT; i++)
        orScriptEngineRef.handle()->RegisterEnumValue("MouseButton", K(i), i);


    orScriptEngineRef.handle()->RegisterObjectType("MouseManager", 0, asOBJ_REF | asOBJ_NOHANDLE);
    orScriptEngineRef.handle()->RegisterGlobalProperty("MouseManager orMouseManager", this);
    orScriptEngineRef.handle()->RegisterObjectMethod("MouseManager", "bool buttonPressed(MouseButton button)", asMETHOD(IKeyboardManager, keyPressed), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod("MouseManager", "bool buttonReleased(MouseButton button)", asMETHOD(IKeyboardManager, keyPressed), asCALL_THISCALL);

    orApplicationRef.updateSignal().connect<IMouseManager, &IMouseManager::onUpdate>(this);
}

void IMouseManager::onUpdate(float)
{
    for (std::pair<const MouseButton, bool>& button : m_released)
        button.second = false;
}
