#include "angelscript/addons/scriptbuilder.h"
#include <Athena/Utility.hpp>
#include "physfs.h"
#include "CVar.hpp"
#include <vector>

#include <stdio.h>
#if defined(_MSC_VER) && !defined(_WIN32_WCE) && !defined(__S3E__)
#include <direct.h>
#endif
#ifdef _WIN32_WCE
#include <windows.h> // For GetModuleFileName()
#endif

#if defined(__S3E__) || defined(__APPLE__) || defined(__linux__)
#include <unistd.h> // For getcwd()
#endif

BEGIN_AS_NAMESPACE

// Helper functions
static const char *GetCurrentDir(char *buf, size_t size);

extern CVar* sc_debugScripts;

CScriptBuilder::CScriptBuilder()
{
    if (sc_debugScripts->toBoolean())
        DefineWord("DEBUG");

    engine = 0;
    module = 0;

    includeCallback = 0;
    callbackParam   = 0;
}

void CScriptBuilder::SetIncludeCallback(INCLUDECALLBACK_t callback, void *userParam)
{
    includeCallback = callback;
    callbackParam   = userParam;
}

int CScriptBuilder::StartNewModule(asIScriptEngine *engine, const char *moduleName)
{
    if( engine == 0 ) return -1;

    this->engine = engine;
    module = engine->GetModule(moduleName, asGM_CREATE_IF_NOT_EXISTS);
    if( module == 0 )
        return -1;

    ClearAll();

    return 0;
}

asIScriptModule *CScriptBuilder::GetModule()
{
    return module;
}

unsigned int CScriptBuilder::GetSectionCount() const
{
    return (unsigned int)(includedScripts.size());
}

std::string CScriptBuilder::GetSectionName(unsigned int idx) const
{
    if( idx >= includedScripts.size() ) return "";

    std::set<std::string>::const_iterator it = includedScripts.begin();
    while( idx-- > 0 ) it++;
    return *it;
}

int CScriptBuilder::AddSectionFromFile(const char *filename)
{
    // TODO: The file name stored in the set should be the fully resolved name because
    // it is possible to name the same file in multiple ways using relative paths.

    if( IncludeIfNotAlreadyIncluded(filename) )
    {
        int r = LoadScriptSection(filename);
        if( r < 0 )
            return r;
        else
            return 1;
    }

    return 0;
}

int CScriptBuilder::AddSectionFromMemory(const char *sectionName, const char *scriptCode, unsigned int scriptLength)
{
    if( IncludeIfNotAlreadyIncluded(sectionName) )
    {
        int r = ProcessScriptSection(scriptCode, scriptLength, sectionName);
        if( r < 0 )
            return r;
        else
            return 1;
    }

    return 0;
}

int CScriptBuilder::BuildModule()
{
    return Build();
}

void CScriptBuilder::DefineWord(const char *word)
{
    std::string sword = word;
    if( definedWords.find(sword) == definedWords.end() )
    {
        definedWords.insert(sword);
    }
}

void CScriptBuilder::UnDefineWord(const char *word)
{
    std::string sword = word;
    if( definedWords.find(sword) != definedWords.end() )
    {
        definedWords.erase(definedWords.find(sword));
    }
}

void CScriptBuilder::ClearAll()
{
    includedScripts.clear();

#if AS_PROCESS_METADATA == 1
    currentClass = "";
    currentNamespace = "";

    foundDeclarations.clear();
    typeMetadataMap.clear();
    funcMetadataMap.clear();
    varMetadataMap.clear();
#endif
}

bool CScriptBuilder::IncludeIfNotAlreadyIncluded(const char *filename)
{
    std::string scriptFile = filename;
    if( includedScripts.find(scriptFile) != includedScripts.end() )
    {
        // Already included
        return false;
    }

    // Add the file to the set of included sections
    includedScripts.insert(scriptFile);

    return true;
}

