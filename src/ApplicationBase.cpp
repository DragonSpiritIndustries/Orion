#include "ApplicationBase.hpp"
#include APPLICATION_IMPL_HEADER
#include WINDOW_IMPL_HEADER
#include MOUSEMANAGER_IMPL_HEADER
#include JOYSTICKMANAGER_IMPL_HEADER
#include "ScriptEngine.hpp"
#include "ObjectManager.hpp"
#include "CVarManager.hpp"
#include "CVar.hpp"
#include "Config.hpp"
#include "GL/gl.h"

CVar* sys_title        = new CVar("sys_title",     orDEFAULT_APPLICATION_NAME, "Sets the window title", CVar::Literal, CVar::System | CVar::ReadOnly);
CVar* com_windowWidth  = new CVar("vid_width",     "640", "Horizontal resolution of the window", CVar::Integer, CVar::System | CVar::Archive | CVar::ReadOnly);
CVar* com_windowHeight = new CVar("vid_height",    "480", "Vertical resolution of the window", CVar::Integer, CVar::System | CVar::Archive | CVar::ReadOnly);
CVar* com_fullscreen   = new CVar("r_fullscreen",  "false", "If true, the game renders in fullscreen mode, windowed otherwise", CVar::Boolean, CVar::System | CVar::Archive);
CVar* com_framelimit   = new CVar("sys_framelimit","60", "Sets the framerate limit", CVar::Integer, CVar::System | CVar::Archive);
CVar* com_verticalSync = new CVar("sys_vsync",     "true", "Prevents tearing", CVar::Boolean, CVar::System | CVar::Archive);
CVar* com_showstats    = new CVar("sys_showstats", "false", "Show system statistics", CVar::Boolean, CVar::System | CVar::Archive | CVar::ReadOnly);
CVar* com_clear        = new CVar("r_clear",       "true", "If true the window clears after each frame, otherwise it doesn't", CVar::Boolean, CVar::System | CVar::Archive);
CVar* com_clearColor   = new CVar("r_clearcolor",  Colorb::black, "Sets the color of the scene", CVar::System | CVar::Archive);
CVar* com_drawwire     = new CVar("r_drawwire",    "false", "Draws the geometry of objects on screen", CVar::Boolean, (CVar::System | CVar::Archive));
CVar* com_showfps      = new CVar("r_showfps",     "false", "If true, renders the framerate in the upper right hand corner.", CVar::Boolean, CVar::System | CVar::Archive);

ApplicationBase::ApplicationBase()
    : m_scriptContext(nullptr)
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
    orCVarManagerRef.initialize();

    m_window = std::shared_ptr<IWindow>(new WINDOW_IMPL);
    if (!m_window.get()->initialize())
        return false;

    m_renderer = std::shared_ptr<IRenderer>(new RENDERER_IMPL);
    if (!m_renderer.get()->initialize(m_window.get()))
        return false;

    if (!orResourceManagerRef.initialize(argv[0]))
        return false;

    orConsoleRef.initialize();
    orObjectManagerRef.initialize();
    glPolygonMode(GL_FRONT_AND_BACK, (com_drawwire->toBoolean() ? GL_LINE : GL_FILL));
    // First initialize the input managers
    m_joystickManager = std::shared_ptr<IJoystickManager>(new JOYSTICKMANAGER_IMPL);
    m_mouseManager    = std::shared_ptr<IMouseManager>   (new MOUSEMANAGER_IMPL);

    m_mainScript = orResourceManagerRef.loadResource<ScriptResource>("scripts/main.as");
    if (!m_mainScript)
        orConsoleRef.print(orConsoleRef.Fatal, "Unable to load main.as");
    else
    {
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
    if (windowWidth() != com_windowWidth->toInteger())
    {
        if (com_windowWidth->isModified())
            setWindowWidth(com_windowWidth->toInteger());
        else
        {
            CVarUnlocker unlock(com_windowWidth);
            com_windowWidth->fromInteger(windowWidth());
        }
        com_windowWidth->clearModified();
    }
    if (windowHeight() != com_windowHeight->toInteger())
    {
        if (com_windowHeight->isModified())
            setWindowHeight(com_windowHeight->toInteger());
        else
        {
            CVarUnlocker unlock(com_windowHeight);
            com_windowHeight->fromInteger(windowHeight());
        }
        com_windowHeight->clearModified();
    }

    if (com_verticalSync->isModified())
    {
        m_renderer.get()->setVSync(com_verticalSync->toBoolean());
        com_verticalSync->clearModified();
    }

    // TODO: Abstract this
    if (com_drawwire->isModified())
    {
        glPolygonMode(GL_FRONT_AND_BACK, (com_drawwire->toBoolean() ? GL_LINE : GL_FILL));
        com_drawwire->clearModified();
    }

    if (com_clearColor->isModified())
    {
        m_renderer.get()->setClearColor(com_clearColor->toColorf());
        com_clearColor->clearModified();
    }

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

void ApplicationBase::onDraw()
{
    orObjectManagerRef.draw();
    orScriptEngineRef.onDraw();
    orConsoleRef.draw();
    if (com_showfps->toBoolean())
        drawDebugText(Athena::utility::sprintf("FPS: %.2f", m_fps), windowWidth() - 100, 0);
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
    orKeyboardManagerRef.shutdown();
}

Vector2i ApplicationBase::windowSize()
{
    return m_window.get()->windowSize();
}

void ApplicationBase::setWindowSize(int w, int h)
{
    m_window.get()->setWindowSize(w, h);
}

void ApplicationBase::setWindowSize(const Vector2i& size)
{
    m_window.get()->setWindowSize(size);
}

void ApplicationBase::setWindowWidth(int w)
{
    m_window.get()->setWindowWidth(w);
}

int ApplicationBase::windowWidth()
{
    return windowSize().x;
}

void ApplicationBase::setWindowHeight(int h)
{
    m_window.get()->setWindowHeight(h);
}

int ApplicationBase::windowHeight()
{
    return windowSize().y;
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

float ApplicationBase::fps() const
{
    return m_fps;
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
