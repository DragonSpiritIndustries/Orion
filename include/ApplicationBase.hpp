#ifndef IAPPLICATION_HPP
#define IAPPLICATION_HPP

#include <Global.hpp>
#include <string>
#include <memory>
#include <iostream>
#include "Event.hpp"
#include "IKeyboardManager.hpp"
#include "IJoystickManager.hpp"
#include "IMouseManager.hpp"
#include "IWindow.hpp"
#include "IRenderer.hpp"
#include "ResourceManager.hpp"
#include "ScriptResource.hpp"
#include "Color.hpp"
#include "Viewport.hpp"
#include "nano-signal-slot/nano_signal_slot.hpp"

class ApplicationBase
{
public:
    ApplicationBase();
    virtual ~ApplicationBase();
    virtual int  exec();
    virtual bool init(int argc, char* argv[]);
    virtual void onStart();
    virtual void onUpdate();
    virtual void onDraw();
    virtual void onExit();
    virtual void setTitle(const std::string& title)=0;
    virtual std::string title() const=0;
    virtual void close()=0;

    virtual void* rendererHandle()=0;
    virtual void drawDebugText(const std::string&, float, float, Colorb col=Colorb::white)=0;
    virtual void drawDebugText(const std::string&, const Vector2f&, Colorb col=Colorb::white)=0;
    virtual Vector2i windowSize();
    virtual void setWindowSize(int w, int h);
    virtual void setWindowSize(const Vector2i& size);
    virtual void setWindowWidth(int w);
    virtual int windowWidth();
    virtual void setWindowHeight(int h);
    virtual int windowHeight();
    virtual void setClearColor(const Colorf& color = Colorf::black)=0;
    virtual void drawRectangle(int w, int h, int x, int y, bool fill = false, Colorb col=Colorb::white)=0;
    virtual Nano::Signal<void(const Event&)>& eventSignal();
    virtual Nano::Signal<void(const Event&)>& keyboardSignal();
    virtual Nano::Signal<void(const Event&)>& mouseButtonSignal();
    virtual Nano::Signal<void(const Event&)>& mouseWheelSignal();
    virtual Nano::Signal<void(const Event&)>& mouseMoveSignal();
    virtual Nano::Signal<void(const Event&)>& joystickSignal();
    virtual Nano::Signal<void(const Event&)>& resizeSignal();
    virtual Nano::Signal<void(const Event&)>& focusChangedSignal();
    virtual Nano::Signal<void(const Event&)>& textSignal();
    virtual Nano::Signal<void(float)>& updateSignal();
    virtual Nano::Signal<void(int)>&   joystickAddedSignal();
    virtual Nano::Signal<void(int)>&   joystickRemovedSignal();
    static ApplicationBase& instanceRef();
    static ApplicationBase* instancePtr();
    IJoystickManager& joystickManagerRef();
    IJoystickManager* joystickManagerPtr();
    IMouseManager&    mouseManagerRef();
    IMouseManager*    mouseManagerPtr();
    // Override to add your specific viewport code
    virtual void setViewport(Viewport& view);
    Viewport* currentViewport();
    Viewport defaultViewport() const;
    void restoreDefaultViewport();
    virtual float fps() const;
protected:
    // README: If you add new event handler remember to emit the event using m_eventSignal *FIRST*
    virtual void pollEvents()=0;
    virtual void parseCommandLine(int argc, char* argv[]){UNUSED(argc),UNUSED(argv);}
    // Use this to implement your viewport code
    // It gets called when a viewport is set and, if necessary, when the window is resized
    virtual void applyViewport()=0;
    std::shared_ptr<IJoystickManager>    m_joystickManager;
    std::shared_ptr<IMouseManager>       m_mouseManager;
    std::shared_ptr<IWindow>             m_window;
    std::shared_ptr<IRenderer>           m_renderer;
    Nano::Signal<void(const Event&)>     m_eventSignal;
    Nano::Signal<void(const Event&)>     m_keyboardSignal;
    Nano::Signal<void(const Event&)>     m_textSignal;
    Nano::Signal<void(const Event&)>     m_mouseButtonSignal;
    Nano::Signal<void(const Event&)>     m_mouseWheelSignal;
    Nano::Signal<void(const Event&)>     m_mouseMoveSignal;
    Nano::Signal<void(const Event&)>     m_joystickSignal;
    Nano::Signal<void(const Event&)>     m_resizeSignal;
    Nano::Signal<void(const Event&)>     m_focusSignal;
    Nano::Signal<void(float)>            m_updateSignal;
    Nano::Signal<void(int)>              m_joystickAddedSignal;
    Nano::Signal<void(int)>              m_joystickRemovedSignal;
    ScriptResource*                      m_mainScript;
    asIScriptContext*                    m_scriptContext;
    Viewport*                            m_currentViewport;
    Viewport                             m_defaultViewport;
    float         m_fps;
    float         m_frameTime;
};

#define orApplicationRef ApplicationBase::instanceRef()
#define orApplicationPtr ApplicationBase::instancePtr()
#define orKeyboardManagerRef IKeyboardManager::instanceRef()
#define orKeyboardManagerPtr IKeyboardManager::instancePtr()
#define orJoystickManagerRef orApplicationPtr->joystickManagerRef()
#define orJoystickManagerPtr orApplicationPtr->joystickManagerPtr()
#define orMouseManagerRef orApplicationPtr->mouseManagerRef()
#define orMouseManagerPtr orApplicationPtr->mouseManagerPtr()


#endif // IAPPLICATION_HPP
