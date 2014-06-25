#include "Global.hpp"
#include "Console.hpp"

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

    orForeach (ResourceLoader loader _in_ m_resourceLoaders)
    {
        IResource* res = loader(resourceName);
        if (res)
        {
            m_resources[resourceName] = res;
            T* test = dynamic_cast<T*>(res);
            if (test)
                return test;

            orConsoleRef.print(orConsoleRef.Warning, "Cast failed\n");
        }
    }

    orConsoleRef.print(orConsoleRef.Warning, "Unable to find resource with name %s\n", resourceName.c_str());
    return nullptr;
}
