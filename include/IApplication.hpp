#ifndef IAPPLICATION_HPP
#define IAPPLICATION_HPP

#include <Global.hpp>
#include <string>
#include <memory>
#include <iostream>
#include "Event.hpp"
#include "IKeyboardManager.hpp"
#include "IJoystickManager.hpp"
#include "Color.hpp"
#include "nano-signal-slot/nano_signal_slot.hpp"

class IApplication
{
public:
    virtual ~IApplication() { std::cout << "Application shutdown" << std::endl;}

    virtual int  exec()=0;
    virtual bool init(int argc, char* argv[])=0;
    virtual void onUpdate()=0;
    virtual void onDraw()=0;
    virtual void onExit()=0;
    virtual void setTitle(const std::string& title)=0;
    virtual void close()=0;

    virtual void setClearColor(const Colorb& color = Colorb::black)=0;
    virtual void drawRectangle(int w, int h, int x, int y, bool fill = false)=0;
    virtual Nano::Signal<void(Event)>&         eventSignal();
    virtual Nano::Signal<void(Event)>&         keyboardSignal();
    virtual Nano::Signal<void(Event)>&         mouseButtonSignal();
    virtual Nano::Signal<void(Event)>&         mouseWheelSignal();
    virtual Nano::Signal<void(Event)>&         mouseMoveSignal();
    virtual Nano::Signal<void(Event)>&         joystickSignal();
    virtual Nano::Signal<void(Event)>&         resizeSignal();
    virtual Nano::Signal<void(Event)>&         focusChangedSignal();
    virtual Nano::Signal<void(Event)>&         textSignal();
    virtual Nano::Signal<void(float)>&         updateSignal();
    virtual Nano::Signal<void(int)>&           joystickAddedSignal();
    virtual Nano::Signal<void(int)>&           joystickRemovedSignal();
    virtual Nano::Signal<void(IApplication*)>& drawSignal();
    static IApplication& instanceRef();
    static IApplication* instancePtr();
    IKeyboardManager& keyboardManagerRef();
    IKeyboardManager* keyboardManagerPtr();
    IJoystickManager& joystickManagerRef();
    IJoystickManager* joystickManagerPtr();
    static void setApplication(IApplication* app);
    virtual float fps() const =0;
protected:
    // README: If you add new event handler remember to emit the event using m_eventSignal *FIRST*
    virtual void pollEvents()=0;
    virtual void parseCommandLine(int argc, char* argv[]){UNUSED(argc),UNUSED(argv);}
    std::shared_ptr<IKeyboardManager> m_keyboardManager;
    std::shared_ptr<IJoystickManager> m_joystickManager;
    Nano::Signal<void(Event)>         m_eventSignal;
    Nano::Signal<void(Event)>         m_keyboardSignal;
    Nano::Signal<void(Event)>         m_textSignal;
    Nano::Signal<void(Event)>         m_mouseButtonSignal;
    Nano::Signal<void(Event)>         m_mouseWheelSignal;
    Nano::Signal<void(Event)>         m_mouseMoveSignal;
    Nano::Signal<void(Event)>         m_joystickSignal;
    Nano::Signal<void(Event)>         m_resizeSignal;
    Nano::Signal<void(Event)>         m_focusSignal;
    Nano::Signal<void(float)>         m_updateSignal;
    Nano::Signal<void(int)>           m_joystickAddedSignal;
    Nano::Signal<void(int)>           m_joystickRemovedSignal;
    Nano::Signal<void(IApplication*)> m_drawSignal;
    static std::shared_ptr<IApplication> m_instance;
};

static inline void orCreateApplication(IApplication* ptr)
{
    IApplication::setApplication(ptr);
}

#define orApplicationRef IApplication::instanceRef()
#define orApplicationPtr IApplication::instancePtr()

#define orKeyboardManagerRef orApplicationPtr->keyboardManagerRef()
#define orKeyboardManagerPtr orApplicationPtr->keyboardManagerPtr()
#define orJoystickManagerRef orApplicationPtr->joystickManagerRef()
#define orJoystickManagerPtr orApplicationPtr->joystickManagerPtr()
#endif // IAPPLICATION_HPP
