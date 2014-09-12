#include "IResource.hpp"
#include "ResourceManager.hpp"

IResource::IResource(const std::string& path)
    : m_path(path),
      m_refCount(1),
      m_weakRefFlag(nullptr)
{
}

int IResource::addRef()
{
    return ++m_refCount;
}

int IResource::release()
{
    if (--m_refCount == 0)
    {
        orResourceManagerRef.removeResource(this);
        return 0;
    }

    return m_refCount;
}

void IResource::destroyAndRelease()
{
    release();
}

asILockableSharedBool* IResource::weakRefFlag()
{
    if (!m_weakRefFlag)
        m_weakRefFlag = asCreateLockableSharedBool();

    return m_weakRefFlag;
}

std::string IResource::path() const
{
    return m_path;
}
