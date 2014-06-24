#include "ResourceManager.hpp"
#include <physfs.h>
#include <algorithm>

ResourceManager::ResourceManager()
{
}

void ResourceManager::initialize(const std::string& executablePath,
                                 const std::string& organizationName,
                                 const std::string& applicationName,
                                 const std::string& archiveExt)
{
    m_executablePath = executablePath;
    m_organizationName = organizationName;
    m_applicationName = applicationName;
    m_archiveExt = archiveExt;

    orDebug("Initializing resource manager:\n");
    orDebug("arg0 value       = %s\n", m_executablePath.c_str());
    orDebug("Organization     = %s\n", m_organizationName.c_str());
    orDebug("Application      = %s\n", m_applicationName.c_str());
    orDebug("ArchiveExtension = %s\n", m_archiveExt.c_str());
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

void ResourceManager::registerResource(ResourceLoader loader)
{
    if (std::find(m_resourceLoaders.begin(), m_resourceLoaders.end(), loader)  != m_resourceLoaders.end())
        return;

    m_resourceLoaders.push_back(loader);
}

ResourceManager& ResourceManager::instanceRef()
{
    return *ResourceManager::instancePtr();
}

ResourceManager* ResourceManager::instancePtr()
{
    static std::shared_ptr<ResourceManager> instance = std::shared_ptr<ResourceManager>(new ResourceManager);
    return instance.get();
}
