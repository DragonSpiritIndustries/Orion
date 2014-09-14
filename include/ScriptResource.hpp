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

#ifndef SCRIPTRESOURCE_HPP
#define SCRIPTRESOURCE_HPP

#include "ResourceManager.hpp"
#include "angelscript/addons/scriptbuilder.h"

class ScriptResource : public IResource
{
public:
    ScriptResource(const std::string& path);
    virtual ~ScriptResource();
    asIScriptModule* module();

    asIScriptFunction* functionByDecl(const std::string& decl);
    asIScriptFunction* functionByName(const std::string& name);
    asIScriptFunction* functionByIndex(int id);

    bool loadData(const std::string &path);
    static IResource* load(const std::string& path);
private:
    CScriptBuilder m_builder;
    bool           m_valid;
};

#endif // SCRIPTRESOURCE_HPP
