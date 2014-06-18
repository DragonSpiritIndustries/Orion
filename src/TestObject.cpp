#include "TestObject.hpp"
#include "time.h"

TestObject::TestObject()
    : Object("test"),
      m_idle(false)
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
    bool keyLeft  = (orKeyboardManagerRef.keyPressed(Key::A) || orJoystickManagerRef.buttonPressed(0, 7));
    bool keyRight = (orKeyboardManagerRef.keyPressed(Key::D) || orJoystickManagerRef.buttonPressed(0, 5));
    bool keyUp    = (orKeyboardManagerRef.keyPressed(Key::W) || orJoystickManagerRef.buttonPressed(0, 4));
    bool keyDown  = (orKeyboardManagerRef.keyPressed(Key::S) || orJoystickManagerRef.buttonPressed(0, 6));

    if (keyLeft && !keyRight)
    {
        // stop heading right
        if (m_velocity.X > 0.f)
            m_velocity.X = 0.f;
        m_velocity.X -= 8.f*delta;
    }
    else if (keyRight && !keyLeft)
    {
        // stop heading left
        if (m_velocity.X < 0.f)
            m_velocity.X = 0.f;
        m_velocity.X += 8.f*delta;
    }
    else if (keyRight && keyLeft)
    {
        // Quarter speed
        m_velocity.X *=.75f;
    }
    else
        m_velocity.X = 0;

    if (keyUp && !keyDown)
    {
        // stop heading down
        if (m_velocity.Y > 0.f)
            m_velocity.Y = 0.f;
        m_velocity.Y -= 8.f*delta;
    }
    else if (keyDown && !keyUp)
    {
        // stop heading down
        if (m_velocity.Y < 0.f)
            m_velocity.Y = 0.f;
        m_velocity.Y += 8.f*delta;
    }
    else if (keyDown && keyUp)
    {
        // Quarter speed
        m_velocity.Y *=.75f;
    }
    else
        m_velocity.Y = 0;

    if (orKeyboardManagerRef.keyReleased(Key::ESCAPE)/* || orJoystickManagerRef.buttonReleased(0, 16)*/)
        orApplicationRef.close();

    m_velocity.X += (92.f*orJoystickManagerRef.axisPosition(0, 0))*delta;
    m_velocity.Y += (92.f*orJoystickManagerRef.axisPosition(0, 1))*delta;


    if (m_velocity.X >  2.f)
        m_velocity.X =  2.f;
    if (m_velocity.X < -2.f)
        m_velocity.X = -2.f;
    if (m_velocity.Y >  2.f)
        m_velocity.Y =  2.f;
    if (m_velocity.Y < -2.f)
        m_velocity.Y = -2.f;

    if (m_position.X + 32 > 640 && !m_idle)
    {
        m_velocity.X = 0.0f;
        while(m_position.X + 32 >= 640)
            m_position.X -= .1f;
        m_idle = true;
    }
    else if (m_position.X < 0 && !m_idle)
    {
        m_velocity.X = 0.0f;
        while(m_position.X < 0)
            m_position.X += .1f;
        m_idle = true;
    }


    if (m_position.Y + 32 > 480 && !m_idle)
    {
        m_velocity.Y = 0;
        while(m_position.Y + 32 >= 480)
            m_position.Y -= .1f;
        m_position.Y += .1f;
        m_idle = true;
    }
    else if (m_position.Y < 0 && !m_idle)
    {
        m_velocity.Y = 0;
        while(m_position.Y <= 0)
            m_position.Y += .1f;
        m_position.Y -= .1f;
        m_idle = true;
    }

    if (m_velocity.X != 0.0f || m_velocity.Y != 0.0f)
        m_idle = false;
    else
    {
        if (m_velocity.X == 0.0f)
        {
            m_velocity.X  =  0;
            m_position.X = m_position.X;
        }
        if (m_velocity.Y == 0.0f)
        {
            m_velocity.Y  =  0;
            m_position.Y = m_position.Y;
        }
    }

    m_position += m_velocity;
    m_position.X = std::abs(m_position.X);
    m_position.Y = std::abs(m_position.Y);
}

void TestObject::draw(IApplication* app)
{
    app->drawRectangle(32, 32, m_position.X, m_position.Y, true);
}

void TestObject::onEvent(Event e)
{
//    if (e.type == Event::EV_JOY_BTN_PRESSED || e.type == Event::EV_JOY_BTN_RELEASED)
//        orDebug("%s %i %i\n", orJoystickManagerRef.name(e.eventData.joystickEvent.id).c_str(), e.eventData.joystickEvent.id, e.eventData.joystickEvent.button);
//    else if (e.type == Event::EV_JOY_AXIS)
//    {
//        orDebug("%s %i %i %f\n", orJoystickManagerRef.name(e.eventData.joystickEvent.id).c_str(), e.eventData.joystickEvent.id, e.eventData.joystickEvent.axis, e.eventData.joystickEvent.position);
//    }
}

void TestObject::onDestroyed()
{
    orApplicationRef.drawSignal().disconnect<TestObject, &TestObject::draw>(this);
}
