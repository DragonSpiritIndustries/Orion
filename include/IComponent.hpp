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

#ifndef ICOMPONENT_HPP
#define ICOMPONENT_HPP

#include <string>
#include "ComponentFactory.hpp"
class Object;
class asILockableSharedBool;
class IComponent
{
public:
    IComponent(const std::string& name);
    virtual ~IComponent();
    virtual std::string name() const;

    void setOwner(Object* owner);
    Object* owner();
    virtual void setName(const std::string& name);

protected:
    std::string m_name;
    Object* m_owner;
};


#ifndef REGISTER_COMPONENT
#define REGISTER_COMPONENT(Class, Name, Loader) \
struct hidden_componentRegistration##Class \
{ \
    hidden_componentRegistration##Class() \
    { \
        orComponentFactoryRef.registerComponent(Name, Class::Loader); \
    }\
};\
static hidden_componentRegistration##Class __hidden_componentRegistration##Class
#endif

template<class A=IComponent, class B=IComponent>
B* refCast(A* a)
{
    if (!a)
        return nullptr;

    B* b = dynamic_cast<B*>(a);
    return b;
}

#endif // ICOMPONENT_HPP
