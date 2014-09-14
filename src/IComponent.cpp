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
