
#ifndef __SYMBOID_ASTRO_SWEPH_DEFS_H__
#define __SYMBOID_ASTRO_SWEPH_DEFS_H__

#include "sdk/defs.h"

#ifdef BUILD_ASTRO_SWEPH
    #define ASTRO_SWEPH_API SY_API_EXPORT
#else
    #define ASTRO_SWEPH_API SY_API_IMPORT
#endif

#define swe_ns_begin sy_ns_begin(swe)
#define swe_ns_end sy_ns_end

#endif // __SYMBOID_ASTRO_SWEPH_DEFS_H__
