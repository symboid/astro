
#ifndef __SYMBOID_ASTRO_EPH_DEFS_H__
#define __SYMBOID_ASTRO_EPH_DEFS_H__

#include "sdk/arch/defs.h"

#define eph_ns_begin sy_ns_begin(eph)
#define eph_ns_end sy_ns_end

eph_ns_begin

enum class calc_result
{
    SUCCESS,
    FAILED,
};

eph_ns_end

#endif // __SYMBOID_ASTRO_EPH_DEFS_H__
