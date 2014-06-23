#ifndef ITEXTURERESOURCE_HPP
#define ITEXTURERESOURCE_HPP

#include "IResource.hpp"
#include "ResourceManager.hpp"
#include "Rectangle.hpp"
#include "Vector2.hpp"

class IApplication;
class ITextureResource : public IResource
{
public:
    ITextureResource(const std::string path)
        : IResource(path)
    {
    }

    virtual void draw(IApplication*, int x, int y)=0;
    virtual void drawPart(IApplication*, int x, int y, Rectanglef partRect)=0;
    virtual Vector2i size() const = 0;
    virtual int width()     const = 0;
    virtual int height()    const = 0;
};

#endif // ITEXTURERESOURCE_HPP
