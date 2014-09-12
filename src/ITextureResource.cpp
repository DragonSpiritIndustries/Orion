#include "ITextureResource.hpp"
#include TEXTURERESOURCE_IMPL_HEADER
#include "ResourceManager.hpp"
#include "ScriptEngine.hpp"

int ITextureResource::width() const
{
    return m_size.x;
}

int ITextureResource::height() const
{
    return m_size.y;
}

void ITextureResource::setRepeat(bool val)
{
    m_repeat = val;
}


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
    orScriptEngineRef.handle()->RegisterObjectMethod("Texture", "void draw(const float x, const float y, const Rectanglef& in subrect, const Vector2f& in origin, const bool flipH, const bool flipV, const float angle) const",
                                                     asMETHODPR(ITextureResource, draw, (float, float, const Rectanglef&, const Vector2f&, bool, bool, float), void), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod("Texture", "void draw(const Rectanglef& in rect, const Rectanglef& in subrect, const Vector2f& in origin, const bool flipH, const bool flipV, const float angle) const",
                                                     asMETHODPR(ITextureResource, draw, (const Rectanglef&, const Rectanglef&, const Vector2f&, bool, bool, float), void), asCALL_THISCALL);
    // Properties
    orScriptEngineRef.handle()->RegisterObjectMethod("Texture", "Vector2i size() const", asMETHOD(ITextureResource, size), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod("Texture", "int width() const", asMETHOD(ITextureResource, width), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterObjectMethod("Texture", "int height() const", asMETHOD(ITextureResource, height), asCALL_THISCALL);
    orScriptEngineRef.handle()->RegisterGlobalFunction(std::string("Texture @+ orLoadTexture(string path)").c_str(), asFUNCTIONPR(loadTexture, (const std::string&), ITextureResource*), asCALL_CDECL);
}

REGISTER_SCRIPT_FUNCTION(TextureResource, registerClass);
REGISTER_RESOURCE(TEXTURERESOURCE_IMPL, "jpeg jpg png bmp tga", loadTexture);
