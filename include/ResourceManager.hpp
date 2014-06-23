#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <unordered_map>
#include <Global.hpp>

class IResource;

typedef IResource* (*ResourceLoader)(const std::string&);

class ResourceManager
{
public:
    ResourceManager(const std::string& executablePath,
                    const std::string& organizationName= std::string(orDEFAULT_ORGANIZATION_NAME),
                    const std::string& applicationName = std::string(orDEFAULT_APPLICATION_NAME),
                    const std::string& archiveExt      = std::string(orDEFAULT_ARCHIVE_EXTENSION));

    void shutdown();
    // Either stored in the data directory, or stored within a pack;
    IResource* loadResource(const std::string& resourceName, const std::string& resourceType = std::string());

    static void registerResource(const std::string& resourceType, ResourceLoader loader);
private:
    std::unordered_map<std::string, IResource*> m_resources;
    static std::unordered_map<std::string, ResourceLoader> m_resourceLoaders;
    std::string m_executablePath;
    std::string m_organizationName;
    std::string m_applicationName;
    std::string m_archiveExt;
};


struct ResourceRegistrator
{
    ResourceRegistrator(const std::string& resourceType, ResourceLoader loader)
    {
        ResourceManager::registerResource(resourceType, loader);
    }
};


#ifndef DEFINE_RESOURCE
#define DEFINE_RESOURCE() \
    static const std::string m_resourceType; \
    static const ResourceRegistrator __DO_NOT_USE_p_resourceRegistrator; \
    public: \
        const std::string resourceType() { return m_resourceType; } \
    private:
#endif

#ifndef REGISTER_RESOURCE
#define REGISTER_RESOURCE(Class, ResourceType, Loader) \
    const std::string Class::m_resourceType = ResourceType; \
    const ResourceRegistrator Class::__DO_NOT_USE_p_resourceRegistrator = ResourceRegistrator(Class::m_resourceType, Class::Loader)
#endif

#endif // RESOURCEMANAGER_HPP
