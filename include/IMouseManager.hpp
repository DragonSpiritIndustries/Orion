#ifndef IMOUSEMANAGER_HPP
#define IMOUSEMANAGER_HPP

#include "Vector2.hpp"
#include "Event.hpp"
#include <unordered_map>

enum class MouseButton : int
{
    LEFT,
    MIDDLE,
    RIGHT,
    X1,
    X2,
    COUNT,
    UNKNOWN
};

// For std::unordered_map
namespace std
{
template<>
struct hash< ::MouseButton >
{
    typedef ::MouseButton argument_type;
    typedef std::underlying_type< argument_type >::type underlying_type;
    typedef std::hash< underlying_type >::result_type result_type;
    result_type operator()( const argument_type& arg ) const
    {
        std::hash< underlying_type > hasher;
        return hasher( static_cast< underlying_type >( arg ) );
    }
};
}

class IMouseManager
{
public:
    IMouseManager();
    virtual ~IMouseManager() {}
    virtual void shutdown()=0;
    virtual bool buttonPressed (MouseButton button)=0;
    virtual bool buttonReleased(MouseButton button)=0;
    virtual Vector2i position()=0;
    virtual Vector2i wheelDelta()=0;
    virtual void setMousePosition(int, int)=0;
    virtual void setMousePosition(Vector2i)=0;

protected:
    void onUpdate(float);
    virtual void onMouseButton(const Event& e)=0;
    virtual void onMouseWheel(const Event& e)=0;
    Vector2i m_mouseWheelDelta;
    std::unordered_map<MouseButton, bool> m_pressed;
    std::unordered_map<MouseButton, bool> m_released;
};

#endif // IMOUSEMANAGER_HPP
