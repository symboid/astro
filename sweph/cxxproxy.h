
#ifndef __SYMBOID_ASTRO_EPHE_SWEPH_CXXPROXY_H__
#define __SYMBOID_ASTRO_EPHE_SWEPH_CXXPROXY_H__

#include "astro/sweph/defs.h"
#include "astro/sweph/src/swephexp.h"
#include <string>
#include <chrono>
#include "astro/eph/ecliptic.h"
#include "astro/eph/calendarcoords.h"

swe_ns_begin

struct ASTRO_SWEPH_API proxy
{
    static void set_eph_dir_path(const std::string& _eph_dir_path);

    struct ASTRO_SWEPH_API clock
    {
        typedef std::chrono::duration<double, std::ratio<86400>> duration;
        typedef typename duration::rep rep;
        typedef typename duration::period period;
        typedef typename std::chrono::time_point<clock> time_point;
        static time_point time(const eph::calendar_coords& _calendar_coords);
    };

    struct ASTRO_SWEPH_API object
    {
        enum class calc_type
        {
            SWISSEPH,
            JPL,
            MOSHIER,

            DEFAULT = SWISSEPH,
        };
        static void set_calc_type(calc_type _calc_type);

        enum index
        {
            sun = SE_SUN,
            moon = SE_MOON,
            mercury = SE_MERCURY,
            venus = SE_VENUS,
            mars = SE_MARS,
            jupiter = SE_JUPITER,
            saturn = SE_SATURN,
            uranus = SE_URANUS,
            neptune = SE_NEPTUNE,
            pluto = SE_PLUTO,

            dragon_head = SE_MEAN_NODE,
            lilith = SE_MEAN_APOG,
        };
        static eph::calc_result calc_pos(index _object_index, clock::time_point _time,
                eph::ecl_pos& _ecl_pos, eph::ecl_speed& _ecl_speed);
    };

    struct ASTRO_SWEPH_API houses
    {
        enum class type : int
        {
            placidus = 'P',
            koch = 'K',
            regiomontanus = 'R',
            campanus = 'C',
            equal = 'E',
        };
        static eph::calc_result calc(clock::time_point _time, type _houses_type,
                eph::arc_degree _geo_lont, eph::arc_degree _geo_latt, eph::ecl_lont* _ecl_lont_buffer);
    };
};

swe_ns_end

typedef swe::proxy eph_proxy;

#endif // __SYMBOID_ASTRO_EPHE_SWEPH_CXXPROXY_H__
