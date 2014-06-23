#ifndef TILEOBJ_HPP
#define TILEOBJ_HPP

#include "Object.hpp"
#include "Vector2.hpp"
class TileObj : public Object
{
public:
    TileObj();

    void setPosition(float x, float y);
    void setPosition(Vector2f position);
    void draw(IApplication * app);
private:
    Vector2f m_position;
};

#endif // TILEOBJ_HPP
