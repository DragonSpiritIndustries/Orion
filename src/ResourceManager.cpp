#include "ResourceManager.hpp"
#include "IResource.hpp"
#include <physfs.h>
#include <algorithm>

std::unordered_map<std::string, ResourceLoader> ResourceManager::m_resourceLoaders;

ResourceManager::ResourceManager(const std::string& executablePath,
                                 const std::string& organizationName,
                                 const std::string& applicationName,
                                 const std::string& archiveExt)
    : m_executablePath(executablePath),
      m_organizationName(organizationName),
      m_applicationName(applicationName),
      m_archiveExt(archiveExt)
{
    orDebug("Initializing resource manager:\n");
    orDebug("arg0 value       = %s\n", executablePath.c_str());
    orDebug("Organization     = %s\n", organizationName.c_str());
    orDebug("Application      = %s\n", applicationName.c_str());
    orDebug("ArchiveExtension = %s\n", archiveExt.c_str());
    if (!PHYSFS_init(m_executablePath.c_str()))
    {
        orDebug("Failed to initialize PHYSFS: %s\n", PHYSFS_getLastError());
        return;
    }

    PHYSFS_setSaneConfig(m_organizationName.c_str(), m_executablePath.c_str(), m_executablePath.c_str(), false, true);

    char** listBegin = PHYSFS_enumerateFiles("/");
    std::vector<std::string> archives;
    for (char** file = listBegin; *file != NULL; file++)
    {
        // We don't support sub-directories for pak files
        if (PHYSFS_isDirectory(*file))
            continue;

        std::string fileName(*file);
        fileName.insert(0, std::string(PHYSFS_getRealDir(*file)) + "/");
        std::string extension = fileName.substr(fileName.find_last_of(".") + 1, fileName.length() - (fileName.find_last_of(".") + 1));
        if ((!extension.compare(m_archiveExt)))
            archives.push_back(fileName);
    }

    // Sort files in descending order
    std::sort(archives.begin(), archives.end(), std::less<std::string>());

    for (std::string archive : archives)
    {
        orDebug("Mounting archive: %s\n", archive.c_str());
        PHYSFS_mount(archive.c_str(), NULL, 0);
    }
}

void ResourceManager::shutdown()
{
    orForeach(std::pair<std::string, IResource*> res _in_ m_resources)
    {
        delete res.second;
        res.second = nullptr;
    }

    m_resources.clear();
    PHYSFS_deinit();
}

IResource* ResourceManager::loadResource(const std::string& resourceName, const std::string& resourceType)
{
    if (m_resources.find(resourceName) != m_resources.end())
    {
        orDebug("Using cached resource %s of type %s\n", resourceName.c_str(), m_resources[resourceName]->resourceType().c_str());
        return m_resources[resourceName];
    }

    orForeach (std::pair<std::string, ResourceLoader> loader _in_ m_resourceLoaders)
    {
        if (resourceType != std::string() && loader.first.compare(resourceType))
            continue;

        IResource* res = loader.second(resourceName);
        if (res)
        {
            m_resources[resourceName] = res;
            return res;
        }
    }

    orDebug("Unable to find resource with name %s\n", resourceName.c_str());
    return nullptr;
}

void ResourceManager::registerResource(const std::string& resourceType, ResourceLoader loader)
{
    if (m_resourceLoaders.find(resourceType) != m_resourceLoaders.end())
        return;

    m_resourceLoaders[resourceType] = loader;
    orDebug("Registered loader for \"%s\"\n", resourceType.c_str());
}
