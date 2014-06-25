#include "ApplicationBase.hpp"
#include "ScriptEngine.hpp"
#include "ObjectManager.hpp"

std::shared_ptr<ApplicationBase> ApplicationBase::m_instance = nullptr;

ApplicationBase::ApplicationBase()
{

}

bool ApplicationBase::init(int /*argc*/, char* argv[])
{
    orScriptEngineRef.initialize();
    if (!orScriptEngineRef.handle())
        return false;

    orConsoleRef.initialize();
    orObjectManagerRef.initialize();
    orScriptEngineRef.handle()->RegisterObjectType("Application", 0, asOBJ_REF | asOBJ_NOHANDLE);
    orScriptEngineRef.handle()->RegisterObjectMethod("Application",
                                                     "float fps()",
                                                     asMETHOD(ApplicationBase, fps), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod("Application",
                                                     "void quit()",
                                                     asMETHOD(ApplicationBase, close), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod("Application",
                                                     "void setTitle(const string& in)",
                                                     asMETHOD(ApplicationBase, setTitle), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod("Application",
                                                     "string title()",
                                                     asMETHOD(ApplicationBase, title), asCALL_THISCALL);

    orScriptEngineRef.handle()->RegisterGlobalProperty("Application application", this);
    orResourceManagerRef.initialize(argv[0]);
    return true;
}

Nano::Signal<void (Event)>& ApplicationBase::eventSignal()
{
    return m_eventSignal;
}

Nano::Signal<void (Event)>& ApplicationBase::keyboardSignal()
{
    return m_keyboardSignal;
}

Nano::Signal<void (Event)>& ApplicationBase::mouseButtonSignal()
{
    return m_mouseButtonSignal;
}

Nano::Signal<void (Event)>& ApplicationBase::mouseWheelSignal()
{
    return m_mouseWheelSignal;
}

Nano::Signal<void (Event)>& ApplicationBase::mouseMoveSignal()
{
    return m_mouseMoveSignal;
}

Nano::Signal<void (Event)>& ApplicationBase::joystickSignal()
{
    return m_joystickSignal;
}

Nano::Signal<void (Event)>& ApplicationBase::resizeSignal()
{
    return m_resizeSignal;
}

Nano::Signal<void (Event)>& ApplicationBase::focusChangedSignal()
{
    return m_focusSignal;
}

Nano::Signal<void (Event)>& ApplicationBase::textSignal()
{
    return m_textSignal;
}

Nano::Signal<void (float)>& ApplicationBase::updateSignal()
{
    return m_updateSignal;
}

Nano::Signal<void (int)>& ApplicationBase::joystickAddedSignal()
{
    return m_joystickAddedSignal;
}

Nano::Signal<void (int)>& ApplicationBase::joystickRemovedSignal()
{
    return m_joystickRemovedSignal;
}

ApplicationBase& ApplicationBase::instanceRef()
{
    return *m_instance.get();
}

ApplicationBase* ApplicationBase::instancePtr()
{
    return  m_instance.get();
}

IKeyboardManager& ApplicationBase::keyboardManagerRef()
{
    return *m_keyboardManager.get();
}

IKeyboardManager* ApplicationBase::keyboardManagerPtr()
{
    return m_keyboardManager.get();
}

IJoystickManager& ApplicationBase::joystickManagerRef()
{
    return *m_joystickManager.get();

}

IJoystickManager* ApplicationBase::joystickManagerPtr()
{
    return m_joystickManager.get();
}

IMouseManager& ApplicationBase::mouseManagerRef()
{
    return *m_mouseManager.get();
}

IMouseManager* ApplicationBase::mouseManagerPtr()
{
    return m_mouseManager.get();
}

void ApplicationBase::setApplication(ApplicationBase* app)
{
    m_instance = std::shared_ptr<ApplicationBase>(app);
}
