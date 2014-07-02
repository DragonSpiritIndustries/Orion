#ifndef IFONTRESOURCE_HPP
#define IFONTRESOURCE_HPP

#include "IResource.hpp"
class IFontResource : public IResource
{
public:
    IFontResource(const std::string& path);
};

#endif // IFONTRESOURCE_HPP
