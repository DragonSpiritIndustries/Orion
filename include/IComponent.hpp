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

    int  addRef();
    int  release();
    void destroyAndRelease();
    asILockableSharedBool* weakRefFlag();

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