int CScriptBuilder::LoadScriptSection(const char *filename)
{
    // Open the script file
    std::string scriptFile = filename;
    PHYSFS_File *f = PHYSFS_openRead(scriptFile.c_str());
    if( f == 0 )
    {
        // Write a message to the engine's message callback
        char buf[256];
        std::string msg = "Failed to open script file '" + std::string(GetCurrentDir(buf, 256)) + "\\" + scriptFile + "'";
        engine->WriteMessage(filename, 0, 0, asMSGTYPE_ERROR, msg.c_str());

        // TODO: Write the file where this one was included from

        return -1;
    }

    // Determine size of the file
    int len = PHYSFS_fileLength(f);

    // On Win32 it is possible to do the following instead
    // int len = _filelength(_fileno(f));

    // Read the entire file
    std::string code;
    int c = 0;
    if( len > 0 )
    {
        code.resize(len);
        c = PHYSFS_read(f, &code[0], len, 1);
    }

    PHYSFS_close(f);

    if( c < 0 && len > 0 )
    {
        // Write a message to the engine's message callback
        char buf[256];
        std::string msg = "Failed to load script file '" + std::string(GetCurrentDir(buf, 256)) + scriptFile + "'";
        engine->WriteMessage(filename, 0, 0, asMSGTYPE_ERROR, msg.c_str());
        return -1;
    }

    // Process the script section even if it is zero length so that the name is registered
    return ProcessScriptSection(code.c_str(), (unsigned int)(code.length()), filename);
}

int CScriptBuilder::ProcessScriptSection(const char *script, unsigned int length, const char *sectionname)
{
    std::vector<std::string> includes;

    // Perform a superficial parsing of the script first to store the metadata
    if( length )
        modifiedScript.assign(script, length);
    else
        modifiedScript = script;

    // First perform the checks for #if directives to exclude code that shouldn't be compiled
    unsigned int pos = 0;
    int nested = 0;
    int line =0;
    while( pos < modifiedScript.size() )
    {
        int len;
        asETokenClass t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
        if( t == asTC_UNKNOWN && modifiedScript[pos] == '#' && (pos + 1 < modifiedScript.size()) )
        {
            int start = pos++;

            // Is this an #if directive?
            asETokenClass t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);

            std::string token;
            token.assign(&modifiedScript[pos], len);

            pos += len;
            if( token == "if" || token == "ifndef" || token == "ifdef")
            {
                t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
                if( t == asTC_WHITESPACE )
                {
                    pos += len;
                    t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
                }

                if( t == asTC_IDENTIFIER )
                {
                    std::string word;
                    word.assign(&modifiedScript[pos], len);

                    // Overwrite the #if directive with space characters to avoid compiler error
                    pos += len;
                    OverwriteCode(start, pos-start);

                    // Has this identifier been defined by the application or not?
                    if( definedWords.find(word) == definedWords.end() && token != "ifndef")
                    {
                        // Exclude all the code until and including the #endif
                        pos = ExcludeCode(pos);
                    }
                    else
                    {
                        nested++;
                    }
                }
                else if (t == asTC_VALUE || t == asTC_KEYWORD)
                {
                    std::string word;
                    word.assign(&modifiedScript[pos], len);

                    // Overwrite the #if directive with space characters to avoid compiler error
                    pos += len;
                    OverwriteCode(start, pos-start);

                    if (word == "1" || word == "true")
                    {
                        nested++;
                    }
                    else
                    {
                        // Exclude all the code until and including the #endif
                        pos = ExcludeCode(pos);
                    }
                }
            }
            else if( token == "endif" )
            {
                // Only remove the #endif if there was a matching #if
                if( nested > 0 )
                {
                    OverwriteCode(start, pos-start);
                    nested--;
                }
            }
            else if( token == "define")
            {
                t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
                if( t == asTC_WHITESPACE )
                {
                    pos += len;
                    t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
                }
                if (t == asTC_IDENTIFIER)
                {
                    std::string word;
                    word.assign(&modifiedScript[pos], len);
                    if (definedWords.find(word) == definedWords.end())
                        DefineWord(word.c_str());
                    else
                    {
                        std::string msg = "Redefinition of " + std::string(word);
                        engine->WriteMessage(sectionname, line, 0, asMSGTYPE_WARNING, msg.c_str());
                    }

                }
                pos += len;
                // Overwrite the #if directive with space characters to avoid compiler error
                OverwriteCode(start, pos-start);
            }
            else if (token == "undef")
            {
                t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
                if( t == asTC_WHITESPACE )
                {
                    pos += len;
                    t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
                }
                if (t == asTC_IDENTIFIER)
                {
                    std::string word;
                    word.assign(&modifiedScript[pos], len);
                    if (definedWords.find(word) != definedWords.end())
                        UnDefineWord(word.c_str());
                    else
                    {
                        std::string msg = "Unable to undefine " + std::string(word) + " it is not defined";
                        engine->WriteMessage(sectionname, line, 0, asMSGTYPE_WARNING, msg.c_str());
                    }
                }
                else
                {
                    engine->WriteMessage(sectionname, line, 0, asMSGTYPE_ERROR, "Invalid value for #undef");
                    return -1;
                }
                // Overwrite the #if directive with space characters to avoid compiler error
                pos += len;
                OverwriteCode(start, pos-start);
            }
        }
        else
            pos += len;
        line++;
    }

    if (nested > 0)
    {
        engine->WriteMessage(sectionname, line, 0, asMSGTYPE_ERROR, "Missing #endif in section");
        return -1;
    }

