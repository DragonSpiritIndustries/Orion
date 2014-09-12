#include "ResourceManager.hpp"
#include "ScriptEngine.hpp"
#include "CVar.hpp"
#include <physfs.h>
#include <algorithm>

CVar* res_basepath = new CVar("fs_basepath", "data", "Base directory for resources", CVar::Literal, CVar::System | CVar::ReadOnly);
CVar* sv_pure      = new CVar("sv_pure",     "true","In \"pure\" mode only resources in archives are loaded", CVar::Boolean, CVar::System | CVar::Cheat);

ResourceManager::ResourceManager()
{
    orScriptEngineRef.handle()->RegisterObjectType("ResourceManager", 0, asOBJ_REF | asOBJ_NOHANDLE);
    orScriptEngineRef.handle()->RegisterGlobalProperty("ResourceManager orResourceManager", this);
}

bool ResourceManager::initialize(const std::string& executablePath,
                                 const std::string& organizationName,
                                 const std::string& applicationName,
                                 const std::string& archiveExt)
{
    m_executablePath = executablePath;
    m_organizationName = organizationName;
    m_applicationName = applicationName;
    m_archiveExt = archiveExt;

    orConsoleRef.print(orConsoleRef.Info, "Initializing resource manager:\n");
    orConsoleRef.print(orConsoleRef.Info, "arg0 %s\n", m_executablePath.c_str());
    orConsoleRef.print(orConsoleRef.Info, "Organization %s\n", m_organizationName.c_str());
    orConsoleRef.print(orConsoleRef.Info, "Application %s\n", m_applicationName.c_str());
    orConsoleRef.print(orConsoleRef.Info, "ArchiveExtension %s\n", m_archiveExt.c_str());
    if (!PHYSFS_init(m_executablePath.c_str()))
    {
        orConsoleRef.print(orConsoleRef.Info, "Failed to initialize PHYSFS: %s\n", PHYSFS_getLastError());
        return false;
    }

    PHYSFS_setWriteDir(res_basepath->toLiteral().c_str());
    PHYSFS_setSaneConfig(m_organizationName.c_str(), m_executablePath.c_str(), m_archiveExt.c_str(), false, true);
    if (!PHYSFS_mount(res_basepath->toLiteral().c_str(), "/", false))
    {
        orConsoleRef.print(orConsoleRef.Fatal, "Failed to mount basepath");
        return false;
    }

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

    PHYSFS_freeList(listBegin);

    // If the engine is in "pure" mode, unmount fs_basepath, we don't want players loading "unpure" resources
    if (sv_pure->toBoolean())
        PHYSFS_removeFromSearchPath(res_basepath->toLiteral().c_str());

    // Sort files in descending order
    std::sort(archives.begin(), archives.end(), std::less<std::string>());

    for (std::string archive : archives)
    {
        orConsoleRef.print(orConsoleRef.Info, "Mounting archive: %s\n", archive.c_str());
        PHYSFS_mount(archive.c_str(), nullptr, true);
    }

    return true;
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

void ResourceManager::registerResource(ResourceLoader* loader)
{
    if (std::find(m_resourceLoaders.begin(), m_resourceLoaders.end(), loader)  != m_resourceLoaders.end())
        return;

    m_resourceLoaders.push_back(loader);
}

void ResourceManager::removeResource(const std::string& name)
{
    if (m_resources.find(name) == m_resources.end())
        return;

    delete m_resources[name];
    m_resources.erase(name);
}

void ResourceManager::removeResource(IResource* res)
{
    if (m_resources.find(res->path()) == m_resources.end())
        return;

    m_resources.erase(res->path());
    delete res;
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
