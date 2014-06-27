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
    orScriptEngineRef.handle()->RegisterObjectMethod("MouseManager", "bool buttonPressed(Key key)", asMETHOD(IKeyboardManager, keyPressed), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod("MouseManager", "bool buttonReleased(Key key)", asMETHOD(IKeyboardManager, keyPressed), asCALL_THISCALL);

    orApplicationRef.updateSignal().connect<IMouseManager, &IMouseManager::onUpdate>(this);
}

void IMouseManager::onUpdate(float)
{
    for (std::pair<const MouseButton, bool>& button : m_released)
        button.second = false;
}
