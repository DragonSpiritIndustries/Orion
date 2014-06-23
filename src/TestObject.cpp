#include "TestObject.hpp"
#include "TileObj.hpp"
#include "ObjectManager.hpp"
#include "ITextureResource.hpp"
#include "time.h"

TestObject::TestObject()
    : Object("test"),
      m_idle(false)
{
    orApplicationRef.eventSignal().connect<TestObject, &TestObject::onEvent>(this);
    orApplicationRef.updateSignal().connect<TestObject, &TestObject::update>(this);
    IResource* res = orResourceManagerRef.loadResource("test/test.png", "TextureResource");
    m_texture = dynamic_cast<ITextureResource*>(res);
}

TestObject::~TestObject()
{
}

void TestObject::update(float delta)
{
    m_position.x = (orMouseManagerPtr->position().x / 32) * (32);
    m_position.y = (orMouseManagerPtr->position().y / 32) * (32);
    if (orMouseManagerRef.buttonPressed(MouseButton::Left))
    {
        TileObj* newObj = new TileObj;
        newObj->setPosition(m_position);
        orObjectManagerRef.addObject(newObj);
    }
    return;
    bool keyLeft  = (orKeyboardManagerRef.keyPressed(Key::A) || orJoystickManagerRef.buttonPressed(0, 7));
    bool keyRight = (orKeyboardManagerRef.keyPressed(Key::D) || orJoystickManagerRef.buttonPressed(0, 5));
    bool keyUp    = (orKeyboardManagerRef.keyPressed(Key::W) || orJoystickManagerRef.buttonPressed(0, 4));
    bool keyDown  = (orKeyboardManagerRef.keyPressed(Key::S) || orJoystickManagerRef.buttonPressed(0, 6));

    if (keyLeft)
    {
    }

    if (keyLeft && !keyRight)
    {
        // stop heading right
        if (m_velocity.x > 0.f)
            m_velocity.x = 0.f;
        m_velocity.x -= delta*32.f;
    }
    else if (keyRight && !keyLeft)
    {
        // stop heading left
        if (m_velocity.x < 0.f)
            m_velocity.x = 0.f;
        m_velocity.x += delta*32.f;
    }
    else if (keyRight && keyLeft)
    {
        // Quarter speed
        m_velocity.x *=.75f;
    }
    else
        m_velocity.x = 0;

    if (keyUp && !keyDown)
    {
        // stop heading down
        if (m_velocity.y > 0.f)
            m_velocity.y = 0.f;
        m_velocity.y -= delta*32.f;
    }
    else if (keyDown && !keyUp)
    {
        // stop heading down
        if (m_velocity.y < 0.f)
            m_velocity.y = 0.f;
        m_velocity.y += delta*32.f;
    }
    else if (keyDown && keyUp)
    {
        // Quarter speed
        m_velocity.y *=.75f;
    }
    else
        m_velocity.y = 0;

    if (!keyDown && !keyUp && !keyLeft && !keyRight)
        m_idle = true;

    if (orKeyboardManagerRef.keyReleased(Key::ESCAPE)/* || orJoystickManagerRef.buttonReleased(0, 16)*/)
        orApplicationRef.close();

    m_velocity.x += (92.f*orJoystickManagerRef.axisPosition(0, 0))*delta;
    m_velocity.y += (92.f*orJoystickManagerRef.axisPosition(0, 1))*delta;


    if (m_velocity.x >  64.f*delta)
        m_velocity.x =  64.f*delta;
    if (m_velocity.x < -64.f*delta)
        m_velocity.x = -64.f*delta;
    if (m_velocity.y >  64.f*delta)
        m_velocity.y =  64.f*delta;
    if (m_velocity.y < -64.f*delta)
        m_velocity.y = -64.f*delta;

    if ((m_position.x + 32)  + m_velocity.x > 640)
    {
        while((m_position.x + 32) + m_velocity.x > 640)
            m_position.x -= m_velocity.x;
        m_velocity.x = 0;
        m_idle = true;
    }

    if (m_position.x - m_velocity.x < 0)
    {
        while(m_position.x - m_velocity.x < 0)
            m_position.x += m_velocity.x;
        m_velocity.x = 0;
        m_idle = true;
    }


    if ((m_position.y + 32) + m_velocity.y > 480)
    {
        while((m_position.y + 32) + m_velocity.y  > 480)
            m_position.y -= m_velocity.y;
        m_velocity.y = 0;
        m_idle = true;
    }
    if (m_position.y - m_velocity.y < 0)
    {
        while(m_position.y - m_velocity.y < 0)
            m_position.y += m_velocity.y;
        m_velocity.y = 0;
        m_idle = true;
    }

    if (m_velocity.x != 0.0f || m_velocity.y != 0.0f)
        m_idle = false;
    else
    {
        if (m_velocity.x == 0.0f)
        {
            m_velocity.x  =  0;
            m_position.x = m_position.x;
        }
        if (m_velocity.y == 0.0f)
        {
            m_velocity.y  =  0;
            m_position.y = m_position.y;
        }
    }

    m_position += m_velocity;
    m_position.x = std::abs(m_position.x);
    m_position.y = std::abs(m_position.y);
}

void TestObject::draw(IApplication* app)
{
    if (m_texture)
        m_texture->draw(app, m_position.x, m_position.y);
    app->drawRectangle(32, 32, m_position.x, m_position.y, true);
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
    orApplicationRef.eventSignal().disconnect<TestObject,  &TestObject::onEvent>(this);
    orApplicationRef.updateSignal().disconnect<TestObject, &TestObject::update>(this);
}

void TestObject::setPosition(float x, float y)
{
    setPosition(Vector2f(x, y));
}

void TestObject::setPosition(Vector2f position)
{
    m_position = position;
}
