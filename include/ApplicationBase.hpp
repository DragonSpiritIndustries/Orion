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
#include "ResourceManager.hpp"
#include "ScriptResource.hpp"
#include "Color.hpp"
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
    virtual void onDraw()=0;
    virtual void onExit();
    virtual void setTitle(const std::string& title)=0;
    virtual std::string title() const=0;
    virtual void close()=0;

    virtual void* rendererHandle()=0;
    virtual void drawDebugText(const std::string&, float, float)=0;
    virtual void drawDebugText(const std::string&, const Vector2f&)=0;
    virtual Vector2i windowSize()=0;
    virtual int windowWidth()=0;
    virtual int windowHeight()=0;
    virtual void setClearColor(const Colorf& color = Colorf::black)=0;
    virtual void drawRectangle(int w, int h, int x, int y, bool fill = false, Colorf col=Colorf::white)=0;
    virtual Nano::Signal<void(Event)>& eventSignal();
    virtual Nano::Signal<void(Event)>& keyboardSignal();
    virtual Nano::Signal<void(Event)>& mouseButtonSignal();
    virtual Nano::Signal<void(Event)>& mouseWheelSignal();
    virtual Nano::Signal<void(Event)>& mouseMoveSignal();
    virtual Nano::Signal<void(Event)>& joystickSignal();
    virtual Nano::Signal<void(Event)>& resizeSignal();
    virtual Nano::Signal<void(Event)>& focusChangedSignal();
    virtual Nano::Signal<void(Event)>& textSignal();
    virtual Nano::Signal<void(float)>& updateSignal();
    virtual Nano::Signal<void(int)>&   joystickAddedSignal();
    virtual Nano::Signal<void(int)>&   joystickRemovedSignal();
    static ApplicationBase& instanceRef();
    static ApplicationBase* instancePtr();
    IKeyboardManager& keyboardManagerRef();
    IKeyboardManager* keyboardManagerPtr();
    IJoystickManager& joystickManagerRef();
    IJoystickManager* joystickManagerPtr();
    IMouseManager&    mouseManagerRef();
    IMouseManager*    mouseManagerPtr();
    virtual float fps() const =0;
protected:
    // README: If you add new event handler remember to emit the event using m_eventSignal *FIRST*
    virtual void pollEvents()=0;
    virtual void parseCommandLine(int argc, char* argv[]){UNUSED(argc),UNUSED(argv);}
    std::shared_ptr<IKeyboardManager>    m_keyboardManager;
    std::shared_ptr<IJoystickManager>    m_joystickManager;
    std::shared_ptr<IMouseManager>       m_mouseManager;
    Nano::Signal<void(Event)>            m_eventSignal;
    Nano::Signal<void(Event)>            m_keyboardSignal;
    Nano::Signal<void(Event)>            m_textSignal;
    Nano::Signal<void(Event)>            m_mouseButtonSignal;
    Nano::Signal<void(Event)>            m_mouseWheelSignal;
    Nano::Signal<void(Event)>            m_mouseMoveSignal;
    Nano::Signal<void(Event)>            m_joystickSignal;
    Nano::Signal<void(Event)>            m_resizeSignal;
    Nano::Signal<void(Event)>            m_focusSignal;
    Nano::Signal<void(float)>            m_updateSignal;
    Nano::Signal<void(int)>              m_joystickAddedSignal;
    Nano::Signal<void(int)>              m_joystickRemovedSignal;
    ScriptResource*                      m_mainScript;
    asIScriptContext*                    m_scriptContext;
    float         m_fps;
    float         m_frameTime;
};

#define orApplicationRef ApplicationBase::instanceRef()
#define orApplicationPtr ApplicationBase::instancePtr()
#define orKeyboardManagerRef orApplicationPtr->keyboardManagerRef()
#define orKeyboardManagerPtr orApplicationPtr->keyboardManagerPtr()
#define orJoystickManagerRef orApplicationPtr->joystickManagerRef()
#define orJoystickManagerPtr orApplicationPtr->joystickManagerPtr()
#define orMouseManagerRef orApplicationPtr->mouseManagerRef()
#define orMouseManagerPtr orApplicationPtr->mouseManagerPtr()


#endif // IAPPLICATION_HPP
