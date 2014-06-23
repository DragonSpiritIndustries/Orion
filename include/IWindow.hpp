#ifndef IWINDOW_HPP
#define IWINDOW_HPP

#include "Event.hpp"
#include <string>
#include "Vector2.hpp"

//! This should remain a private member
//! refrain from making it publicly accessible
//! doing so will violate Orions ability to abstract
//! core window functionality.
//! your IApplication implementation is required to expose
//! setTitle, title, windowSize, and window* functions anyway

template <class HandleType>
class IWindow
{
public:
    virtual bool initialize()=0;
    virtual void setTitle(const std::string&)=0;
    virtual std::string title() const=0;


    virtual HandleType* handle() const=0;

    virtual Vector2i windowSize()=0;
    virtual int windowWidth()=0;
    virtual int windowHeight()=0;
};

#endif // IWINDOW_HPP
