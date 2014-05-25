#ifndef IAPPLICATION_HPP
#define IAPPLICATION_HPP

#include <Global.hpp>
#include <string>

namespace Nano
{
template<typename T>
class Signal;
}
union SDL_Event;

class IApplication
{
public:
    IApplication();

    virtual ~IApplication() {}

    virtual int  exec()=0;
    virtual bool init(int argc, char* argv[])=0;
    virtual void onEvent(SDL_Event*)=0;
    virtual void onUpdate()=0;
    virtual void onDraw()=0;
    virtual void onExit()=0;
    virtual void setTitle(const std::string& title)=0;

    virtual void drawRectangle(int w, int h, int x, int y, bool fill = false)=0;
    virtual Nano::Signal<void(IApplication*)>& drawSignal()=0;
    virtual Nano::Signal<void(IApplication*)>& updateSignal()=0;
    static IApplication& instanceByRef();
    static IApplication* instanceByPtr();
protected:
    virtual void parseCommandLine(int argc, char* argv[]){UNUSED(argc),UNUSED(argv);}
    static IApplication* m_instance;
};

#endif // IAPPLICATION_HPP
