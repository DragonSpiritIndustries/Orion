#ifndef TRANSFORMABLECOMONENT_HPP
#define TRANSFORMABLECOMONENT_HPP

#include "IComponent.hpp"
#include "Vector2.hpp"

class TransformableComponent : public IComponent
{
public:
    TransformableComponent(const std::string& name);

    float rotation();
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
