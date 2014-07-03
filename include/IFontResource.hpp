#ifndef IFONTRESOURCE_HPP
#define IFONTRESOURCE_HPP

#include "IResource.hpp"
#include "Vector2.hpp"
#include <Color.hpp>
#include <Athena/Types.hpp>
class IFontResource : public IResource
{
public:
    IFontResource(const std::string& path);
    ~IFontResource() {}
    virtual Vector2i measureString(const std::string& str, int size=16)=0;
    virtual int glyphAdvance(atUint16 ch, int size = 16)=0;
    virtual void draw(float x, float y, const std::string& str, int size=16, const Colorb& color=Colorb::white)=0;
};

#endif // IFONTRESOURCE_HPP
