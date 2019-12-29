
#ifndef __SYMBOID_ASTRO_EPH_CALENDARCOORDS_H__
#define __SYMBOID_ASTRO_EPH_CALENDARCOORDS_H__

#include "astro/eph/defs.h"

eph_ns_begin

enum class calendar_type
{
    GREGORIAN,
    JULIAN,
};

struct calendar_coords
{
    int _M_year = 1900, _M_month = 1, _M_day = 1;
    int _M_hour = 0, _M_minute = 0, _M_second = 0;

    calendar_type _M_calendar_type = calendar_type::GREGORIAN;
};

eph_ns_end

#endif // __SYMBOID_ASTRO_EPH_CALENDARCOORDS_H__