#if AS_PROCESS_METADATA == 1
    // Preallocate memory
    std::string metadata, declaration;
    metadata.reserve(500);
    declaration.reserve(100);
#endif

    // Then check for meta data and #include directives
    pos = 0;
    while( pos < modifiedScript.size() )
    {
        int len;
        asETokenClass t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
        if( t == asTC_COMMENT || t == asTC_WHITESPACE )
        {
            pos += len;
            continue;
        }

#if AS_PROCESS_METADATA == 1
        // Check if class
        if( currentClass == "" && modifiedScript.substr(pos,len) == "class" )
        {
            // Get the identifier after "class"
            do
            {
                pos += len;
                if( pos >= modifiedScript.size() )
                {
                    t = asTC_UNKNOWN;
                    break;
                }
                t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
            } while(t == asTC_COMMENT || t == asTC_WHITESPACE);

            if( t == asTC_IDENTIFIER )
            {
                currentClass = modifiedScript.substr(pos,len);

                // Search until first { is encountered
                while( pos < modifiedScript.length() )
                {
                    engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);

                    // If start of class section encountered stop
                    if( modifiedScript[pos] == '{' )
                    {
                        pos += len;
                        break;
                    }

                    // Check next symbol
                    pos += len;
                }
            }

            continue;
        }

        // Check if end of class
        if( currentClass != "" && modifiedScript[pos] == '}' )
        {
            currentClass = "";
            pos += len;
            continue;
        }

        // Check if namespace
        if( modifiedScript.substr(pos,len) == "namespace" )
        {
            // Get the identifier after "namespace"
            do
            {
                pos += len;
                t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
            } while(t == asTC_COMMENT || t == asTC_WHITESPACE);

            if( currentNamespace != "" )
                currentNamespace += "::";
            currentNamespace += modifiedScript.substr(pos,len);

            // Search until first { is encountered
            while( pos < modifiedScript.length() )
            {
                engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);

                // If start of namespace section encountered stop
                if( modifiedScript[pos] == '{' )
                {
                    pos += len;
                    break;
                }

                // Check next symbol
                pos += len;
            }

            continue;
        }

        // Check if end of namespace
        if( currentNamespace != "" && modifiedScript[pos] == '}' )
        {
            size_t found = currentNamespace.rfind( "::" );
            if( found != std::string::npos )
            {
                currentNamespace.erase( found );
            }
            else
            {
                currentNamespace = "";
            }
            pos += len;
            continue;
        }

        // Is this the start of metadata?
        if( modifiedScript[pos] == '[' )
        {
            // Get the metadata string
            pos = ExtractMetadataString(pos, metadata);

            // Determine what this metadata is for
            int type;
            ExtractDeclaration(pos, declaration, type);

            // Store away the declaration in a map for lookup after the build has completed
            if( type > 0 )
            {
                SMetadataDecl decl(metadata, declaration, type, currentClass, currentNamespace);
                foundDeclarations.push_back(decl);
            }
        }
        else
