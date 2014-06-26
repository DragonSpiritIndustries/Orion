#include "ComponentManager.hpp"
#include "IComponent.hpp"
#include "Console.hpp"
#include "ScriptEngine.hpp"
#include <memory>

ComponentFactory::ComponentFactory()
{
    orConsoleRef.print(orConsoleRef.Info, "ComponentFactory initialized");
    orScriptEngineRef.handle()->RegisterObjectType("Component", 0, asOBJ_REF);
    orScriptEngineRef.handle()->RegisterObjectBehaviour("Component", asBEHAVE_ADDREF, "void f()", asMETHOD(IComponent, addRef), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectBehaviour("Component", asBEHAVE_RELEASE, "void f()", asMETHOD(IComponent, release), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectBehaviour("Component", asBEHAVE_GET_WEAKREF_FLAG, "int &f()", asMETHOD(IComponent, weakRefFlag), asCALL_THISCALL);
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