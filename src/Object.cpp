#include "Object.hpp"
#include <algorithm>


Object::Object(const std::string& name)
    : m_name(name)
{
}

std::string Object::name() const
{
    return m_name;
}

void Object::setName(const std::string& name)
{
    m_name = name;
}

void Object::addChild(Object* child)
{
    if (child)
        m_children.push_back(child);
}

void Object::removeChild(const std::string& name)
{
    Iterator index = m_children.begin();

    for (;index != m_children.end(); index++)
        if (!((Object*)*index)->name().compare(name))
            break;

    if (index != m_children.end())
    {
        m_children.erase(index);
        delete *index;
    }
}

void Object::removeChild(int index)
{
    if (index < 0 || index >= (int)m_children.size())
        return;

    delete m_children[index];
    m_children.erase(m_children.begin()+index);
}

void Object::removeChild(Object* child)
{
    Iterator index = std::find(m_children.begin(), m_children.end(), child);

    if (index != m_children.end())
    {
        delete child;
        m_children.erase(index);
    }
}

Object* Object::takeChild(const std::string& name)
{
    Iterator index = m_children.begin();
    for (;index != m_children.end(); index++)
        if (!((Object*)*index)->name().compare(name))
            break;

    if (index != m_children.end())
    {
        m_children.erase(index);
        return *index;
    }

    return nullptr;
}

Object* Object::takeChild(int index)
{
    if (index < 0 || index >= (int)m_children.size())
        return nullptr;

    Object* ret = m_children.at(index);
    m_children.erase(m_children.begin()+index);
    return ret;
}
