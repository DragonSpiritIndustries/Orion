#include "ScriptEngine.hpp"
#include "angelscript/angelscript.h"
#include "angelscript/addons.h"
#include "Console.hpp"
#include "Vector2.hpp"
#include <memory>

// This is used for AngelScript error messages
void MessageCallback(const asSMessageInfo &msg)
{
    const char *type = "ERR ";
    if( msg.type == asMSGTYPE_WARNING )
    {
        type = "WARN";
    }
    else if( msg.type == asMSGTYPE_INFORMATION )
    {
        type = "INFO";
    }

    orConsoleRef.print(orConsoleRef.Warning,  "%s (%i, %i) : %s : %s", msg.section, msg.row, msg.col, type, msg.message);
}


ScriptEngine::ScriptEngine()
{
    m_engineHandle = asCreateScriptEngine(ANGELSCRIPT_VERSION);
    if (!m_engineHandle)
        orConsoleRef.print(orConsoleRef.Fatal, "Failed to initialize AngelScript!!!!");
    else
    {
        orConsoleRef.print(orConsoleRef.Info, "AngelScript v%s initialized", ANGELSCRIPT_VERSION_STRING);
        m_engineHandle->SetMessageCallback(asFUNCTION(MessageCallback), 0, asCALL_CDECL);
        m_engineHandle->RegisterObjectType("ScriptEngine", 0, asOBJ_REF | asOBJ_NOHANDLE);
        m_engineHandle->RegisterObjectMethod("ScriptEngine", "void clean()", asMETHOD(ScriptEngine, clean), asCALL_THISCALL);
        m_engineHandle->RegisterGlobalProperty("ScriptEngine scriptEngine", this);
    }
}

ScriptEngine::~ScriptEngine()
{
    if (m_engineHandle)
        m_engineHandle->Release();
}

void ScriptEngine::initialize()
{
    if (!m_engineHandle)
        return;

    RegisterStdString(m_engineHandle);
    RegisterStdStringUtils(m_engineHandle);
    RegisterScriptMath(m_engineHandle);
    RegisterScriptWeakRef(m_engineHandle);
    RegisterScriptHandle(m_engineHandle);

    m_engineHandle->RegisterObjectType("Vector2f", sizeof(Vector2f), asOBJ_VALUE | asOBJ_POD);
    m_engineHandle->RegisterObjectProperty("Vector2f", "float x", asOFFSET(Vector2f, x));
    m_engineHandle->RegisterObjectProperty("Vector2f", "float y", asOFFSET(Vector2f, y));
    m_engineHandle->RegisterObjectType("Vector2i", sizeof(Vector2i), asOBJ_VALUE | asOBJ_POD);
    m_engineHandle->RegisterObjectProperty("Vector2i", "int x", asOFFSET(Vector2i, x));
    m_engineHandle->RegisterObjectProperty("Vector2i", "int y", asOFFSET(Vector2i, y));
}

asIScriptEngine* ScriptEngine::handle()
{
    return m_engineHandle;
}

ScriptEngine& ScriptEngine::instanceRef()
{
    return *ScriptEngine::instancePtr();
}

ScriptEngine* ScriptEngine::instancePtr()
{
    static std::shared_ptr<ScriptEngine> instance = std::shared_ptr<ScriptEngine>(new ScriptEngine);

    return instance.get();
}

void ScriptEngine::clean()
{
    m_engineHandle->GarbageCollect();
    orConsoleRef.print(orConsoleRef.Info, "Flushed AngelScript GC");
}
