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

#include "Vector2.hpp"
#include "Global.hpp"
#include <new>
#include "ScriptEngine.hpp"


void registerVector2()
{
    asIScriptEngine* handle  = orScriptEngineRef.handle();
    // Vector2
    handle->RegisterObjectType("Vector2f", sizeof(Vector2f), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
    handle->RegisterObjectBehaviour("Vector2f", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION((constructVector2<float>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectBehaviour("Vector2f", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION((constructVector2Args<float>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectBehaviour("Vector2f", asBEHAVE_DESTRUCT,  "void f()", asFUNCTION((destructVector2<float>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectMethod("Vector2f", "Vector2f &opAssign(const Vector2f &in)", asMETHODPR(Vector2f, operator =, (const Vector2f&), Vector2f&), asCALL_THISCALL);
    handle->RegisterObjectProperty("Vector2f", "float x", asOFFSET(Vector2f, x));
    handle->RegisterObjectProperty("Vector2f", "float y", asOFFSET(Vector2f, y));
    handle->RegisterObjectType("Vector2d", sizeof(Vector2d), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
    handle->RegisterObjectBehaviour("Vector2d", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION((constructVector2<double>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectBehaviour("Vector2d", asBEHAVE_CONSTRUCT, "void f(double, double)", asFUNCTION((constructVector2Args<double>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectBehaviour("Vector2d", asBEHAVE_DESTRUCT,  "void f()", asFUNCTION((destructVector2<double>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectMethod("Vector2d", "Vector2d &opAssign(const Vector2d &in)", asMETHODPR(Vector2d, operator =, (const Vector2d&), Vector2d&), asCALL_THISCALL);
    handle->RegisterObjectProperty("Vector2d", "double x", asOFFSET(Vector2d, x));
    handle->RegisterObjectProperty("Vector2d", "double y", asOFFSET(Vector2d, y));
    handle->RegisterObjectType("Vector2i", sizeof(Vector2i), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
    handle->RegisterObjectBehaviour("Vector2i", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION((constructVector2<int>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectBehaviour("Vector2i", asBEHAVE_CONSTRUCT, "void f(int, int)", asFUNCTION((constructVector2Args<int>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectBehaviour("Vector2i", asBEHAVE_DESTRUCT,  "void f()", asFUNCTION((destructVector2<int>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectMethod("Vector2i", "Vector2i &opAssign(const Vector2f &in)", asMETHODPR(Vector2i, operator =, (const Vector2i&), Vector2i&), asCALL_THISCALL);
    handle->RegisterObjectProperty("Vector2i", "int x", asOFFSET(Vector2i, x));
    handle->RegisterObjectProperty("Vector2i", "int y", asOFFSET(Vector2i, y));
}

REGISTER_SCRIPT_FUNCTION(Vector2, registerVector2);
