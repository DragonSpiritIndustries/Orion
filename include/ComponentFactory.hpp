#ifndef COMPONENTMANAGER_HPP
#define COMPONENTMANAGER_HPP

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
