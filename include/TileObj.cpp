#include "TileObj.hpp"
#include "IApplication.hpp"
#include "ITextureResource.hpp"

TileObj::TileObj()
    : Object("testTile")
{
    m_texture = orResourceManagerRef.loadResource<ITextureResource>("test/test.png");
}

void TileObj::setPosition(float x, float y)
{
    setPosition(Vector2f(x, y));
}

void TileObj::setPosition(Vector2f position)
{
    m_position = position;
}

void TileObj::draw(IApplication* app)
{
    app->drawRectangle(32, 32, m_position.x, m_position.y, true);
    if (m_texture)
        m_texture->draw(app, m_position.x, m_position.y, Rectanglef(16, 16, 32, 32), Vector2f(16, 16), true, false, 45.0f);
}
