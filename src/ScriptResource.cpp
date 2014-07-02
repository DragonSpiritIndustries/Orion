#include "ScriptResource.hpp"
#include "ScriptEngine.hpp"
#include "angelscript/addons/scriptbuilder.h"
#include "physfs.h"
#include "Athena/Utility.hpp"
#include <aes.h>
#include "ByteCodeStream.hpp"
#include "CVar.hpp"

extern CVar* sc_useBytecode;
extern CVar* sc_buildBytecode;
extern CVar* sc_debugScripts;
extern CVar* res_basepath;
extern CVar* com_developer;

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
    if (path == std::string())
        return false;

    std::string tmp = path;
    if (tmp.find_last_of("/") != std::string::npos)
        tmp = tmp.erase(0, path.find_last_of("/")+1);
    if (tmp.find_last_of(".") != std::string::npos)
        tmp = tmp.erase(tmp.find_last_of("."), tmp.find_last_of(".") - tmp.size() + 1);

    if (tmp == std::string())
        return false;

    m_builder.StartNewModule(orScriptEngineRef.handle(), tmp.c_str());
    if (sc_useBytecode->toBoolean())
    {
        try
        {
            std::string extension = path.substr(path.find_last_of(".") + 1);
            Athena::utility::tolower(extension);
            std::string tmpPath = path;
            if (extension == "as")
                tmpPath += "c";

            ByteCodeReader bcr(tmpPath);

            if (m_builder.GetModule()->LoadByteCode(&bcr) >=0)
                return true;
        }
        catch(const Athena::error::Exception& e)
        {
            orConsoleRef.print(orConsoleRef.Error, "Encountered exception while deserializing bytecode for %s", path.c_str());
            orConsoleRef.print(orConsoleRef.Error, e.message());
            orConsoleRef.print(orConsoleRef.Warning, "Falling back to uncompiled script");
        }
    }

    m_builder.AddSectionFromFile(path.c_str());

    if (m_builder.BuildModule() >= 0)
    {
        m_builder.GetModule()->BindAllImportedFunctions();

        orConsoleRef.print(orConsoleRef.Info, tmp);
        // Only build script bytecode if sc_buildByteCode is true AND we're in developer mode
        if (sc_buildBytecode->toBoolean() && com_developer->toBoolean())
        {
            try
            {
                tmp = res_basepath->toLiteral() + "/" + path + "c";
                ByteCodeWriter bcw(tmp);
                m_builder.GetModule()->SaveByteCode(&bcw, !sc_debugScripts->toBoolean());
            }
            catch(...)
            {
                // fail silently
            }
        }
        return true;
    }
    return false;
}

IResource* ScriptResource::load(const std::string& name)
{
    if (PHYSFS_exists(name.c_str()) || PHYSFS_exists(std::string(name + "c").c_str()))
    {
        ScriptResource* ret = new ScriptResource(name);
        if (ret->loadData(name))
            return ret;

        delete ret;
    }

    orConsoleRef.print(orConsoleRef.Warning, "Script resource %s does not exist or contains errors", name.c_str());
    return nullptr;
}

REGISTER_RESOURCE(ScriptResource, "as asc", load);
