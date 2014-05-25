#ifndef IGRAPHICSDRIVERINFO_HPP
#define IGRAPHICSDRIVERINFO_HPP

#include "IDriverInfo.hpp"
#include <string>

class IGraphicsDriverInfo : public IDriverInfo
{
public:
    virtual const std::string name()         const=0;
    virtual int version()                    const=0;
    virtual const std::string versionString()const=0;
    virtual const std::string extensions()   const=0;
};

#endif // IGRAPHICSDRIVERINFO_HPP
