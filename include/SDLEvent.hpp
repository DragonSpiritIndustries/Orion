#ifndef SDLEVENT_HPP
#define SDLEVENT_HPP

#include "IEvent.hpp"
class SDLEvent : public IEvent
{
public:
    SDLEvent();

    Type type() const;
private:
    Type m_type;
};

#endif // SDLEVENT_HPP
