#ifndef IRENDERER_HPP
#define IRENDERER_HPP

#include "Color.hpp"
class IWindow;
class IRenderer
{
public:
    virtual bool initialize(IWindow*)=0;
    virtual void setClearColor(const Colorf& color = Colorf::black)=0;
    virtual void clear()=0;
    virtual void present()=0;
    virtual void drawRect(int, int, int, int, bool=false, Colorb=Colorb::white)=0;
    virtual void* handle()=0;
    virtual void setVSync(bool)=0;
protected:
    Colorf         m_clearColor;
};

#endif // IRENDERER_HPP
