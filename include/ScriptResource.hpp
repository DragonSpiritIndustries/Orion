#ifndef SCRIPTRESOURCE_HPP
#define SCRIPTRESOURCE_HPP

#include "ResourceManager.hpp"
#include "angelscript/addons/scriptbuilder.h"

class ScriptResource : public IResource
{
    DEFINE_RESOURCE();
public:
    ScriptResource(const std::string& path);
    virtual ~ScriptResource();
    asIScriptModule* module();

    asIScriptFunction* functionByDecl(const std::string& decl);
    asIScriptFunction* functionByName(const std::string& name);
    asIScriptFunction* functionByIndex(int id);

    bool loadData(const std::string &path);
    static IResource* load(const std::string& path);
private:
    CScriptBuilder m_builder;
};

#endif // SCRIPTRESOURCE_HPP