#endif
        // Is this a preprocessor directive?
        if( modifiedScript[pos] == '#' && (pos + 1 < modifiedScript.size()) )
        {
            int start = pos++;

            asETokenClass t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
            if( t == asTC_IDENTIFIER )
            {
                std::string token;
                token.assign(&modifiedScript[pos], len);
                if( token == "include" )
                {
                    pos += len;
                    t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
                    if( t == asTC_WHITESPACE )
                    {
                        pos += len;
                        t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
                    }

                    if( t == asTC_VALUE && len > 2 && (modifiedScript[pos] == '"' || modifiedScript[pos] == '\'') )
                    {
                        // Get the include file
                        std::string includefile;
                        includefile.assign(&modifiedScript[pos+1], len-2);
                        pos += len;

                        // Store it for later processing
                        includes.push_back(includefile);

                        // Overwrite the include directive with space characters to avoid compiler error
                        OverwriteCode(start, pos-start);
                    }
                }
            }
        }
        // Don't search for metadata/includes within statement blocks or between tokens in statements
        else
        {
            pos = SkipStatement(pos);
        }
    }

    // Build the actual script
    engine->SetEngineProperty(asEP_COPY_SCRIPT_SECTIONS, true);
    module->AddScriptSection(sectionname, modifiedScript.c_str(), modifiedScript.size());

    if( includes.size() > 0 )
    {
        // If the callback has been set, then call it for each included file
        if( includeCallback )
        {
            for( int n = 0; n < (int)includes.size(); n++ )
            {
                int r = includeCallback(includes[n].c_str(), sectionname, this, callbackParam);
                if( r < 0 )
                    return r;
            }
        }
        else
        {
            // By default we try to load the included file from the relative directory of the current file

            // Determine the path of the current script so that we can resolve relative paths for includes
            std::string path = sectionname;
            size_t posOfSlash = path.find_last_of("/\\");
            if( posOfSlash != std::string::npos )
                path.resize(posOfSlash+1);
            else
                path = "";

            // Load the included scripts
            for( int n = 0; n < (int)includes.size(); n++ )
            {
                // If the include is a relative path, then prepend the path of the originating script
                if( includes[n].find_first_of("/\\") != 0 &&
                    includes[n].find_first_of(":") == std::string::npos )
                {
                    includes[n] = path + includes[n];
                }

                // Include the script section
                int r = AddSectionFromFile(includes[n].c_str());
                if( r < 0 )
                    return r;
            }
        }
    }

    return 0;
}

