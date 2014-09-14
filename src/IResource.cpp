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
