
#ifndef __SYMBOID_ASTRO_CALCULO_DEFS_H__
#define __SYMBOID_ASTRO_CALCULO_DEFS_H__

#include "sdk/basics/defs.h"

#ifdef BUILD_ASTRO_CALCULO
    #define ASTRO_CALCULO_API SY_API_EXPORT
#else
    #define ASTRO_CALCULO_API SY_API_IMPORT
#endif

#define hor_ns_begin sy_ns_begin(hor)
#define hor_ns_end sy_ns_end

#endif // __SYMBOID_ASTRO_CALCULO_DEFS_H__
