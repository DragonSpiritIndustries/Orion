#ifndef IWINDOW_HPP
#define IWINDOW_HPP

#include "Event.hpp"
#include <string>

template <class HandleType>
class IWindow
{
public:
    virtual bool initialize()=0;
    virtual void setTitle(const std::string&)=0;
    virtual std::string title() const=0;

    virtual HandleType* handle() const=0;
};

#endif // IWINDOW_HPP
