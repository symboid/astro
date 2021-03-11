
#ifndef __SYMBOID_ASTRO_CONTROLS_DEFS_H__
#define __SYMBOID_ASTRO_CONTROLS_DEFS_H__

#include "sdk/arch/defs.h"

#ifdef BUILD_ASTRO_CONTROLS
    #define ASTRO_CONTROLS_API SY_API_EXPORT
#else
    #define ASTRO_CONTROLS_API SY_API_IMPORT
#endif

#endif // __SYMBOID_ASTRO_CONTROLS_DEFS_H__
