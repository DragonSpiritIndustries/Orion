#include "Timer.hpp"

Timer::Timer(bool run)
{
    if (run)
        reset();
}

void Timer::reset()
{
    m_start = HighResClock::now();
}

Timer::Milliseconds Timer::elapsed()
{
    return std::chrono::duration_cast<Milliseconds>(HighResClock::now() - m_start);
}
