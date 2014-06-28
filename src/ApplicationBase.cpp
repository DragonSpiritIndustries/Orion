#include "ApplicationBase.hpp"
#include APPLICATION_IMPL_HEADER
#include WINDOW_IMPL_HEADER
#include "ScriptEngine.hpp"
#include "ObjectManager.hpp"
#include "CVarManager.hpp"
#include "CVar.hpp"
#include "Config.hpp"

CVar* sys_title        = new CVar("sys_title", orDEFAULT_APPLICATION_NAME, "Sets the window title", CVar::Literal, CVar::System | CVar::ReadOnly);
CVar* com_windowWidth  = new CVar("vid_width", "640", "Horizontal resolution of the window", CVar::Integer, CVar::System | CVar::Archive | CVar::ReadOnly);
CVar* com_windowHeight = new CVar("vid_height", "480", "Vertical resolution of the window", CVar::Integer, CVar::System | CVar::Archive | CVar::ReadOnly);
CVar* com_fullscreen   = new CVar("r_fullscreen", "false", "If true, the game renders in fullscreen mode, windowed otherwise", CVar::Boolean, CVar::System | CVar::Archive);
CVar* com_framelimit   = new CVar("sys_framelimit", "60", "Sets the framerate limit", CVar::Integer, CVar::System | CVar::Archive);
CVar* com_verticalSync = new CVar("sys_vsync", "true", "Prevents tearing", CVar::Boolean, CVar::System | CVar::Archive);
CVar* com_showstats    = new CVar("sys_showstats", "false", "Show system statistics", CVar::Boolean, CVar::System | CVar::Archive | CVar::ReadOnly);
CVar* com_clear        = new CVar("r_clear", "true", "If true the window clears after each frame, otherwise it doesn't", CVar::Boolean, CVar::System | CVar::Archive);
CVar* com_clearColor   = new CVar("r_clearcolor", Colorb::black, "Sets the color of the scene", CVar::System | CVar::Archive);
CVar* com_drawwire     = new CVar("r_drawwire", "false", "Draws the geometry of objects on screen", CVar::Boolean, (CVar::System | CVar::Archive));
CVar* com_showfps      = new CVar("r_showfps", "false", "If true, renders the framerate in the upper right hand corner.", CVar::Boolean, (CVar::System | CVar::Archive));

ApplicationBase::ApplicationBase()
{
}

ApplicationBase::~ApplicationBase()
{
    if (m_scriptContext)
        m_scriptContext->Release();
}

int ApplicationBase::exec()
{
    onStart();

    return 0;
}

bool ApplicationBase::init(int /*argc*/, char* argv[])
{
    if (!orScriptEngineRef.handle())
        return false;

    m_window = std::shared_ptr<IWindow>(new WINDOW_IMPL);
    if (!m_window.get()->initialize())
        return false;

    m_renderer = std::shared_ptr<IRenderer>(new RENDERER_IMPL);
    if (!m_renderer.get()->initialize(m_window.get()))
        return false;

    orCVarManagerRef.initialize();
    if (!orResourceManagerRef.initialize(argv[0]))
        return false;
    orConsoleRef.initialize();
    orObjectManagerRef.initialize();

    setTitle(sys_title->toLiteral());

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

Nano::Signal<void (const Event&)>& ApplicationBase::eventSignal()
{
    return m_eventSignal;
}

Nano::Signal<void (const Event&)>& ApplicationBase::keyboardSignal()
{
    return m_keyboardSignal;
}

Nano::Signal<void (const Event&)>& ApplicationBase::mouseButtonSignal()
{
    return m_mouseButtonSignal;
}

Nano::Signal<void (const Event&)>& ApplicationBase::mouseWheelSignal()
{
    return m_mouseWheelSignal;
}

Nano::Signal<void (const Event&)>& ApplicationBase::mouseMoveSignal()
{
    return m_mouseMoveSignal;
}

Nano::Signal<void (const Event&)>& ApplicationBase::joystickSignal()
{
    return m_joystickSignal;
}

Nano::Signal<void (const Event&)>& ApplicationBase::resizeSignal()
{
    return m_resizeSignal;
}

Nano::Signal<void (const Event&)>& ApplicationBase::focusChangedSignal()
{
    return m_focusSignal;
}

Nano::Signal<void (const Event&)>& ApplicationBase::textSignal()
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
    return *instancePtr();
}

ApplicationBase* ApplicationBase::instancePtr()
{
    static std::shared_ptr<ApplicationBase> instance = std::shared_ptr<ApplicationBase>(new APPLICATION_IMPL);
    return  instance.get();
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

static void registerApplication()
{
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
    orScriptEngineRef.handle()->RegisterGlobalProperty("Application orApplication", orApplicationPtr);
}

REGISTER_SCRIPT_FUNCTION(ApplicationBase, registerApplication);
