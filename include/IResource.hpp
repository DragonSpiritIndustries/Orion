#ifndef IRESOURCE_HPP
#define IRESOURCE_HPP

#include <vector>
#include <string>

class IResource
{
public:
    IResource(const std::string& path)
        : m_path(path)
    {
    }

    virtual ~IResource() {}
    std::string path() const { return m_path; }
    virtual const std::string resourceType() =0;
protected:
    std::string m_path;
};

#endif // IRESOURCE_HPP
