
#ifndef __SYMBOID_ASTRO_DB_DEFS_H__
#define __SYMBOID_ASTRO_DB_DEFS_H__

#include "sdk/arch/defs.h"

#ifdef BUILD_ASTRO_DB
    #define ASTRO_DB_API SY_API_EXPORT
#else
    #define ASTRO_DB_API SY_API_IMPORT
#endif

#endif // __SYMBOID_ASTRO_DB_DEFS_H__
