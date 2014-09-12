#ifndef TRANSFORMABLECOMONENT_HPP
#define TRANSFORMABLECOMONENT_HPP

#include "IComponent.hpp"
#include "Vector2.hpp"

class TransformableComponent : public IComponent
{
public:
    TransformableComponent(const std::string& name);

    Vector2f position() const;
    void setPosition(float x, float y);
    void setPosition(const Vector2f& position);
    void move(float x, float y);
    void move(const Vector2f& offset);

    Vector2f scale() const;
    void setScale(float x, float y);
    void setScale(const Vector2f& scale);
    float rotation() const;
    void setRotation(float rotation);
    //!< Creates a new TransformableComponent
    //!< CAN be used directly, but not advised
    static IComponent* newComponent(const std::string& name);

private:
    Vector2f m_position;
    Vector2f m_scale;
    float    m_rotation;
};

#endif // TRANSFORMABLECOMONENT_HPP
