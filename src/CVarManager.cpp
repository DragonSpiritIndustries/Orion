#include "CVarManager.hpp"
#include "CVar.hpp"
#include "IKeyboardManager.hpp"
#include <Athena/Utility.hpp>
#include "ScriptEngine.hpp"
#include <memory>

CVar* com_developer = new CVar("developer", "false", "Enables developer mode", CVar::Boolean, (CVar::System | CVar::Cheat | CVar::ReadOnly));
CVar* com_configfile = new CVar("config", "config.cfg", "File to store configuration", CVar::Literal, CVar::System);
CVar* com_enableCheats = new CVar("iamaweiner", "false", "Enable cheats", CVar::Boolean, CVar::System | CVar::ReadOnly | CVar::Hidden);

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
    CVarUnlocker unlock(com_developer);
    com_developer->fromBoolean(false);

    return oldDeveloper;
}

void CVarManager::restoreDeveloper(bool oldDeveloper)
{
    CVarUnlocker unlock(com_developer);
    com_developer->fromBoolean(oldDeveloper);
}

#include <new>
static void cvarLockerConstruct(CVar* target, CVarUnlocker* unlocker)
{
    new(unlocker) CVarUnlocker(target);
}

void cvarLockerDestruct(CVarUnlocker* unlocker)
{
    unlocker->~CVarUnlocker();
}

static bool joystickValid(int which, CVar* cvar)
{
    if (which > IJoystickManager::MaxJoysticks)
        return false;
    if (cvar)
    {
        bool valid;
        CVar::Binding b = cvar->toBinding(&valid);
        if (!valid)
            return false;

        return b.Joysticks[which].valid;
    }

    return false;
}

static int joyAxis(int which, CVar* cvar)
{
    if (which > IJoystickManager::MaxJoysticks)
        return -1;
    if (cvar)
    {
        bool valid;
        CVar::Binding b = cvar->toBinding(&valid);
        if (!valid)
            return -1;

        return b.Joysticks[which].Axis;
    }

    return -1;
}


static bool joyAxisNeg(int which, CVar* cvar)
{
    if (which > IJoystickManager::MaxJoysticks)
        return -1;
    if (cvar)
    {
        bool valid;
        CVar::Binding b = cvar->toBinding(&valid);
        if (!valid)
            return -1;

        return b.Joysticks[which].NegativeAxis;
    }

    return -1;
}

static int joyButton(int which, CVar* cvar)
{
    if (which > IJoystickManager::MaxJoysticks)
        return -1;
    if (cvar)
    {
        bool valid;
        CVar::Binding b = cvar->toBinding(&valid);
        if (!valid)
            return -1;

        return b.Joysticks[which].Button;
    }

    return -1;
}
static CVar* createCVar(const std::string& name, const std::string& value, const std::string& help, CVar::Type type, int flags)
{
    // if the cvar already exists just pass it
    if (orCVarManagerRef.findCVar(name))
        return orCVarManagerRef.findCVar(name);

    new CVar(name, value, help, type, flags);
    return orCVarManagerRef.findCVar(name);
}

static CVar* createBinding(const std::string& name, const std::string& help, int flags)
{
    // if the cvar already exists just pass it
    if (orCVarManagerRef.findCVar(name))
        return orCVarManagerRef.findCVar(name);

    new CVar(name, CVar::Binding(), help, flags);
    return orCVarManagerRef.findCVar(name);
}

template<typename T>
static CVar* createColor(const std::string& name, const std::string& help, int flags)
{
    // if the cvar already exists just pass it
    if (orCVarManagerRef.findCVar(name))
        return orCVarManagerRef.findCVar(name);

    new CVar(name, Color<T>(), help, flags);
    return orCVarManagerRef.findCVar(name);
}

static void registerCVar();
static void registerCVarLocker();

