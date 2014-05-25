#ifndef SDLEVENT_HPP
#define SDLEVENT_HPP

#include "IEvent.hpp"
class SDLEvent : public IEvent
{
public:
    SDLEvent();

    void poll();
private:
    Type m_type;
};

#endif // SDLEVENT_HPP
