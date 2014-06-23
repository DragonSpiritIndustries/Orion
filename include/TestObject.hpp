#ifndef TESTOBJECT_HPP
#define TESTOBJECT_HPP

#include <IApplication.hpp>
#include <Object.hpp>
#include <Vector2.hpp>

class ITextureResource;
class TestObject : public Object
{
public:
    TestObject();
    ~TestObject();

    void update(float delta);
    void draw(IApplication* app);

    void onEvent(Event e);

    void onDestroyed();
    void setPosition(float x, float y);
    void setPosition(Vector2f position);
private:
    ITextureResource* m_texture;
    Vector2f m_velocity;
    Vector2f m_position;
    bool     m_idle;
};

#endif // TESTOBJECT_HPP
