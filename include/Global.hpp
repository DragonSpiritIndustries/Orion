#ifndef _ORION_GLOBAL_HPP
#define _ORION_GLOBAL_HPP

#include <stdio.h>
#include <stdarg.h>

#include "Config.hpp"

#define orVERSION_STR "0.0.1"

#define orRELEASE_NAME "Dionysus"

#ifndef orDEFAULT_ORGANIZATION_NAME
#define orDEFAULT_ORGANIZATION_NAME "org.orion.default"
#endif

#ifndef orDEFAULT_APPLICATION_NAME
#define orDEFAULT_APPLICATION_NAME "UntitledOrionApplication"
#endif

#ifndef orDEFAULT_ARCHIVE_EXTENSION
#define orDEFAULT_ARCHIVE_EXTENSION "orp"
#endif

//#ifndef orDebug
//#define orDebug(...) printf(__VA_ARGS__)
//#endif

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

#ifndef REGISTER_SCRIPT_FUNCTION
#define REGISTER_SCRIPT_FUNCTION(Class, Function) \
struct hidden_scriptRegistration##Class \
{ hidden_scriptRegistration##Class() { Function(); } }; static hidden_scriptRegistration##Class __hidden_scriptRegistration##Class
#endif


#endif // _ORION_GLOBAL_HPP
