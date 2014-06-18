#ifndef ICOMPONENT_HPP
#define ICOMPONENT_HPP

#include <string>

class IComponent
{
public:
    virtual std::string name() const=0;
};

#endif // ICOMPONENT_HPP
