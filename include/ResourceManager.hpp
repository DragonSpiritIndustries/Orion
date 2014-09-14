// This file is part of Orion.
//
// Orion is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Orion is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Orion.  If not, see <http://www.gnu.org/licenses/>

#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include <unordered_map>
#include <Global.hpp>
#include <IResource.hpp>
#include <Athena/Utility.hpp>
#include <memory>

#ifndef SCRIPTENGINE_HPP
#include "ScriptEngine.hpp"
#endif

typedef IResource* (*ResourceLoaderFunc)(const std::string&);

struct ResourceLoader
{
    std::vector<std::string> SupportedExtensions;
    ResourceLoaderFunc LoaderFunc;
};

class ResourceManager
{
public:
    ResourceManager();
    bool initialize(const std::string& executablePath,
                    const std::string& organizationName= std::string(orDEFAULT_ORGANIZATION_NAME),
                    const std::string& applicationName = std::string(orDEFAULT_APPLICATION_NAME),
                    const std::string& archiveExt      = std::string(orDEFAULT_ARCHIVE_EXTENSION));

    void shutdown();
    // Either stored in the data directory, or stored within a pack;
    template <class T>
    T* loadResource(const std::string& resourceName);

    void registerResource(ResourceLoader* loader);

    void removeResource(const std::string& name);
    void removeResource(IResource* res);

    static ResourceManager& instanceRef();
    static ResourceManager* instancePtr();

private:
    std::unordered_map<std::string, IResource*> m_resources;
    std::vector<ResourceLoader*> m_resourceLoaders;
    std::string m_executablePath;
    std::string m_organizationName;
    std::string m_applicationName;
    std::string m_archiveExt;
};

#define orResourceManagerRef ResourceManager::instanceRef()
#define orResourceManagerPtr ResourceManager::instancePtr()

//! \brief REGISTER_RESOURCE is used to register a particular resource to the resource manager
//! \param Class The registrant class, This is used to create a "secret" object which does the actually registration
//! \param Extensions The supported extensions for the target resource, multiple extensions are denoted by a space
//! \param Loader The function that does the actual loading, MUST be a static member of Class
#ifndef REGISTER_RESOURCE
#define REGISTER_RESOURCE(Class, Extensions, Loader) \
struct hidden_resourceRegistration##Class \
{   hidden_resourceRegistration##Class() \
    {   m_loader = new ResourceLoader; \
        m_loader->SupportedExtensions = Athena::utility::split(Extensions, ' '); \
        m_loader->LoaderFunc = Class::Loader; \
        orResourceManagerRef.registerResource(m_loader); \
    } ~hidden_resourceRegistration##Class() \
    { delete m_loader; } \
private:    ResourceLoader* m_loader; };\
static hidden_resourceRegistration##Class __hidden_resourceRegistration##Class
#endif

#include "ResourceManager.inl"
#endif // RESOURCEMANAGER_HPP
