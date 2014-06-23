#include "IApplication.hpp"

std::shared_ptr<IApplication> IApplication::m_instance = nullptr;

Nano::Signal<void (Event)>& IApplication::eventSignal()
{
    return m_eventSignal;
}

Nano::Signal<void (Event)>& IApplication::keyboardSignal()
{
    return m_keyboardSignal;
}

Nano::Signal<void (Event)>& IApplication::mouseButtonSignal()
{
    return m_mouseButtonSignal;
}

Nano::Signal<void (Event)>& IApplication::mouseWheelSignal()
{
    return m_mouseWheelSignal;
}

Nano::Signal<void (Event)>& IApplication::mouseMoveSignal()
{
    return m_mouseMoveSignal;
}

Nano::Signal<void (Event)>& IApplication::joystickSignal()
{
    return m_joystickSignal;
}

Nano::Signal<void (Event)>& IApplication::resizeSignal()
{
    return m_resizeSignal;
}

Nano::Signal<void (Event)>& IApplication::focusChangedSignal()
{
    return m_focusSignal;
}

Nano::Signal<void (Event)>& IApplication::textSignal()
{
    return m_textSignal;
}

Nano::Signal<void (float)>& IApplication::updateSignal()
{
    return m_updateSignal;
}

Nano::Signal<void (int)>& IApplication::joystickAddedSignal()
{
    return m_joystickAddedSignal;
}

Nano::Signal<void (int)>& IApplication::joystickRemovedSignal()
{
    return m_joystickRemovedSignal;
}

IApplication& IApplication::instanceRef()
{
    return *m_instance.get();
}

IApplication* IApplication::instancePtr()
{
    return  m_instance.get();
}

ResourceManager&IApplication::resourceManagerRef()
{
    return *m_resourceManager.get();
}

ResourceManager* IApplication::resourceManagerPtr()
{
    return m_resourceManager.get();
}

IKeyboardManager& IApplication::keyboardManagerRef()
{
    return *m_keyboardManager.get();
}

IKeyboardManager* IApplication::keyboardManagerPtr()
{
    return m_keyboardManager.get();
}

IJoystickManager& IApplication::joystickManagerRef()
{
    return *m_joystickManager.get();

}

IJoystickManager* IApplication::joystickManagerPtr()
{
    return m_joystickManager.get();
}

IMouseManager& IApplication::mouseManagerRef()
{
    return *m_mouseManager.get();
}

IMouseManager* IApplication::mouseManagerPtr()
{
    return m_mouseManager.get();
}

void IApplication::setApplication(IApplication* app)
{
    m_instance = std::shared_ptr<IApplication>(app);
}
