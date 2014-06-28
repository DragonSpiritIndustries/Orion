#include "IJoystickManager.hpp"
#include "ApplicationBase.hpp"
#include "ScriptEngine.hpp"

IJoystickManager::IJoystickManager()
{
    orApplicationPtr->updateSignal().connect<IJoystickManager, &IJoystickManager::onUpdate>(this);

    if (orScriptEngineRef.handle())
    {
        orScriptEngineRef.handle()->RegisterObjectType("JoystickManager", 0, asOBJ_REF | asOBJ_NOHANDLE);
        orScriptEngineRef.handle()->RegisterGlobalProperty("JoystickManager orJoystickManager", this);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "bool buttonPressed(int joy, int button)",
                                                         asMETHOD(IJoystickManager, buttonPressed),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "bool buttonPressed(const int joy, const int button) const",
                                                         asMETHOD(IJoystickManager, buttonPressed),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "bool buttonReleased(int joy, int button)",
                                                         asMETHOD(IJoystickManager, buttonReleased),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "bool buttonReleased(const int joy, const int button) const",
                                                         asMETHOD(IJoystickManager, buttonReleased),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "float axisPosition(int joy, int axis)",
                                                         asMETHOD(IJoystickManager, axisPosition),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "float axisPosition(const int joy, const int axis) const",
                                                         asMETHOD(IJoystickManager, axisPosition),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "string name(int joy)",
                                                         asMETHOD(IJoystickManager, name),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "string name(const int joy) const",
                                                         asMETHOD(IJoystickManager, name),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "void motorOn(int joy)",
                                                         asMETHOD(IJoystickManager, motorOn),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "void motorOn(const int joy) const",
                                                         asMETHOD(IJoystickManager, motorOn),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "void motorOff(int joy)",
                                                         asMETHOD(IJoystickManager, motorOff),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "void motorOff(const int joy) const",
                                                         asMETHOD(IJoystickManager, motorOff),
                                                         asCALL_THISCALL);
    }
}

void IJoystickManager::onUpdate(float)
{
    for (int i = 0; i < m_releasedButtons.size(); i++)
    {
        for (std::pair<const int, bool>& btn : m_releasedButtons[i])
        {
            btn.second = false;
        }
    }
}
