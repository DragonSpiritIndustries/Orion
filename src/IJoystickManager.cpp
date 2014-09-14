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

#include "IJoystickManager.hpp"
#include "ApplicationBase.hpp"
#include "ScriptEngine.hpp"
#include <physfs.h>
#include <tinyxml.h>

IJoystickManager::IJoystickManager()
{
    orApplicationPtr->updateSignal().connect<IJoystickManager, &IJoystickManager::onUpdate>(this);

    if (orScriptEngineRef.handle())
    {
        orScriptEngineRef.handle()->RegisterObjectType("JoystickManager", 0, asOBJ_REF | asOBJ_NOHANDLE);
        orScriptEngineRef.handle()->RegisterGlobalProperty("JoystickManager orJoystickManager", this);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "bool buttonPressed(int joy, int button)",
                                                         asMETHOD(IJoystickManager, buttonPressed),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "bool buttonPressed(const int joy, const int button) const",
                                                         asMETHOD(IJoystickManager, buttonPressed),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "bool buttonReleased(int joy, int button)",
                                                         asMETHOD(IJoystickManager, buttonReleased),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "bool buttonReleased(const int joy, const int button) const",
                                                         asMETHOD(IJoystickManager, buttonReleased),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "float axisPosition(int joy, int axis)",
                                                         asMETHOD(IJoystickManager, axisPosition),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "float axisPosition(const int joy, const int axis) const",
                                                         asMETHOD(IJoystickManager, axisPosition),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "string name(int joy)",
                                                         asMETHOD(IJoystickManager, name),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "string name(const int joy) const",
                                                         asMETHOD(IJoystickManager, name),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "void motorOn(int joy)",
                                                         asMETHOD(IJoystickManager, motorOn),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "void motorOn(const int joy) const",
                                                         asMETHOD(IJoystickManager, motorOn),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "void motorOff(int joy)",
                                                         asMETHOD(IJoystickManager, motorOff),
                                                         asCALL_THISCALL);
        orScriptEngineRef.handle()->RegisterObjectMethod("JoystickManager",
                                                         "void motorOff(const int joy) const",
                                                         asMETHOD(IJoystickManager, motorOff),
                                                         asCALL_THISCALL);
    }
}

void IJoystickManager::onUpdate(float)
{
    for (int i = 0; i < (int)m_releasedButtons.size(); i++)
    {
        for (std::pair<const int, bool>& btn : m_releasedButtons[i])
        {
            btn.second = false;
        }
    }
}

bool IJoystickManager::hasMetaData(const std::string& joyName)
{
    std::vector<JoyMetaData>::const_iterator iter = std::find_if(m_metaData.cbegin(),
                                                                 m_metaData.cend(),
                                                                 [&joyName](JoyMetaData jmd)->bool{return jmd.name == joyName;});
    return iter != m_metaData.cend();
}

void IJoystickManager::loadMetaData(const std::string& joyName)
{
    // If it's already loaded there is no need to load it again
    if (hasMetaData(joyName))
        return;

    // Iterate through each controller definition and check the name
    // TODO: Load all controller definitions at start?
    char** fileList = PHYSFS_enumerateFiles("controller_definitions");
    // cache the start of the list
    char** listBeg = fileList;
    TiXmlDocument document;

    char* xmlData = nullptr;
    for (; *fileList != nullptr; fileList++)
    {
        std::string fileName(*fileList);
        std::string tmp(fileName);
        Athena::utility::tolower(tmp);
        std::string::size_type iter = tmp.find_last_of(".xml");
        if (iter == std::string::npos || iter != tmp.size() - 1)
            continue;

        fileName = "controller_definitions/" + fileName;
        PHYSFS_File* file = PHYSFS_openRead(fileName.c_str());
        if (file)
        {
            xmlData = new char[PHYSFS_fileLength(file)];
            PHYSFS_read(file, xmlData, 1, PHYSFS_fileLength(file));
            document.Parse(xmlData);
            TiXmlElement* elem = document.FirstChildElement();
            if (elem->Attribute("name") == joyName)
                break;

            delete[] xmlData;
            xmlData = nullptr;
        }
    }
    fileList = listBeg;
    PHYSFS_freeList(fileList);

    // Do we have valid data?
    if (xmlData)
    {
        TiXmlElement* root = document.FirstChildElement();
        JoyMetaData jmd;
        jmd.name = root->Attribute("name");
        TiXmlElement* btnInfo = root->FirstChildElement();
        while (btnInfo != nullptr)
        {
            JoyButtonInfo jbi;
            btnInfo->Attribute("id", &jbi.id);
            jbi.name  = btnInfo->Attribute("name");
            jbi.image = btnInfo->Attribute("img");
            jmd.buttonInfo.push_back(jbi);
            btnInfo = btnInfo->NextSiblingElement();
        }
        m_metaData.push_back(jmd);
        delete[] xmlData;
    }
}