int CScriptBuilder::Build()
{
    int r = module->Build();
    if( r < 0 )
        return r;

#if AS_PROCESS_METADATA == 1
    // After the script has been built, the metadata strings should be
    // stored for later lookup by function id, type id, and variable index
    for( int n = 0; n < (int)foundDeclarations.size(); n++ )
    {
        SMetadataDecl *decl = &foundDeclarations[n];
        module->SetDefaultNamespace(decl->nameSpace.c_str());
        if( decl->type == 1 )
        {
            // Find the type id
            int typeId = module->GetTypeIdByDecl(decl->declaration.c_str());
            if( typeId >= 0 )
                typeMetadataMap.insert(std::map<int, std::string>::value_type(typeId, decl->metadata));
        }
        else if( decl->type == 2 )
        {
            if( decl->parentClass == "" )
            {
                // Find the function id
                asIScriptFunction *func = module->GetFunctionByDecl(decl->declaration.c_str());
                if( func )
                    funcMetadataMap.insert(std::map<int, std::string>::value_type(func->GetId(), decl->metadata));
            }
            else
            {
                // Find the method id
                int typeId = module->GetTypeIdByDecl(decl->parentClass.c_str());
                std::map<int, SClassMetadata>::iterator it = classMetadataMap.find(typeId);
                if( it == classMetadataMap.end() )
                {
                    classMetadataMap.insert(std::map<int, SClassMetadata>::value_type(typeId, SClassMetadata(decl->parentClass)));
                    it = classMetadataMap.find(typeId);
                }

                asIObjectType *type = engine->GetObjectTypeById(typeId);
                asIScriptFunction *func = type->GetMethodByDecl(decl->declaration.c_str());
                if( func )
                    it->second.funcMetadataMap.insert(std::map<int, std::string>::value_type(func->GetId(), decl->metadata));
            }
        }
        else if( decl->type == 4 )
        {
            if( decl->parentClass == "" )
            {
                // Find the global virtual property accessors
                asIScriptFunction *func = module->GetFunctionByName(("get_" + decl->declaration).c_str());
                if( func )
                    funcMetadataMap.insert(std::map<int, std::string>::value_type(func->GetId(), decl->metadata));
                func = module->GetFunctionByName(("set_" + decl->declaration).c_str());
                if( func )
                    funcMetadataMap.insert(std::map<int, std::string>::value_type(func->GetId(), decl->metadata));
            }
            else
            {
                // Find the method virtual property accessors
                int typeId = module->GetTypeIdByDecl(decl->parentClass.c_str());
                std::map<int, SClassMetadata>::iterator it = classMetadataMap.find(typeId);
                if( it == classMetadataMap.end() )
                {
                    classMetadataMap.insert(std::map<int, SClassMetadata>::value_type(typeId, SClassMetadata(decl->parentClass)));
                    it = classMetadataMap.find(typeId);
                }

                asIObjectType *type = engine->GetObjectTypeById(typeId);
                asIScriptFunction *func = type->GetMethodByName(("get_" + decl->declaration).c_str());
                if( func )
                    it->second.funcMetadataMap.insert(std::map<int, std::string>::value_type(func->GetId(), decl->metadata));
                func = type->GetMethodByName(("set_" + decl->declaration).c_str());
                if( func )
                    it->second.funcMetadataMap.insert(std::map<int, std::string>::value_type(func->GetId(), decl->metadata));

            }
        }
        else if( decl->type == 3 )
        {
            if( decl->parentClass == "" )
            {
                // Find the global variable index
                int varIdx = module->GetGlobalVarIndexByName(decl->declaration.c_str());
                if( varIdx >= 0 )
                    varMetadataMap.insert(std::map<int, std::string>::value_type(varIdx, decl->metadata));
            }
            else
            {
                int typeId = module->GetTypeIdByDecl(decl->parentClass.c_str());

                // Add the classes if needed
                std::map<int, SClassMetadata>::iterator it = classMetadataMap.find(typeId);
                if( it == classMetadataMap.end() )
                {
                    classMetadataMap.insert(std::map<int, SClassMetadata>::value_type(typeId, SClassMetadata(decl->parentClass)));
                    it = classMetadataMap.find(typeId);
                }

                // Add the variable to class
                asIObjectType *objectType = engine->GetObjectTypeById(typeId);
                int idx = -1;

                // Search through all properties to get proper declaration
                for( asUINT i = 0; i < (asUINT)objectType->GetPropertyCount(); ++i )
                {
                    const char *name;
                    objectType->GetProperty(i, &name);
                    if( decl->declaration == name )
                    {
                        idx = i;
                        break;
                    }
                }

                // If found, add it
                if( idx >= 0 ) it->second.varMetadataMap.insert(std::map<int, std::string>::value_type(idx, decl->metadata));
            }
        }
    }
    module->SetDefaultNamespace("");
#endif

    return 0;
}

int CScriptBuilder::SkipStatement(int pos)
{
    int len;

    // Skip until ; or { whichever comes first
    while( pos < (int)modifiedScript.length() && modifiedScript[pos] != ';' && modifiedScript[pos] != '{' )
    {
        engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
        pos += len;
    }

    // Skip entire statement block
    if( pos < (int)modifiedScript.length() && modifiedScript[pos] == '{' )
    {
        pos += 1;

        // Find the end of the statement block
        int level = 1;
        while( level > 0 && pos < (int)modifiedScript.size() )
        {
            asETokenClass t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
            if( t == asTC_KEYWORD )
            {
                if( modifiedScript[pos] == '{' )
                    level++;
                else if( modifiedScript[pos] == '}' )
                    level--;
            }

            pos += len;
        }
    }
    else
        pos += 1;

    return pos;
}

