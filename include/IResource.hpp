#ifndef IRESOURCE_HPP
#define IRESOURCE_HPP

#include <vector>
#include <string>

class asILockableSharedBool;
class asIScriptObject;
class asIScriptContext;
class IResource
{
public:
    IResource(const std::string& path);

    virtual ~IResource() {}

    int addRef();

    int release();

    void destroyAndRelease();

    asILockableSharedBool* weakRefFlag();


    std::string path() const;
protected:
    std::string m_path;
    asILockableSharedBool* m_weakRefFlag;
    asIScriptContext*      m_scriptContext;
    int m_refCount;
};

#endif // IRESOURCE_HPP
