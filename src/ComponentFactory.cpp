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

#include "ComponentFactory.hpp"
#include "Console.hpp"
#include "ScriptEngine.hpp"
#include <memory>

ComponentFactory::ComponentFactory()
{
    orConsoleRef.print(orConsoleRef.Info, "ComponentFactory initialized");
}

IComponent*ComponentFactory::newComponent(const std::string& componentType, const std::string& componentName)
{
    if (m_registeredComponents.find(componentType) == m_registeredComponents.end())
        return nullptr;

    return m_registeredComponents[componentType](componentName);
}

void ComponentFactory::registerComponent(const std::string& name, ComponentLoaderFunc func)
{
    if (m_registeredComponents.find(name) != m_registeredComponents.end())
    {
        orConsoleRef.print(orConsoleRef.Error, "Component types must be unique, %s is already a registered type", name.c_str());
        return;
    }

    m_registeredComponents[name] = func;
    orConsoleRef.print(orConsoleRef.Info, "Registered component type %s", name.c_str());
}

ComponentFactory& ComponentFactory::instanceRef()
{
    return *ComponentFactory::instancePtr();
}

ComponentFactory* ComponentFactory::instancePtr()
{
    static std::shared_ptr<ComponentFactory> instance = std::shared_ptr<ComponentFactory>(new ComponentFactory);
    return instance.get();
}