// Overwrite all code with blanks until the matching #endif
int CScriptBuilder::ExcludeCode(int pos)
{
    int len;
    int nested = 0;
    while( pos < (int)modifiedScript.size() )
    {
        engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
        if( modifiedScript[pos] == '#' )
        {
            modifiedScript[pos] = ' ';
            pos++;

            // Is it an #if or #endif directive?
            engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
            std::string token;
            token.assign(&modifiedScript[pos], len);
            OverwriteCode(pos, len);

            if( token == "if" )
            {
                nested++;
            }
            else if( token == "endif" )
            {
                if( nested-- == 0 )
                {
                    pos += len;
                    break;
                }
            }
        }
        else if( modifiedScript[pos] != '\n' )
        {
            OverwriteCode(pos, len);
        }
        pos += len;
    }

    return pos;
}

// Overwrite all characters except line breaks with blanks
void CScriptBuilder::OverwriteCode(int start, int len)
{
    char *code = &modifiedScript[start];
    for( int n = 0; n < len; n++ )
    {
        if( *code != '\n' )
            *code = ' ';
        code++;
    }
}

#if AS_PROCESS_METADATA == 1
int CScriptBuilder::ExtractMetadataString(int pos, std::string &metadata)
{
    metadata = "";

    // Overwrite the metadata with space characters to allow compilation
    modifiedScript[pos] = ' ';

    // Skip opening brackets
    pos += 1;

    int level = 1;
    int len;
    while( level > 0 && pos < (int)modifiedScript.size() )
    {
        asETokenClass t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
        if( t == asTC_KEYWORD )
        {
            if( modifiedScript[pos] == '[' )
                level++;
            else if( modifiedScript[pos] == ']' )
                level--;
        }

        // Copy the metadata to our buffer
        if( level > 0 )
            metadata.append(&modifiedScript[pos], len);

        // Overwrite the metadata with space characters to allow compilation
        if( t != asTC_WHITESPACE )
            OverwriteCode(pos, len);

        pos += len;
    }

    return pos;
}

int CScriptBuilder::ExtractDeclaration(int pos, std::string &declaration, int &type)
{
    declaration = "";
    type = 0;

    int start = pos;

    std::string token;
    int len = 0;
    asETokenClass t = asTC_WHITESPACE;

    // Skip white spaces and comments
    do
    {
        pos += len;
        t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
    } while ( t == asTC_WHITESPACE || t == asTC_COMMENT );

    // We're expecting, either a class, interface, function, or variable declaration
    if( t == asTC_KEYWORD || t == asTC_IDENTIFIER )
    {
        token.assign(&modifiedScript[pos], len);
        if( token == "interface" || token == "class" )
        {
            // Skip white spaces and comments
            do
            {
                pos += len;
                t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
            } while ( t == asTC_WHITESPACE || t == asTC_COMMENT );

            if( t == asTC_IDENTIFIER )
            {
                type = 1;
                declaration.assign(&modifiedScript[pos], len);
                pos += len;
                return pos;
            }
        }
        else
        {
            // For function declarations, store everything up to the start of the statement block

            // For variable declaration store just the name as there can only be one

            // We'll only know if the declaration is a variable or function declaration when we see the statement block, or absense of a statement block.
            bool hasParenthesis = false;
            declaration.append(&modifiedScript[pos], len);
            pos += len;
            std::string name;
            for(; pos < (int)modifiedScript.size();)
            {
                t = engine->ParseToken(&modifiedScript[pos], modifiedScript.size() - pos, &len);
                if( t == asTC_KEYWORD )
                {
                    token.assign(&modifiedScript[pos], len);
                    if( token == "{" )
                    {
                        if( hasParenthesis )
                        {
                            // We've found the end of a function signature
                            type = 2;
                        }
                        else
                        {
                            // We've found a virtual property. Just keep the name
                            declaration = name;
                            type = 4;
                        }
                        return pos;
                    }
                    if( token == "=" || token == ";" )
                    {
                        // Substitute the declaration with just the name
                        declaration = name;
                        type = 3;
                        return pos;
                    }
                    else if( token == "(" )
                    {
                        // This is the first parenthesis we encounter. If the parenthesis isn't followed
                        // by a statement block, then this is a variable declaration, in which case we
                        // should only store the type and name of the variable, not the initialization parameters.
                        hasParenthesis = true;
                    }
                }
                else if( t == asTC_IDENTIFIER )
                {
                    name.assign(&modifiedScript[pos], len);
                }

                declaration.append(&modifiedScript[pos], len);
                pos += len;
            }
        }
    }

    return start;
}

