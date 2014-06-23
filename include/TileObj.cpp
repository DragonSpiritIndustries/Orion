#include "TileObj.hpp"
#include "IApplication.hpp"

TileObj::TileObj()
    : Object("testTile")
{
    orResourceManagerRef.loadResource("test/test.png", "TextureResource");
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
}
