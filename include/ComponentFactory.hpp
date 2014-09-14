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

#ifndef COMPONENTMANAGER_HPP
#define COMPONENTMANAGER_HPP

#include "Global.hpp"
#include <unordered_map>
#include <string>

class IComponent;
typedef IComponent* (*ComponentLoaderFunc)(const std::string&);

class ComponentFactory
{
public:
    ComponentFactory();


    IComponent* newComponent(const std::string& componentType, const std::string& componentName);
    void registerComponent(const std::string& name, ComponentLoaderFunc func);
    static ComponentFactory& instanceRef();
    static ComponentFactory* instancePtr();
private:
    std::unordered_map<std::string, ComponentLoaderFunc> m_registeredComponents;
};


#define orComponentFactoryRef ComponentFactory::instanceRef()
#define orComponentFactoryPtr ComponentFactory::instancePtr()

#endif // COMPONENTMANAGER_HPP
