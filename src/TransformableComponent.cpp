// This file is part of Orion.
//
// Orion is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Orion is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Orion.  If not, see <http://www.gnu.org/licenses/>

#include "TransformableComponent.hpp"
#include "ScriptEngine.hpp"

TransformableComponent::TransformableComponent(const std::string& name)
    : IComponent(name)
{
}

Vector2f TransformableComponent::position() const
{
    return m_position;
}

void TransformableComponent::setPosition(float x, float y)
{
    setPosition(Vector2f(x, y));
}

void TransformableComponent::setPosition(const Vector2f& position)
{
    m_position = position;
}

void TransformableComponent::move(float x, float y)
{
    move(Vector2f(x, y));
}

void TransformableComponent::move(const Vector2f& offset)
{
    m_position += offset;
}

Vector2f TransformableComponent::scale() const
{
    return m_scale;
}

void TransformableComponent::setScale(float x, float y)
{
    setScale(Vector2f(x, y));
}

void TransformableComponent::setScale(const Vector2f& scale)
{
    m_scale = scale;
}

float TransformableComponent::rotation() const
{
    return m_rotation;
}

void TransformableComponent::setRotation(float rotation)
{
    m_rotation = rotation;
}

IComponent* TransformableComponent::newComponent(const std::string& name)
{
    return new TransformableComponent(name);
}

REGISTER_COMPONENT(TransformableComponent, "Transformable", newComponent);

static void registerTransformable()
{
    orScriptEngineRef.handle()->RegisterObjectType     ("Transformable", 0, asOBJ_REF | asOBJ_NOCOUNT);
    orScriptEngineRef.handle()->RegisterObjectBehaviour("Component",     asBEHAVE_REF_CAST, "Transformable@ f()", asFUNCTION((refCast<IComponent, TransformableComponent>)), asCALL_CDECL_OBJLAST);
    orScriptEngineRef.handle()->RegisterObjectBehaviour("Transformable", asBEHAVE_REF_CAST, "Component@ f()", asFUNCTION((refCast<TransformableComponent, IComponent>)), asCALL_CDECL_OBJLAST);
    orScriptEngineRef.handle()->RegisterObjectMethod   ("Transformable", "Vector2f position() const", asMETHOD(TransformableComponent, position), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod   ("Transformable", "void setPosition(float x, float y)", asMETHODPR(TransformableComponent, setPosition, (float, float), void), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod   ("Transformable", "void setPosition(const Vector2f& in)", asMETHODPR(TransformableComponent, setPosition, (const Vector2f&), void), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod   ("Transformable", "Vector2f scale() const", asMETHOD(TransformableComponent, scale), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod   ("Transformable", "void scale(float x, float y)", asMETHODPR(TransformableComponent, setScale, (float, float), void), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod   ("Transformable", "void scale(const Vector2f& in)", asMETHODPR(TransformableComponent, setScale, (const Vector2f&), void), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod   ("Transformable", "Vector2f rotation() const", asMETHOD(TransformableComponent, rotation), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod   ("Transformable", "void move(float x, float y)", asMETHODPR(TransformableComponent, move, (float, float), void), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod   ("Transformable", "void move(const Vector2f& in)", asMETHODPR(TransformableComponent, move, (const Vector2f&), void), asCALL_THISCALL);
}

REGISTER_SCRIPT_FUNCTION(TransformableComponent, registerTransformable);
