#ifndef TIMER_HPP
#define TIMER_HPP

#include <Athena/Types.hpp>
#include <chrono>

class Timer
{
    typedef std::chrono::high_resolution_clock HighResClock;
    typedef std::chrono::milliseconds Milliseconds;
public:
    explicit Timer(bool run = false);

    void reset();
    Milliseconds elapsed();

private:
    HighResClock::time_point m_start;
};

#endif // TIMER_HPP
