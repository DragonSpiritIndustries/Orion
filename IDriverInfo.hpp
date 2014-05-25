#ifndef IDRIVERINFO_HPP
#define IDRIVERINFO_HPP

#include <string>

class IDriverInfo
{
public:
    virtual const std::string name() const=0;
};

#endif // IDRIVERINFO_HPP
