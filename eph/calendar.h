
#ifndef __SYMBOID_ASTRO_EPH_CALENDAR_H__
#define __SYMBOID_ASTRO_EPH_CALENDAR_H__

#include "astro/eph/defs.h"
#include <chrono>
#include "astro/eph/calendarcoords.h"

eph_ns_begin

template <class _EphProxy>
struct basic_calendar
{
    typedef typename _EphProxy::clock clock;
    typedef std::chrono::time_point<clock> time_point;

    typedef typename clock::duration days;
    static constexpr days ONE_DAY = days(1.0);

    static time_point time(const calendar_coords& _calendar_coords)
    {
        return clock::time(_calendar_coords);
    }
};

template <class _EphProxy>
using basic_time_point = typename basic_calendar<_EphProxy>::time_point;

eph_ns_end

#endif // __SYMBOID_ASTRO_EPH_CALENDAR_H__
