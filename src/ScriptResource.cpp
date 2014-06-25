#include "ScriptResource.hpp"
#include "ScriptEngine.hpp"
#include "angelscript/addons/scriptbuilder.h"
#include "physfs.h"
#include "Athena/Utility.hpp"

ScriptResource::ScriptResource(const std::string& path)
    : IResource(path)
{
}

ScriptResource::~ScriptResource()
{
    module()->Discard();
}

asIScriptModule* ScriptResource::module()
{
    return m_builder.GetModule();
}

asIScriptFunction* ScriptResource::functionByDecl(const std::string& decl)
{
    return module()->GetFunctionByDecl(decl.c_str());
}

asIScriptFunction* ScriptResource::functionByName(const std::string& name)
{
    return module()->GetFunctionByName(name.c_str());
}


asIScriptFunction* ScriptResource::functionByIndex(int id)
{
    return module()->GetFunctionByIndex(id);
}

bool ScriptResource::loadData(const std::string& path)
{
    std::string tmp = path;
    if (tmp.find_last_of("/\\") != std::string::npos)
        tmp = tmp.substr(path.find_last_of("/\\"));
    if (tmp.find_last_of(".") != std::string::npos)
        tmp = tmp.substr(0, path.find_last_of("."));

    m_builder.StartNewModule(orScriptEngineRef.handle(), tmp.c_str());
    m_builder.AddSectionFromFile(path.c_str());

    if (m_builder.BuildModule() >= 0)
        return true;

    return false;
}

IResource* ScriptResource::load(const std::string& name)
{
    if (PHYSFS_exists(name.c_str()))
    {
        ScriptResource* ret = new ScriptResource(name);
        if (ret->loadData(name))
            return ret;

        delete ret;
    }

    orConsoleRef.print(orConsoleRef.Warning, "Script resource %s does not exist or contains errors", name.c_str());
    return nullptr;
}

REGISTER_RESOURCE(ScriptResource, load);
