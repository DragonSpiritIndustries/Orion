#include "TestObject.hpp"
#include "time.h"

TestObject::TestObject()
    : Object("test"),
      m_motorState(false)
{
    orApplicationRef.drawSignal().connect<TestObject, &TestObject::draw>(this);
    orApplicationRef.eventSignal().connect<TestObject, &TestObject::onEvent>(this);
    orApplicationRef.updateSignal().connect<TestObject, &TestObject::update>(this);
}

TestObject::~TestObject()
{
}

void TestObject::update(float delta)
{
    if (orKeyboardManagerRef.keyPressed(Key::A) || orJoystickManagerRef.buttonPressed(0, 7))
        m_position.X -= 64.f*delta;
    if (orKeyboardManagerRef.keyPressed(Key::D) || orJoystickManagerRef.buttonPressed(0, 5))
        m_position.X += 2;
    if (orKeyboardManagerRef.keyPressed(Key::W) || orJoystickManagerRef.buttonPressed(0, 4))
        m_position.Y -= 2;
    if (orKeyboardManagerRef.keyPressed(Key::S) || orJoystickManagerRef.buttonPressed(0, 6))
        m_position.Y += 2;
    if (orKeyboardManagerRef.keyReleased(Key::ESCAPE)/* || orJoystickManagerRef.buttonReleased(0, 16)*/)
        orApplicationRef.close();

    m_position.X += (64.f*orJoystickManagerRef.axisPosition(0, 0))*delta;
    m_position.Y += (64.f*orJoystickManagerRef.axisPosition(0, 1))*delta;
}

void TestObject::draw(IApplication* app)
{
    app->drawRectangle(32, 32, m_position.X, m_position.Y, true);
}

void TestObject::onEvent(Event e)
{
    if (e.type == Event::EV_JOY_BTN_PRESSED || e.type == Event::EV_JOY_BTN_RELEASED)
        orDebug("%s %i %i\n", orJoystickManagerRef.name(e.eventData.joystickEvent.id).c_str(), e.eventData.joystickEvent.id, e.eventData.joystickEvent.button);
    else if (e.type == Event::EV_JOY_AXIS)
    {
        orDebug("%s %i %i %f\n", orJoystickManagerRef.name(e.eventData.joystickEvent.id).c_str(), e.eventData.joystickEvent.id, e.eventData.joystickEvent.axis, e.eventData.joystickEvent.position);
    }
}

void TestObject::onDestroyed()
{
    orApplicationRef.drawSignal().disconnect<TestObject, &TestObject::draw>(this);
}
