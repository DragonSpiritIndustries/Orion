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
    virtual void draw(const Vector2f& position)=0;
    virtual void draw(float x, float y, const Rectanglef& subrect, const Vector2f& origin, bool flipH, bool flipV, float angle)=0;
    virtual void draw(const Rectanglef& rect, const Rectanglef& subrect, const Vector2f& origin, bool flipH, bool flipV, float angle)=0;
    virtual Vector2i size() const = 0;
    virtual int width()     const;
    virtual int height()    const;
    virtual void setWrapH(bool val)=0;
    virtual void setWrapV(bool val)=0;
protected:
    bool m_wrapH;
    bool m_wrapV;
    Vector2i     m_size;
};

#endif // ITEXTURERESOURCE_HPP
