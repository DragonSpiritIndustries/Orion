#include "ITextureResource.hpp"
#include "ResourceManager.hpp"
#include "ScriptEngine.hpp"

ITextureResource* loadTexture(const std::string& path)
{
    return orResourceManagerRef.loadResource<ITextureResource>(path);
}

void registerClass()
{
    orScriptEngineRef.handle()->RegisterObjectType("Texture", 0, asOBJ_REF);
    orScriptEngineRef.handle()->RegisterObjectBehaviour("Texture", asBEHAVE_ADDREF, "void f()", asMETHOD(ITextureResource, addRef), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectBehaviour("Texture", asBEHAVE_RELEASE, "void f()", asMETHOD(ITextureResource, release), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectBehaviour("Texture", asBEHAVE_GET_WEAKREF_FLAG, "int &f()", asMETHOD(ITextureResource, weakRefFlag), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod("Texture", "void draw(const float x, const float y) const", asMETHODPR(ITextureResource, draw, (float, float), void), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod("Texture", "void draw(const float x, const float y, const Rectanglef subrect, const Vector2f origin, const bool flipH, const bool flipV, const float angle) const",
                                                     asMETHODPR(ITextureResource, draw, (float, float, Rectanglef, Vector2f, bool, bool, float), void), asCALL_THISCALL);

    orScriptEngineRef.handle()->RegisterGlobalFunction(std::string("Texture @+ orLoadTexture(string path)").c_str(), asFUNCTIONPR(loadTexture, (const std::string&), ITextureResource*), asCALL_CDECL);
}

REGISTER_SCRIPT_FUNCTION(TextureResource, registerClass);

