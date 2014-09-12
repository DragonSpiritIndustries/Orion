#ifndef SCRIPTENGINE_HPP
#define SCRIPTENGINE_HPP

#include "angelscript/angelscript.h"

class ScriptEngine
{
public:
    ScriptEngine();
    ~ScriptEngine();

    asIScriptEngine* handle();
    static ScriptEngine& instanceRef();
    static ScriptEngine* instancePtr();

    void clean();
    void onDraw();
private:
    asIScriptEngine* m_engineHandle;
};

#define orScriptEngineRef ScriptEngine::instanceRef()
#define orScriptEnginePtr ScriptEngine::instancePtr()

#endif // SCRIPTENGINE_HPP
