#ifndef ITEXTURERESOURCE_HPP
#define ITEXTURERESOURCE_HPP

#include "IResource.hpp"
#include "ResourceManager.hpp"
#include "Rectangle.hpp"
#include "Vector2.hpp"

class ApplicationBase;
class ITextureResource : public IResource
{
public:
    ITextureResource(const std::string path)
        : IResource(path)
    {
    }

    virtual void draw(float x, float y)=0;
    virtual void draw(Vector2f position)=0;
    virtual void draw(float x, float y, Rectanglef subrect, Vector2f origin, bool flipH, bool flipV, float angle)=0;
    virtual void draw(Vector2f position, Rectanglef subrect, Vector2f origin, bool flipH, bool flipV, float angle)=0;
    virtual Vector2i size() const = 0;
    virtual int width()     const = 0;
    virtual int height()    const = 0;
};

#endif // ITEXTURERESOURCE_HPP
