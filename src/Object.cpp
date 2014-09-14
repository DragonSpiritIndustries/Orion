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

#include "Object.hpp"
#include "ObjectManager.hpp"
#include "Global.hpp"
#include "ScriptEngine.hpp"
#include "ResourceManager.hpp"
#include "ScriptResource.hpp"
#include "IComponent.hpp"
#include "ComponentFactory.hpp"
#include <algorithm>


Object::Object(const std::string& name)
    : m_name(name),
      m_refCount(1),
      m_weakRefFlag(nullptr),
      m_scriptContext(nullptr),
      m_state(0)
{
    m_script = orResourceManagerRef.loadResource<ScriptResource>(std::string("scripts/")+ this->name() + ".as");
    if (m_script)
        m_scriptContext = orScriptEngineRef.handle()->CreateContext();

    onCreate();
}

Object::~Object()
{
    if (m_weakRefFlag)
    {
        m_weakRefFlag->Set(true);
        m_weakRefFlag->Release();
    }

    // remove ourselves from the manager
    orObjectManagerRef.takeObject(name());

    orForeach(Object* o : m_children)
    {
        orObjectManagerRef.removeObject(o);
    }

    orForeach(IComponent* c : m_components)
    {
        delete c;
    }

    if (m_scriptContext)
        m_scriptContext->Release();
    m_children.clear();
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

void Object::onCreate()
{
    if (m_scriptContext && m_script)
    {
        asIScriptFunction* destroyedFunc = m_script->functionByName("onCreate");
        if(destroyedFunc)
        {
            m_scriptContext->Prepare(destroyedFunc);
            m_scriptContext->SetArgObject(0, this);
            m_scriptContext->Execute();
        }
    }
}

void Object::onDestroyed()
{
    if (m_scriptContext && m_script)
    {
        asIScriptFunction* destroyedFunc = m_script->functionByName("onDestroyed");
        if(destroyedFunc)
        {
            m_scriptContext->Prepare(destroyedFunc);
            m_scriptContext->SetArgObject(0, this);
            m_scriptContext->Execute();
        }
    }
}

void Object::onDraw()
{
    if (m_scriptContext && m_script)
    {
        asIScriptFunction* destroyedFunc = m_script->functionByName("onDraw");
        if(destroyedFunc)
        {
            m_scriptContext->Prepare(destroyedFunc);
            m_scriptContext->SetArgObject(0, this);
            m_scriptContext->Execute();
        }
    }
}

void Object::setPosition(float x, float y)
{
    setPosition(Vector2f(x, y));
}

void Object::setPosition(const Vector2f& position)
{
    m_position = position;
}

Vector2f Object::position() const
{
    return m_position;
}

void Object::move(float x, float y)
{
    move(Vector2f(x, y));
}

void Object::move(const Vector2f& amount)
{
    m_position += amount;
}

int Object::state()
{
    return m_state;
}

void Object::setState(int state)
{
    m_state = state;
}

void Object::onThink(float delta)
{
    if (m_scriptContext && m_script)
    {
        asIScriptFunction* thinkFunc = m_script->functionByName("onThink");
        if(thinkFunc)
        {
            m_scriptContext->Prepare(thinkFunc);
            m_scriptContext->SetArgObject(0, this);
            m_scriptContext->SetArgFloat(1, delta);
            m_scriptContext->Execute();
        }
    }
}

void Object::onUpdate(float delta)
{
    if (orConsoleRef.isOpen())
        return;

    onThink(delta);

    if (m_scriptContext && m_script)
    {
        asIScriptFunction* updateFunc = m_script->functionByName("onUpdate");
        if(updateFunc)
        {
            m_scriptContext->Prepare(updateFunc);
            m_scriptContext->SetArgObject(0, this);
            m_scriptContext->SetArgFloat(1, delta);
            m_scriptContext->Execute();
        }
    }
}

int Object::addRef()
{
    return ++m_refCount;
}

int Object::release()
{
    if (--m_refCount == 0)
    {
        delete this;
        return 0;
    }

    return m_refCount;
}

void Object::destroyAndRelease()
{
    release();
}

asILockableSharedBool* Object::weakRefFlag()
{
    if (!m_weakRefFlag)
        m_weakRefFlag = asCreateLockableSharedBool();

    return m_weakRefFlag;
}

IComponent* Object::addComponent(const std::string& type, const std::string& componentName)
{
    std::vector<IComponent*>::iterator  iter = std::find_if(m_components.begin(), m_components.end(), [&componentName](IComponent* c)->bool{return c->name()  == componentName;});
    if (iter != m_components.end())
        return nullptr;

    IComponent* ret = orComponentFactoryRef.newComponent(type, componentName);
    if (ret)
    {
        ret->setOwner(this);
        m_components.push_back(ret);
        return ret;
    }

    return nullptr;
}

void Object::removeComponent(IComponent* component)
{
    std::vector<IComponent*>::iterator  iter = std::find(m_components.begin(), m_components.end(), component);

    if (iter == m_components.end())
        return;

    m_components.erase(iter);
}

void registerObject(asIScriptEngine* engine)
{
    static const std::string name="Object";
    engine->RegisterObjectType(name.c_str(), 0, asOBJ_REF);
    engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_ADDREF, "void f()", asMETHOD(Object, addRef), asCALL_THISCALL);
    engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_RELEASE, "void f()", asMETHOD(Object, release), asCALL_THISCALL);
    engine->RegisterObjectBehaviour(name.c_str(), asBEHAVE_GET_WEAKREF_FLAG, "int &f()", asMETHOD(Object, weakRefFlag), asCALL_THISCALL);
    engine->RegisterObjectMethod(name.c_str(), "void move(float x, float y)", asMETHODPR(Object, move, (float, float), void), asCALL_THISCALL);
    engine->RegisterObjectMethod(name.c_str(), "void move(const Vector2f& in)", asMETHODPR(Object, move, (const Vector2f&), void), asCALL_THISCALL);
    engine->RegisterObjectMethod(name.c_str(), "string name()", asMETHOD(Object, name), asCALL_THISCALL);
    engine->RegisterObjectMethod(name.c_str(), "string name() const", asMETHOD(Object, name), asCALL_THISCALL);
    engine->RegisterObjectMethod(name.c_str(), "int state()", asMETHOD(Object, state), asCALL_THISCALL);
    engine->RegisterObjectMethod(name.c_str(), "int state() const", asMETHOD(Object, state), asCALL_THISCALL);
    engine->RegisterObjectMethod(name.c_str(), "void setState(int state)", asMETHOD(Object, setState), asCALL_THISCALL);
    engine->RegisterObjectMethod(name.c_str(), "Component@ addComponent(string type, string name)", asMETHOD(Object, addComponent), asCALL_THISCALL);
}
