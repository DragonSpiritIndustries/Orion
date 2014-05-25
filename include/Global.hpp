#ifndef _ORION_GLOBAL_HPP
#define _ORION_GLOBAL_HPP

#include <stdio.h>
#include <stdarg.h>

// TODO: Fill in
class Logger
{
public:
};


#ifndef orDebug
#define orDebug(...) printf(__VA_ARGS__)
#endif

#ifndef UNUSED
#define UNUSED(x) ((void)x)
#endif // UNUSED

#endif // _ORION_GLOBAL_HPP