const char *CScriptBuilder::GetMetadataStringForType(int typeId)
{
    std::map<int,std::string>::iterator it = typeMetadataMap.find(typeId);
    if( it != typeMetadataMap.end() )
        return it->second.c_str();

    return "";
}

const char *CScriptBuilder::GetMetadataStringForFunc(asIScriptFunction *func)
{
    if( func )
    {
        std::map<int,std::string>::iterator it = funcMetadataMap.find(func->GetId());
        if( it != funcMetadataMap.end() )
            return it->second.c_str();
    }

    return "";
}

const char *CScriptBuilder::GetMetadataStringForVar(int varIdx)
{
    std::map<int,std::string>::iterator it = varMetadataMap.find(varIdx);
    if( it != varMetadataMap.end() )
        return it->second.c_str();

    return "";
}

const char *CScriptBuilder::GetMetadataStringForTypeProperty(int typeId, int varIdx)
{
    std::map<int, SClassMetadata>::iterator typeIt = classMetadataMap.find(typeId);
    if(typeIt == classMetadataMap.end()) return "";

    std::map<int, std::string>::iterator propIt = typeIt->second.varMetadataMap.find(varIdx);
    if(propIt == typeIt->second.varMetadataMap.end()) return "";

    return propIt->second.c_str();
}

const char *CScriptBuilder::GetMetadataStringForTypeMethod(int typeId, asIScriptFunction *method)
{
    if( method )
    {
        std::map<int, SClassMetadata>::iterator typeIt = classMetadataMap.find(typeId);
        if(typeIt == classMetadataMap.end()) return "";

        std::map<int, std::string>::iterator methodIt = typeIt->second.funcMetadataMap.find(method->GetId());
        if(methodIt == typeIt->second.funcMetadataMap.end()) return "";

        return methodIt->second.c_str();
    }

    return "";
}
#endif

static const char *GetCurrentDir(char *buf, size_t size)
{
#ifdef _MSC_VER
#ifdef _WIN32_WCE
    static TCHAR apppath[MAX_PATH] = TEXT("");
    if (!apppath[0])
    {
        GetModuleFileName(NULL, apppath, MAX_PATH);


        int appLen = _tcslen(apppath);

        // Look for the last backslash in the path, which would be the end
        // of the path itself and the start of the filename.  We only want
        // the path part of the exe's full-path filename
        // Safety is that we make sure not to walk off the front of the
        // array (in case the path is nothing more than a filename)
        while (appLen > 1)
        {
            if (apppath[appLen-1] == TEXT('\\'))
                break;
            appLen--;
        }

        // Terminate the string after the trailing backslash
        apppath[appLen] = TEXT('\0');
    }
#ifdef _UNICODE
    wcstombs(buf, apppath, min(size, wcslen(apppath)*sizeof(wchar_t)));
#else
    memcpy(buf, apppath, min(size, strlen(apppath)));
#endif

    return buf;
#elif defined(__S3E__)
    // Marmalade uses its own portable C library
    return getcwd(buf, (int)size);
#elif _XBOX_VER >= 200
    // XBox 360 doesn't support the getcwd function, just use the root folder
    assert( size >= 7 );
    sprintf(buf, "game:\\");
    return buf;
#elif defined(_M_ARM)
    // TODO: How to determine current working dir on Windows Phone?
    return "";
#else
    return _getcwd(buf, (int)size);
#endif // _MSC_VER
#elif defined(__APPLE__) || defined(__linux__)
    return getcwd(buf, size);
#else
    return "";
#endif
}

END_AS_NAMESPACE


