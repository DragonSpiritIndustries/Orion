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

#ifndef CVARMANAGER_HPP
#define CVARMANAGER_HPP

#include <unordered_map>
#include <vector>
#include <fstream>
#include <tinyxml.h>

class CVar;

class CVarManager
{
public:
    CVarManager();
    ~CVarManager();

    void initialize();
    bool registerCVar(CVar* cvar);
    void requestSerialization(CVar* cvar);

    CVar* findCVar(const std::string& name);

    std::vector<CVar*> archivedCVars() const;
    std::vector<CVar*> bindings() const;
    std::vector<CVar*> cvars() const;

    void writeToFile(const std::string&  file=std::string());
    void loadFromFile(const std::string& file=std::string());

    static CVarManager& instanceRef();
    static CVarManager* instancePtr();
private:
    bool suppressDeveloper();
    void restoreDeveloper(bool oldDeveloper);

    std::unordered_map<std::string, CVar*> m_cvars;
    TiXmlDocument m_config;

};

#define orCVarManagerRef CVarManager::instanceRef()
#define orCVarManagerPtr CVarManager::instancePtr()
#endif // CVARMANAGER_HPP
