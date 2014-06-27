#include "CVarManager.hpp"
#include "CVar.hpp"
#include "IKeyboardManager.hpp"
#include <Athena/Utility.hpp>
#include "ScriptEngine.hpp"
#include <memory>

CVar* com_developer = new CVar("developer", "false", "Enables developer mode", CVar::Boolean, (CVar::System | CVar::Cheat | CVar::ReadOnly));
CVar* com_configfile = new CVar("config", "config.cfg", "File to store configuration", CVar::Literal, CVar::System);

CVarManager::CVarManager()
{
}

CVarManager::~CVarManager()
{
    writeToFile(com_configfile->toLiteral());

    for (std::pair<std::string, CVar*> pair : m_cvars)
    {
        delete pair.second;
        pair.second = NULL;
    }

    m_cvars.clear();
}

void CVarManager::initialize()
{
    loadFromFile();
}

bool CVarManager::registerCVar(CVar *cvar)
{
    std::string tmp = cvar->name();
    Athena::utility::tolower(tmp);
    if (m_cvars.find(tmp) != m_cvars.end())
        return false;

    m_cvars[tmp] = cvar;
    if (m_config.RootElement())
        cvar->deserialize(m_config.RootElement());

    return true;
}

void CVarManager::requestSerialization(CVar* cvar)
{
    bool oldDeveloper = suppressDeveloper();

    if (cvar != NULL)
    {
        if (cvar->type() == CVar::Bind)
            cvar->serializeBinding(m_config.RootElement());
        else
            cvar->serializeCVar(m_config.RootElement(), oldDeveloper);
    }

    restoreDeveloper(oldDeveloper);
    writeToFile();
}

CVar* CVarManager::findCVar(const std::string &name)
{
    std::string tmp = std::string(name);
    Athena::utility::tolower(tmp);
    if (m_cvars.find(tmp) == m_cvars.end())
        return NULL;
    return m_cvars[tmp];
}

std::vector<CVar*> CVarManager::archivedCVars() const
{
    std::vector<CVar*> ret;
    for (std::pair<std::string, CVar*> pair : m_cvars)
    {
        if ((pair.second->flags() & CVar::Archive) == CVar::Archive && pair.second->type() != CVar::Bind)
            ret.push_back(pair.second);
    }

    return ret;
}

std::vector<CVar *> CVarManager::bindings() const
{
    std::vector<CVar*> ret;
    for (std::pair<std::string, CVar*> pair : m_cvars)
    {
        if (pair.second->type() == CVar::Bind)
            ret.push_back(pair.second);
    }

    return ret;
}

std::vector<CVar*> CVarManager::cvars() const
{
    std::vector<CVar*> ret;
    for (std::pair<std::string, CVar*> pair : m_cvars)
        ret.push_back(pair.second);

    return ret;
}

void CVarManager::writeToFile(const std::string &file)
{
    if (file != std::string())
        com_configfile->fromLiteral(file);

    bool oldDeveloper = suppressDeveloper();

    bool newConfig = m_config.NoChildren();

    TiXmlNode* root = NULL;
    if (newConfig)
    {
        TiXmlDeclaration* versionDecl = new TiXmlDeclaration("1.0", "UTF-8", "");
        root = new TiXmlElement("Config");
        m_config.LinkEndChild(versionDecl);
        m_config.LinkEndChild(root);
    }
    else
        root = m_config.RootElement();

    for (CVar* binding : bindings())
        binding->serializeBinding(root);

    for (CVar* cvar : archivedCVars())
        cvar->serializeCVar(root, oldDeveloper);

    restoreDeveloper(oldDeveloper);
    m_config.SaveFile(com_configfile->toLiteral());
}

void CVarManager::loadFromFile(const std::string& file)
{
    if (file != std::string())
        com_configfile->fromLiteral(file);
    m_config.LoadFile(com_configfile->toLiteral());
    m_config.SetCondenseWhiteSpace(true);
    m_config.SetTabSize(4);

    for (CVar* cvar : cvars())
        cvar->deserialize(m_config.RootElement());
}

CVarManager& CVarManager::instanceRef()
{
    return *instancePtr();
}

CVarManager* CVarManager::instancePtr()
{
    static std::shared_ptr<CVarManager> instance = std::shared_ptr<CVarManager>(new CVarManager);

    return instance.get();
}

bool CVarManager::suppressDeveloper()
{
    bool oldDeveloper = com_developer->toBoolean();
    com_developer->unlock();
    com_developer->fromBoolean(false);

    return oldDeveloper;
}

void CVarManager::restoreDeveloper(bool oldDeveloper)
{
    com_developer->unlock();
    com_developer->fromBoolean(oldDeveloper);
    com_developer->lock();
}

static void registerCVar();

static void registerCVarManager()
{
    registerCVar();
    orScriptEngineRef.handle()->RegisterObjectType    ("CVarManager", 0, asOBJ_REF | asOBJ_NOHANDLE);
    orScriptEngineRef.handle()->RegisterObjectMethod  ("CVarManager", "CVar @ cvar(const string& in)", asMETHOD(CVarManager, findCVar), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterGlobalProperty("CVarManager orCVarManager", orCVarManagerPtr);
}

static void registerCVar()
{
    asIScriptEngine* handle = orScriptEngineRef.handle();
    handle->RegisterObjectType  ("CVar", 0, asOBJ_REF | asOBJ_NOCOUNT);
    handle->RegisterObjectMethod("CVar", "float toFloat()",                   asMETHOD(CVar, toFloat),   asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "float toFloat(bool& out)",          asMETHOD(CVar, toFloat),   asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool fromFloat(float& in)",         asMETHOD(CVar, fromFloat),   asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool toBoolean()",                  asMETHOD(CVar, toBoolean), asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool toBoolean(bool& out)",         asMETHOD(CVar, toBoolean), asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool fromBoolean(const bool& in)",  asMETHOD(CVar, fromBoolean), asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "int toInteger()",                   asMETHOD(CVar, toInteger), asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "int toInteger(bool& out)",          asMETHOD(CVar, toInteger), asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool fromInteger(const int& in)",   asMETHOD(CVar, fromInteger), asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "string toLiteral()",                asMETHOD(CVar, toLiteral), asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "string toLiteral(bool& out)",       asMETHOD(CVar, toLiteral), asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool fromLiteral(const string& in)",asMETHOD(CVar, fromLiteral), asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "Colorf toColorf()",                 asMETHOD(CVar, toColorf),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "Colorf toColorf(bool& out)",        asMETHOD(CVar, toColorf),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool fromColorf(const Colorf& in)", asMETHOD(CVar, fromColorf), asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "Colorb toColorb()",                 asMETHOD(CVar, toColorb),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "Colorb toColorb(bool& out)",        asMETHOD(CVar, toColorb),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool fromColorb(const Colorb& in)", asMETHOD(CVar, fromColorb), asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "Colori toColori()",                 asMETHOD(CVar, toColori),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "Colori toColori(bool& out)",        asMETHOD(CVar, toColori),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool fromColori(const Colori& in)", asMETHOD(CVar, fromColori), asCALL_THISCALL);
}

REGISTER_SCRIPT_FUNCTION(CVarManager, registerCVarManager);
