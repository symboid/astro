
#ifndef __SYMBOID_ASTRO_HORA_DEFS_H__
#define __SYMBOID_ASTRO_HORA_DEFS_H__

#include "sdk/arch/defs.h"

#ifdef BUILD_ASTRO_HORA
    #define ASTRO_HORA_API SY_API_EXPORT
#else
    #define ASTRO_HORA_API SY_API_IMPORT
#endif

#endif // __SYMBOID_ASTRO_HORA_DEFS_H__
