#ifndef IRENDERER_HPP
#define IRENDERER_HPP

#include "Color.hpp"
class IWindow;
class IRenderer
{
public:
    virtual bool initialize(IWindow&)=0;
    virtual void setClearColor(const Colorb& color = Colorb::black)=0;
    virtual void clear()=0;
    virtual void present()=0;
    virtual void drawRect(int, int, int, int, bool=false)=0;
    virtual void* handle()=0;
};

#endif // IRENDERER_HPP