static void registerCVarManager()
{
    registerCVar();
    registerCVarLocker();
    orScriptEngineRef.handle()->RegisterObjectType    ("CVarManager", 0, asOBJ_REF | asOBJ_NOHANDLE);
    orScriptEngineRef.handle()->RegisterObjectMethod  ("CVarManager", "CVar @ cvar(const string& in)", asMETHOD(CVarManager, findCVar), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterGlobalProperty("CVarManager orCVarManager", orCVarManagerPtr);

    orScriptEngineRef.handle()->RegisterGlobalFunction("CVar @ orCreateCVar(const string& in name, const string& in value, const string& in help, int type, int flags)",
                                                       asFUNCTION(createCVar), asCALL_CDECL);
    orScriptEngineRef.handle()->RegisterGlobalFunction("CVar @ orCreateColorf(const string& in name, const string& in help, int flags)",
                                                       asFUNCTION((createColor<float>)), asCALL_CDECL);
    orScriptEngineRef.handle()->RegisterGlobalFunction("CVar @ orCreateColorb(const string& in name, const string& in help, int flags)",
                                                       asFUNCTION((createColor<char>)), asCALL_CDECL);
    orScriptEngineRef.handle()->RegisterGlobalFunction("CVar @ orCreateColori(const string& in name, const string& in help, int flags)",
                                                       asFUNCTION((createColor<int>)), asCALL_CDECL);
    orScriptEngineRef.handle()->RegisterGlobalFunction("CVar @ orCreateBinding(const string& in name, const string& in help, int flags)",
                                                       asFUNCTION(createBinding), asCALL_CDECL);
}

static void registerCVarLocker()
{
    orScriptEngineRef.handle()->RegisterObjectType     ("CVarUnlocker", sizeof(CVarUnlocker), asOBJ_VALUE | asOBJ_APP_CLASS_CD | asOBJ_POD);
    orScriptEngineRef.handle()->RegisterObjectBehaviour("CVarUnlocker", asBEHAVE_CONSTRUCT, "void f(CVar@)", asFUNCTION(cvarLockerConstruct), asCALL_CDECL_OBJLAST);
    orScriptEngineRef.handle()->RegisterObjectBehaviour("CVarUnlocker", asBEHAVE_DESTRUCT,  "void f()",      asFUNCTION(cvarLockerDestruct),  asCALL_CDECL_OBJLAST);
}

static void registerCVar()
{
    asIScriptEngine* handle = orScriptEngineRef.handle();

    handle->RegisterObjectType  ("CVar", sizeof(CVar), asOBJ_REF | asOBJ_NOCOUNT);
    handle->RegisterObjectMethod("CVar", "float toFloat()",                   asMETHOD(CVar, toFloat),    asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "float toFloat(bool& out)",          asMETHOD(CVar, toFloat),    asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool fromFloat(float& in)",         asMETHOD(CVar, fromFloat),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool isFloat()",                    asMETHOD(CVar, isFloat),    asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool toBoolean()",                  asMETHOD(CVar, toBoolean),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool toBoolean(bool& out)",         asMETHOD(CVar, toBoolean),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool fromBoolean(const bool& in)",  asMETHOD(CVar, fromBoolean),asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool isBoolean()",                  asMETHOD(CVar, isBoolean),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "int toInteger()",                   asMETHOD(CVar, toInteger),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "int toInteger(bool& out)",          asMETHOD(CVar, toInteger),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool fromInteger(const int& in)",   asMETHOD(CVar, fromInteger),asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool isInteger()",                  asMETHOD(CVar, isInteger),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "string toLiteral()",                asMETHOD(CVar, toLiteral),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "string toLiteral(bool& out)",       asMETHOD(CVar, toLiteral),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool fromLiteral(const string& in)",asMETHOD(CVar, fromLiteral),asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool isLiteral()",                  asMETHOD(CVar, isLiteral),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "Colorf toColorf()",                 asMETHOD(CVar, toColorf),   asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "Colorf toColorf(bool& out)",        asMETHOD(CVar, toColorf),   asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool fromColorf(const Colorf& in)", asMETHOD(CVar, fromColorf), asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "Colorb toColorb()",                 asMETHOD(CVar, toColorb),   asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "Colorb toColorb(bool& out)",        asMETHOD(CVar, toColorb),   asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool fromColorb(const Colorb& in)", asMETHOD(CVar, fromColorb), asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "Colori toColori()",                 asMETHOD(CVar, toColori),   asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "Colori toColori(bool& out)",        asMETHOD(CVar, toColori),   asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool fromColori(const Colori& in)", asMETHOD(CVar, fromColori), asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool isColor()",                    asMETHOD(CVar, isColor),    asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool isBinding()",                  asMETHOD(CVar, isBinding),  asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "bool joyValid(int which)",          asFUNCTION(joystickValid),  asCALL_CDECL_OBJLAST);
    handle->RegisterObjectMethod("CVar", "bool joyValid(const int which) const",asFUNCTION(joystickValid),  asCALL_CDECL_OBJLAST);
    handle->RegisterObjectMethod("CVar", "int  button(int which)",            asFUNCTION(joyButton),      asCALL_CDECL_OBJLAST);
    handle->RegisterObjectMethod("CVar", "int  button(const int which) const",asFUNCTION(joyButton),      asCALL_CDECL_OBJLAST);
    handle->RegisterObjectMethod("CVar", "int  axis(int which)",              asFUNCTION(joyAxis),        asCALL_CDECL_OBJLAST);
    handle->RegisterObjectMethod("CVar", "int  axis(const int which) const",  asFUNCTION(joyAxis),        asCALL_CDECL_OBJLAST);
    handle->RegisterObjectMethod("CVar", "bool  isAxisNegative(int which)",  asFUNCTION(joyAxisNeg),        asCALL_CDECL_OBJLAST);
    handle->RegisterObjectMethod("CVar", "bool  isAxisNegative(const int which) const",  asFUNCTION(joyAxisNeg),        asCALL_CDECL_OBJLAST);
    handle->RegisterObjectMethod("CVar", "void unlock()",                     asMETHOD(CVar, unlock),     asCALL_THISCALL);
    handle->RegisterObjectMethod("CVar", "void lock()",                       asMETHOD(CVar, unlock),     asCALL_THISCALL);
}

REGISTER_SCRIPT_FUNCTION(CVarManager, registerCVarManager);
