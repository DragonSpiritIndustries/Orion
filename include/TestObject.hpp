#ifndef TESTOBJECT_HPP
#define TESTOBJECT_HPP

#include <IApplication.hpp>
#include <Object.hpp>
#include <Vector2.hpp>

class TestObject : public Object
{
public:
    TestObject();
    ~TestObject();

    void update(float delta);
    void draw(IApplication* app);

    void onEvent(Event e);

    void onDestroyed();
private:
    Vector2f m_position;
    bool     m_motorState;
};

#endif // TESTOBJECT_HPP
