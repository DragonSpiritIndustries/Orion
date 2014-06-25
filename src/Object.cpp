#include "Object.hpp"
#include "ObjectManager.hpp"
#include "Global.hpp"
#include "ScriptEngine.hpp"
#include "ResourceManager.hpp"
#include "ScriptResource.hpp"
#include <algorithm>


Object::Object(const std::string& name)
    : m_name(name),
      m_refCount(1),
      m_weakRefFlag(nullptr),
      m_controller(nullptr)
{
    m_script = orResourceManagerRef.loadResource<ScriptResource>(this->name() + ".as");
    m_scriptContext = orScriptEngineRef.handle()->CreateContext();
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

void Object::onThink()
{
    if (m_scriptContext)
    {
        asIScriptFunction* thinkFunc = m_script->functionByName("onThink");
        if(thinkFunc)
        {
            m_scriptContext->Prepare(thinkFunc);
            m_scriptContext->Execute();
        }
    }
}

void Object::onUpdate(float delta)
{
    if (m_scriptContext)
    {
        asIScriptFunction* updateFunc = m_script->functionByDecl("void onUpdate(Object@ self, float delta)");
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
    if (m_controller)
    {
        m_controller->Release();
        m_controller = nullptr;
    }
    release();
}

asILockableSharedBool* Object::weakRefFlag()
{
    if (!m_weakRefFlag)
        m_weakRefFlag = asCreateLockableSharedBool();

    return m_weakRefFlag;
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
}
