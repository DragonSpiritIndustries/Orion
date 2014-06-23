#include "ObjectManager.hpp"
#include "Object.hpp"
#include <IApplication.hpp>
#include <Athena/Utility.hpp>
#include <algorithm>
#include <iostream>

std::shared_ptr<ObjectManager> ObjectManager::m_instance = nullptr;


ObjectManager::~ObjectManager()
{
}

void ObjectManager::addObject(Object* obj)
{
    if (std::find(m_objects.begin(), m_objects.end(), obj) != m_objects.end())
        return;

    orDebug("old %i", m_objects.size());
    m_objects.push_back(obj);
    orDebug(" new %i\n", m_objects.size());
}

void ObjectManager::removeObject(Object* obj)
{
    Iterator iter = std::find(m_objects.begin(), m_objects.end(), obj);
    if (iter != m_objects.end())
    {
        m_objects.erase(iter);
        delete obj;
    }
}

void ObjectManager::removeObject(const std::string& name)
{
    Iterator iter = std::find_if(m_objects.begin(), m_objects.end(), [&name](Object* o)->bool{return o->name() == name;});
    if (iter != m_objects.end())
    {
        m_objects.erase(iter);
        delete *iter;
    }
}

void ObjectManager::shutdown()
{
    orDebug("Killing %i objects\n", m_objects.size());

    orForeach (Object* o _in_ m_objects)
    {
        o->onDestroyed();
        delete o;
    }
}

ObjectManager& ObjectManager::instanceRef()
{
    if (!orApplicationPtr)
    {
        orDebug("Engine not initialized\n");
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
        orDebug("Engine not initialized\n");
        exit(1);
    }

    if (!m_instance)
        m_instance = std::shared_ptr<ObjectManager>(new ObjectManager);

    return m_instance.get();
}

void ObjectManager::draw(IApplication* app)
{
    orForeach(Object* o _in_ m_objects)
    {
        o->draw(app);
    }

    orApplicationRef.drawDebugText(Athena::utility::sprintf("Object count %i", m_objects.size()), 16, 16);
}
