#include "ObjectManager.hpp"
#include "Object.hpp"
#include <IApplication.hpp>
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

    m_objects.push_back(obj);
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
    std::cout << "Killing objects" << std::endl;
    for (Object* o : m_objects)
    {
        o->onDestroyed();
        delete o;
    }
    std::cout << "done" << std::endl;
}

ObjectManager& ObjectManager::instanceRef()
{
    if (!orApplicationPtr)
    {
        std::cout << "Engine not initialized" << std::endl;
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
        std::cout << "Engine not initialized" << std::endl;
        exit(1);
    }

    if (!m_instance)
        m_instance = std::shared_ptr<ObjectManager>(new ObjectManager);

    return m_instance.get();
}
