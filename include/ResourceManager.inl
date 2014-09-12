#include "Global.hpp"
#include "ScriptEngine.hpp"
#include "Console.hpp"
#include "algorithm"
#ifndef RESOURCEMANAGER_HPP
#include "ResourceManager.hpp"
#endif

template <class T>
T* ResourceManager::loadResource(const std::string& resourceName)
{
    if (m_resources.find(resourceName) != m_resources.end())
    {
        T* test = dynamic_cast<T*>(m_resources[resourceName]);
        if (test)
            return test;

        orConsoleRef.print(orConsoleRef.Warning, "Cast failed\n");
    }

    orForeach (ResourceLoader* loader _in_ m_resourceLoaders)
    {
        std::string extension = resourceName.substr(resourceName.find_last_of(".") + 1);
        if (std::find(loader->SupportedExtensions.begin(), loader->SupportedExtensions.end(), extension) != loader->SupportedExtensions.end())
        {
            IResource* res = loader->LoaderFunc(resourceName);
            if (res)
            {
                m_resources[resourceName] = res;
                T* test = dynamic_cast<T*>(res);
                if (test)
                    return test;

                orConsoleRef.print(orConsoleRef.Warning, "Cast failed\n");
            }
        }
    }

    orConsoleRef.print(orConsoleRef.Warning, "Unable to find resource with name %s\n", resourceName.c_str());
    return nullptr;
}
