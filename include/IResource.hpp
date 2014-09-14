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

#ifndef IRESOURCE_HPP
#define IRESOURCE_HPP

#include <vector>
#include <string>
#include "Global.hpp"

class asILockableSharedBool;
class asIScriptObject;
class asIScriptContext;
class IResource
{
public:
    IResource(const std::string& path);

    virtual ~IResource() {}

    int addRef();

    int release();

    void destroyAndRelease();

    asILockableSharedBool* weakRefFlag();


    std::string path() const;

protected:
    std::string m_path;
    asILockableSharedBool* m_weakRefFlag;
    asIScriptContext*      m_scriptContext;
    int m_refCount;
};

#endif // IRESOURCE_HPP
