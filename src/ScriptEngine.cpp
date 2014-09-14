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

#include "ScriptEngine.hpp"
#include "ApplicationBase.hpp"
#include "angelscript/angelscript.h"
#include "angelscript/addons.h"
#include "Console.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Rectangle.hpp"
#include "CVar.hpp"
#include <memory>

CVar* sc_useBytecode   = new CVar("sc_useBytecode",   "false", "Whether or not to use precompiled bytecode", CVar::Boolean, CVar::System | CVar::ReadOnly);
CVar* sc_buildBytecode = new CVar("sc_buildBytecode", "false", "Compile bytecode to be loaded when sc_useBytecode is set to true", CVar::Boolean, CVar::System | CVar::ReadOnly);
CVar* sc_debugScripts  = new CVar("sc_debugScripts",  "false",  "Enable script debugging, also creates bytecode with debug symbols if sc_buildBytecode is enabled", CVar::Boolean, CVar::System | CVar::ReadOnly | CVar::Hidden);

// This is used for AngelScript error messages
void MessageCallback(const asSMessageInfo &msg)
{
    Console::Level type = Console::Error;
    if( msg.type == asMSGTYPE_WARNING )
    {
        type = Console::Warning;
    }
    else if( msg.type == asMSGTYPE_INFORMATION )
    {
        type = Console::Info;
    }

    orConsoleRef.print(type,  "[SCRIPT] %s (%i, %i) : %s", msg.section, msg.row, msg.col, msg.message);
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
        m_engineHandle->RegisterGlobalProperty("ScriptEngine orScriptEngine", this);
        RegisterStdString(m_engineHandle);
        RegisterStdStringUtils(m_engineHandle);
        RegisterScriptMath(m_engineHandle);
        RegisterScriptWeakRef(m_engineHandle);
        RegisterScriptHandle(m_engineHandle);

        // Vector3
        m_engineHandle->RegisterObjectType("Vector3f", sizeof(Vector3f), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
        m_engineHandle->RegisterObjectBehaviour("Vector3f", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(constructVector3f), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectBehaviour("Vector3f", asBEHAVE_CONSTRUCT, "void f(float, float, float)", asFUNCTION(constructVector3fArgs), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectBehaviour("Vector3f", asBEHAVE_DESTRUCT,  "void f()", asFUNCTION(destructVector3f), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectProperty("Vector3f", "float x", asOFFSET(Vector3f, x));
        m_engineHandle->RegisterObjectProperty("Vector3f", "float y", asOFFSET(Vector3f, y));
        m_engineHandle->RegisterObjectProperty("Vector3f", "float z", asOFFSET(Vector3f, z));
        m_engineHandle->RegisterObjectType("Vector3d", sizeof(Vector3d), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
        m_engineHandle->RegisterObjectBehaviour("Vector3d", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(constructVector3d), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectBehaviour("Vector3d", asBEHAVE_CONSTRUCT, "void f(double, double, double)", asFUNCTION(constructVector3dArgs), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectBehaviour("Vector3d", asBEHAVE_DESTRUCT,  "void f()", asFUNCTION(destructVector3f), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectProperty("Vector3d", "double x", asOFFSET(Vector3d, x));
        m_engineHandle->RegisterObjectProperty("Vector3d", "double y", asOFFSET(Vector3d, y));
        m_engineHandle->RegisterObjectProperty("Vector3d", "double z", asOFFSET(Vector3d, z));
        m_engineHandle->RegisterObjectType("Vector3i", sizeof(Vector3i), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
        m_engineHandle->RegisterObjectBehaviour("Vector3i", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(constructVector3i), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectBehaviour("Vector3i", asBEHAVE_CONSTRUCT, "void f(int, int, int)", asFUNCTION(constructVector3iArgs), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectBehaviour("Vector3i", asBEHAVE_DESTRUCT,  "void f()", asFUNCTION(destructVector3i), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectProperty("Vector3i", "int x", asOFFSET(Vector3i, x));
        m_engineHandle->RegisterObjectProperty("Vector3i", "int y", asOFFSET(Vector3i, y));
        m_engineHandle->RegisterObjectProperty("Vector3i", "int z", asOFFSET(Vector3i, z));
        // Rectangle
        m_engineHandle->RegisterObjectType("Rectanglef", sizeof(Rectanglef), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
        m_engineHandle->RegisterObjectBehaviour("Rectanglef", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(constructRectanglef), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectBehaviour("Rectanglef", asBEHAVE_CONSTRUCT, "void f(float, float, float, float)", asFUNCTION(constructRectanglefArgs), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectBehaviour("Rectanglef", asBEHAVE_DESTRUCT,  "void f()", asFUNCTION(destructRectanglef), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectMethod("Rectanglef", "Rectanglef &opAssign(const Rectanglef &in)", asMETHODPR(Rectanglef, operator =, (const Rectanglef&), Rectanglef&), asCALL_THISCALL);
        m_engineHandle->RegisterObjectProperty("Rectanglef", "float x", asOFFSET(Rectanglef, x));
        m_engineHandle->RegisterObjectProperty("Rectanglef", "float y", asOFFSET(Rectanglef, y));
        m_engineHandle->RegisterObjectProperty("Rectanglef", "float w", asOFFSET(Rectanglef, w));
        m_engineHandle->RegisterObjectProperty("Rectanglef", "float h", asOFFSET(Rectanglef, h));
        m_engineHandle->RegisterObjectType("Rectangled", sizeof(Rectangled), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
        m_engineHandle->RegisterObjectBehaviour("Rectangled", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(constructRectangled), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectBehaviour("Rectangled", asBEHAVE_CONSTRUCT, "void f(double, double, double, double)", asFUNCTION(constructRectangledArgs), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectBehaviour("Rectangled", asBEHAVE_DESTRUCT,  "void f()", asFUNCTION(destructRectangled), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectMethod("Rectangled", "Rectangled &opAssign(const Rectangled &in)", asMETHODPR(Rectangled, operator =, (const Rectangled&), Rectangled&), asCALL_THISCALL);
        m_engineHandle->RegisterObjectProperty("Rectangled", "double x", asOFFSET(Rectangled, x));
        m_engineHandle->RegisterObjectProperty("Rectangled", "double y", asOFFSET(Rectangled, y));
        m_engineHandle->RegisterObjectProperty("Rectangled", "double w", asOFFSET(Rectangled, w));
        m_engineHandle->RegisterObjectProperty("Rectangled", "double h", asOFFSET(Rectangled, h));
        m_engineHandle->RegisterObjectType("Rectanglei", sizeof(Rectanglei), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
        m_engineHandle->RegisterObjectBehaviour("Rectanglei", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION(constructRectanglei), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectBehaviour("Rectanglei", asBEHAVE_CONSTRUCT, "void f(int, int, int, int)", asFUNCTION(constructRectangleiArgs), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectBehaviour("Rectanglei", asBEHAVE_DESTRUCT,  "void f()", asFUNCTION(destructRectanglei), asCALL_CDECL_OBJLAST);
        m_engineHandle->RegisterObjectMethod("Rectanglei", "Rectanglei &opAssign(const Rectanglei &in)", asMETHODPR(Rectanglei, operator =, (const Rectanglei&), Rectanglei&), asCALL_THISCALL);
        m_engineHandle->RegisterObjectProperty("Rectanglei", "int x", asOFFSET(Rectanglei, x));
        m_engineHandle->RegisterObjectProperty("Rectanglei", "int y", asOFFSET(Rectanglei, y));
        m_engineHandle->RegisterObjectProperty("Rectanglei", "int w", asOFFSET(Rectanglei, w));
        m_engineHandle->RegisterObjectProperty("Rectanglei", "int h", asOFFSET(Rectanglei, h));
    }
}

ScriptEngine::~ScriptEngine()
{
    if (m_engineHandle)
        m_engineHandle->Release();
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

void ScriptEngine::onDraw()
{
    asUINT curSize, totalDestroyed, totalDetected, newObjs, totalNewDestroyed;
    handle()->GetGCStatistics(&curSize, &totalDestroyed, &totalDetected, &newObjs, &totalNewDestroyed);
    orApplicationRef.drawDebugText(Athena::utility::sprintf("Current Size: %i", curSize), 16, 100);
    orApplicationRef.drawDebugText(Athena::utility::sprintf("Total Destroyed: %i", totalDestroyed), 16, 116);
    orApplicationRef.drawDebugText(Athena::utility::sprintf("Total Detected: %i", totalDetected), 16, 132);
}
