#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <unordered_map>
#include <Global.hpp>
#include <IResource.hpp>
#include <memory>

typedef IResource* (*ResourceLoader)(const std::string&);

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

    void registerResource(ResourceLoader loader);

    static ResourceManager& instanceRef();
    static ResourceManager* instancePtr();
private:
    std::unordered_map<std::string, IResource*> m_resources;
    std::vector<ResourceLoader> m_resourceLoaders;
    std::string m_executablePath;
    std::string m_organizationName;
    std::string m_applicationName;
    std::string m_archiveExt;
};

struct ResourceRegistrator
{
    ResourceRegistrator(ResourceLoader loader)
    {
        ResourceManager::instanceRef().registerResource(loader);
    }
};


#ifndef DEFINE_RESOURCE
#define DEFINE_RESOURCE() \
    static const ResourceRegistrator __DO_NOT_USE_p_resourceRegistrator
#endif

#ifndef REGISTER_RESOURCE
#define REGISTER_RESOURCE(Class, Loader) \
    const ResourceRegistrator Class::__DO_NOT_USE_p_resourceRegistrator = ResourceRegistrator(Class::Loader)
#endif


#define orResourceManagerRef ResourceManager::instanceRef()
#define orResourceManagerPtr ResourceManager::instancePtr()

#include "ResourceManager.inl"
#endif // RESOURCEMANAGER_HPP
