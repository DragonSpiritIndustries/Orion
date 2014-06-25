#ifndef TESTOBJECT_HPP
#define TESTOBJECT_HPP

#include <ApplicationBase.hpp>
#include <Object.hpp>
#include <Vector2.hpp>

class ITextureResource;
class TestObject : public Object
{
public:
    TestObject();
    ~TestObject();

    void onUpdate(float delta);
    void draw(ApplicationBase* app);

    void onEvent(Event e);

    void onDestroyed();
private:
    ITextureResource* m_texture;
    Vector2f m_velocity;
    bool     m_idle;
};

#endif // TESTOBJECT_HPP
