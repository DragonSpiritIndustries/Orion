#ifndef TILEOBJ_HPP
#define TILEOBJ_HPP

#include "Object.hpp"
#include "Vector2.hpp"
class ITextureResource;
class TileObj : public Object
{
public:
    TileObj();

    void setPosition(float x, float y);
    void setPosition(Vector2f position);
    void draw(ApplicationBase * app);
private:
    Vector2f m_position;
    ITextureResource* m_texture;
};

#endif // TILEOBJ_HPP
