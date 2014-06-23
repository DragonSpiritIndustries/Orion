#ifndef _ORION_GLOBAL_HPP
#define _ORION_GLOBAL_HPP

#include <stdio.h>
#include <stdarg.h>

#define orVERSION_STR "0.0.1"

#define orRELEASE_NAME "Dionysus"

// TODO: Fill in
class Logger
{
public:
};


#ifndef orDEFAULT_ORGANIZATION_NAME
#define orDEFAULT_ORGANIZATION_NAME "org.orion.default"
#endif

#ifndef orDEFAULT_APPLICATION_NAME
#define orDEFAULT_APPLICATION_NAME "UntitledOrionApplication"
#endif

#ifndef orDEFAULT_ARCHIVE_EXTENSION
#define orDEFAULT_ARCHIVE_EXTENSION "orp"
#endif

#ifndef orDebug
#define orDebug(...) printf(__VA_ARGS__)
#endif

#ifndef UNUSED
#define UNUSED(x) ((void)x)
#endif // UNUSED

#ifndef orSIGNALS
#define orSIGNALS protected
#endif

// simple foreach macros
#ifndef orForeach
#define orForeach for
#define _in_ :
#endif

#endif // _ORION_GLOBAL_HPP
