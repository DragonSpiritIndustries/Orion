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

#include "Color.hpp"
#include "ScriptEngine.hpp"
#include "Global.hpp"

static void registerColor()
{
    asIScriptEngine* handle  = orScriptEngineRef.handle();
    // Color
    handle->RegisterObjectType("Colorf", sizeof(Colorf), asOBJ_VALUE | asOBJ_ASHANDLE | asOBJ_APP_CLASS_CDAK);
    handle->RegisterObjectBehaviour("Colorf", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION((constructColor<float>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectBehaviour("Colorf", asBEHAVE_CONSTRUCT, "void f(float, float)", asFUNCTION((constructColorArgs<float>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectBehaviour("Colorf", asBEHAVE_DESTRUCT,  "void f()", asFUNCTION((destructColor<float>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectMethod("Colorf", "Colorf &opAssign(const Colorf &in)", asMETHODPR(Colorf, operator =, (const Colorf&), Colorf&), asCALL_THISCALL);
    handle->RegisterObjectProperty("Colorf", "float r", asOFFSET(Colorf, r));
    handle->RegisterObjectProperty("Colorf", "float g", asOFFSET(Colorf, g));
    handle->RegisterObjectProperty("Colorf", "float b", asOFFSET(Colorf, b));
    handle->RegisterObjectProperty("Colorf", "float a", asOFFSET(Colorf, a));
    handle->RegisterObjectType("Colorb", sizeof(Colorb), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
    handle->RegisterObjectBehaviour("Colorb", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION((constructColor<double>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectBehaviour("Colorb", asBEHAVE_CONSTRUCT, "void f(double, double)", asFUNCTION((constructColorArgs<double>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectBehaviour("Colorb", asBEHAVE_DESTRUCT,  "void f()", asFUNCTION((destructColor<double>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectMethod("Colorb", "Colorb &opAssign(const Colorb &in)", asMETHODPR(Colorb, operator =, (const Colorb&), Colorb&), asCALL_THISCALL);
    handle->RegisterObjectProperty("Colorb", "int8 r", asOFFSET(Colorb, r));
    handle->RegisterObjectProperty("Colorb", "int8 g", asOFFSET(Colorb, g));
    handle->RegisterObjectProperty("Colorb", "int8 b", asOFFSET(Colorb, b));
    handle->RegisterObjectProperty("Colorb", "int8 a", asOFFSET(Colorb, a));
    handle->RegisterObjectType("Colori", sizeof(Colori), asOBJ_VALUE | asOBJ_APP_CLASS_CDAK);
    handle->RegisterObjectBehaviour("Colori", asBEHAVE_CONSTRUCT, "void f()", asFUNCTION((constructColor<int>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectBehaviour("Colori", asBEHAVE_CONSTRUCT, "void f(int, int)", asFUNCTION((constructColorArgs<int>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectBehaviour("Colori", asBEHAVE_DESTRUCT,  "void f()", asFUNCTION((destructColor<int>)), asCALL_CDECL_OBJLAST);
    handle->RegisterObjectMethod("Colori", "Colori &opAssign(const Colorf &in)", asMETHODPR(Colori, operator =, (const Colori&), Colori&), asCALL_THISCALL);
    handle->RegisterObjectProperty("Colori", "int r", asOFFSET(Colori, r));
    handle->RegisterObjectProperty("Colori", "int g", asOFFSET(Colori, g));
    handle->RegisterObjectProperty("Colori", "int b", asOFFSET(Colori, b));
    handle->RegisterObjectProperty("Colori", "int a", asOFFSET(Colori, a));
}

REGISTER_SCRIPT_FUNCTION(Color, registerColor);
