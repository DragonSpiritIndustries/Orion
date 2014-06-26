#include "IComponent.hpp"
#include "angelscript/angelscript.h"
#include "Object.hpp"

IComponent::IComponent(const std::string& name)
    : m_name(name),
      m_weakRefFlag(nullptr),
      m_refCount(1)
{
}

IComponent::~IComponent()
{
    if (m_weakRefFlag)
    {
        m_weakRefFlag->Set(true);
        m_weakRefFlag->Release();
    }
}

std::string IComponent::name() const
{
    return m_name;
}

void IComponent::setOwner(Object* owner)
{
    m_owner = owner;
}

Object* IComponent::owner()
{
    return m_owner;
}

void IComponent::setName(const std::string& name)
{
    m_name = name;
}

int IComponent::addRef()
{
    return ++m_refCount;
}

int IComponent::release()
{
    if (--m_refCount == 0)
    {
        if (m_owner)
            m_owner->removeComponent(this);

        delete this;
        return 0;
    }

    return m_refCount;
}

void IComponent::destroyAndRelease()
{
    release();
}

asILockableSharedBool* IComponent::weakRefFlag()
{
    if (!m_weakRefFlag)
        m_weakRefFlag = asCreateLockableSharedBool();

    return m_weakRefFlag;
}
