#include "IComponent.hpp"
#include "ScriptEngine.hpp"
#include "Object.hpp"

IComponent::IComponent(const std::string& name)
    : m_name(name),
      m_owner(nullptr)
{
}

IComponent::~IComponent()
{
}

std::string IComponent::name() const
{
    return m_name;
}

void IComponent::setOwner(Object* owner)
{
    m_owner = owner;
}

Object* IComponent::owner()
{
    return m_owner;
}

void IComponent::setName(const std::string& name)
{
    m_name = name;
}

static void registerComponent()
{
    orScriptEngineRef.handle()->RegisterObjectType("Component", 0, asOBJ_REF | asOBJ_NOCOUNT);
    orScriptEngineRef.handle()->RegisterObjectMethod("Component", "string name()", asMETHOD(IComponent, name), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod("Component", "string name() const", asMETHOD(IComponent, name), asCALL_THISCALL);
}


REGISTER_SCRIPT_FUNCTION(IComponent, registerComponent);
