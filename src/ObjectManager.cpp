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

#include "ObjectManager.hpp"
#include "Object.hpp"
#include "ApplicationBase.hpp"
#include "ScriptEngine.hpp"
#include <Athena/Utility.hpp>
#include <algorithm>
#include <iostream>

std::shared_ptr<ObjectManager> ObjectManager::m_instance = nullptr;

int ObjectManager::objectId = 0;

extern void registerObject(asIScriptEngine* engine);

ObjectManager::ObjectManager()
{
}

ObjectManager::~ObjectManager()
{
}

void ObjectManager::addObject(Object* obj)
{
    if (std::find(m_objects.begin(), m_objects.end(), obj) != m_objects.end())
        return;

    obj->setId(objectId);
    m_objects.push_back(obj);
    objectId++;
}

void ObjectManager::removeObject(Object* obj)
{
    Iterator iter = std::find(m_objects.begin(), m_objects.end(), obj);
    if (iter != m_objects.end())
    {
        m_objects.erase(iter);
        obj->onDestroyed();
        delete obj;
        objectId--;
    }
}

void ObjectManager::removeObject(const std::string& name)
{
    Iterator iter = std::find_if(m_objects.begin(), m_objects.end(), [&name](Object* o)->bool{return o->name() == name;});
    if (iter != m_objects.end())
    {
        m_objects.erase(iter);
        delete *iter;
        objectId--;
    }
}

Object* ObjectManager::takeObject(const std::string& name)
{
    Iterator iter = std::find_if(m_objects.begin(), m_objects.end(), [&name](Object* o)->bool{return o->name() == name;});

    if (iter != m_objects.end())
    {
        m_objects.erase(iter);
        objectId--;
        return *iter;
    }
    return nullptr;
}

Object* ObjectManager::object(const std::string& name)
{
    Iterator iter = std::find_if(m_objects.begin(), m_objects.end(), [&name](Object* o)->bool{return o->name() == name;});
    if (iter == m_objects.end())
        return nullptr;

    return *iter;
}

void ObjectManager::initialize()
{
    if (orScriptEngineRef.handle())
    {
        registerObject(orScriptEngineRef.handle());
        orScriptEngineRef.handle()->RegisterInterface("IController");
        orScriptEngineRef.handle()->RegisterObjectType("ObjectManager", 0, asOBJ_REF | asOBJ_NOHANDLE);
        orScriptEngineRef.handle()->RegisterGlobalProperty("ObjectManager orObjectManager", this);
        orScriptEngineRef.handle()->RegisterObjectMethod("ObjectManager",
                                                         "Object @+ objectByName(const string &in name)",
                                                         asMETHOD(ObjectManager, object),
                                                         asCALL_THISCALL);
    }
}

void ObjectManager::shutdown()
{
    orConsoleRef.print(orConsoleRef.Info, "Killing %i objects\n", (int)m_objects.size());

    while (m_objects.size() > 0)
    {
        Object* o = m_objects.back();
        removeObject(o);
    }
}

ObjectManager& ObjectManager::instanceRef()
{
    if (!orApplicationPtr)
    {
        orConsoleRef.print(orConsoleRef.Fatal, "Engine not initialized\n");
        exit(1);
    }

    if (!m_instance)
        m_instance = std::shared_ptr<ObjectManager>(new ObjectManager);

    return *m_instance.get();
}

ObjectManager* ObjectManager::instancePtr()
{
    if (!orApplicationPtr)
    {
        orConsoleRef.print(orConsoleRef.Fatal, "Engine not initialized\n");
        exit(1);
    }

    if (!m_instance)
        m_instance = std::shared_ptr<ObjectManager>(new ObjectManager);

    return m_instance.get();
}

void ObjectManager::draw()
{
    orForeach(Object* o _in_ m_objects)
    {
        o->onDraw();
    }

    orApplicationRef.drawDebugText(Athena::utility::sprintf("Object count %i", m_objects.size()), 16, 16);
}
