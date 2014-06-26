#include "TransformableComponent.hpp"

TransformableComponent::TransformableComponent(const std::string& name)
    : IComponent(name)
{
}

IComponent* TransformableComponent::newComponent(const std::string& name)
{
    return new TransformableComponent(name);
}

REGISTER_COMPONENT(TransformableComponent, "Transformable", newComponent);
