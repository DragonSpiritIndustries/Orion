#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <unordered_map>
#include <Global.hpp>
#include <IResource.hpp>
#include <memory>

#ifndef SCRIPTENGINE_HPP
#include "ScriptEngine.hpp"
#endif

typedef IResource* (*ResourceLoaderFunc)(const std::string&);

class ResourceManager
{
public:
    ResourceManager();
    void initialize(const std::string& executablePath,
                    const std::string& organizationName= std::string(orDEFAULT_ORGANIZATION_NAME),
                    const std::string& applicationName = std::string(orDEFAULT_APPLICATION_NAME),
                    const std::string& archiveExt      = std::string(orDEFAULT_ARCHIVE_EXTENSION));

    void shutdown();
    // Either stored in the data directory, or stored within a pack;
    template <class T>
    T* loadResource(const std::string& resourceName);

    void registerResource(ResourceLoaderFunc loader);

    void removeResource(const std::string& name);
    void removeResource(IResource* res);

    static ResourceManager& instanceRef();
    static ResourceManager* instancePtr();
private:
    std::unordered_map<std::string, IResource*> m_resources;
    std::vector<ResourceLoaderFunc> m_resourceLoaders;
    std::string m_executablePath;
    std::string m_organizationName;
    std::string m_applicationName;
    std::string m_archiveExt;
};

#define orResourceManagerRef ResourceManager::instanceRef()
#define orResourceManagerPtr ResourceManager::instancePtr()

#ifndef REGISTER_RESOURCE
#define REGISTER_RESOURCE(Class, Loader) \
struct hidden_resourceRegistration##Class \
{ \
    hidden_resourceRegistration##Class() \
    { \
        orResourceManagerRef.registerResource(Class::Loader); \
    }\
};\
static hidden_resourceRegistration##Class __hidden_resourceRegistration##Class
#endif

#ifndef REGISTER_SCRIPT_FUNCTION
#define REGISTER_SCRIPT_FUNCTION(Class, Function) \
struct hidden_scriptRegistration##Class \
{ \
    hidden_scriptRegistration##Class() \
    { \
        Function(); \
    } \
}; \
static hidden_scriptRegistration##Class __hidden_scriptRegistration##Class
#endif

#include "ResourceManager.inl"
#endif // RESOURCEMANAGER_HPP
