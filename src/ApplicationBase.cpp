#include "ApplicationBase.hpp"
#include "ScriptEngine.hpp"
#include "ObjectManager.hpp"

std::shared_ptr<ApplicationBase> ApplicationBase::m_instance = nullptr;

ApplicationBase::ApplicationBase()
{}

ApplicationBase::~ApplicationBase()
{
    if (m_scriptContext)
        m_scriptContext->Release();
}

int ApplicationBase::exec()
{
    onStart();
}

bool ApplicationBase::init(int /*argc*/, char* argv[])
{
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

    orScriptEngineRef.handle()->RegisterGlobalProperty("Application orApplication", this);
    orResourceManagerRef.initialize(argv[0]);

    m_mainScript = orResourceManagerRef.loadResource<ScriptResource>("scripts/main.as");
    if (!m_mainScript)
        return false;

    m_scriptContext = orScriptEngineRef.handle()->CreateContext();

    if (m_scriptContext)
    {
        asIScriptFunction* initFunc = m_mainScript->functionByName("onInitialized");
        if (initFunc)
        {
            m_scriptContext->Prepare(initFunc);
            m_scriptContext->Execute();
        }
    }

    return true;
}

void ApplicationBase::onStart()
{
    if (m_scriptContext)
    {
        asIScriptFunction* startFunc = m_mainScript->functionByName("onStart");
        if (startFunc)
        {
            m_scriptContext->Prepare(startFunc);
            m_scriptContext->Execute();
        }
    }
}

void ApplicationBase::onUpdate()
{
    if (m_scriptContext)
    {
        asIScriptFunction* updateFunc = m_mainScript->functionByName("onUpdate");
        if (updateFunc)
        {
            m_scriptContext->Prepare(updateFunc);
            m_scriptContext->SetArgFloat(0, m_frameTime);
            m_scriptContext->Execute();
        }
    }
    m_updateSignal(m_frameTime);
}

void ApplicationBase::onExit()
{
    if (m_scriptContext)
    {
        asIScriptFunction* shutdownFunc = m_mainScript->functionByName("onShutdown");
        if (shutdownFunc)
        {
            m_scriptContext->Prepare(shutdownFunc);
            m_scriptContext->Execute();
        }
    }

    orObjectManagerRef.shutdown();
    orResourceManagerRef.shutdown();
    m_joystickManager.get()->shutdown();
    m_keyboardManager.get()->shutdown();
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
