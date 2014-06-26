#ifndef ICOMPONENT_HPP
#define ICOMPONENT_HPP

#include <string>
#include "ComponentManager.hpp"

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
    Object* m_owner;
    std::string m_name;
    asILockableSharedBool* m_weakRefFlag;
    int m_refCount;
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

#endif // ICOMPONENT_HPP
