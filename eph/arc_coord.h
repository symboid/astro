
#ifndef __SYMBOID_ASTRO_EPH_ARC_COORD_H__
#define __SYMBOID_ASTRO_EPH_ARC_COORD_H__

#include "astro/eph/defs.h"
#include <cmath>

eph_ns_begin

typedef double arc_degree;

static constexpr arc_degree arc_degree_epsilon = 1.0 / 3600.0;

inline bool static arc_degree_equals(arc_degree _left, arc_degree _right)
{
    return std::abs(_left - _right) < arc_degree_epsilon;
}

struct arc_coord
{
    typedef int signum;
    typedef int degree;
    typedef int minute;
    typedef arc_degree second;

    arc_coord(arc_degree _arc_pos)
        : _M_signum(_arc_pos < 0.0 ? -1 : 1)
        , _M_degree(int(std::floor(_arc_pos *= _M_signum)) % 360)
        , _M_minute(int(std::floor((_arc_pos - _M_degree) * 60.0)))
        , _M_second((_arc_pos - _M_degree) * 3600.0 - _M_minute * 60.0)
    {
    }

    const signum _M_signum;
    const degree _M_degree;
    const minute _M_minute;
    const second _M_second;

    inline static arc_degree calc_arc_pos(signum _signum, degree _degree, minute _minute, second _second)
    {
        if (_degree < 0.0)
        {
            _signum = -1;
            _degree = -_degree;
        }
        return _signum * (arc_degree(_degree) + arc_degree(_minute) / 60.0 + arc_degree(_second) / 3600.0);
    }
    arc_degree arc_pos() const
    {
        return calc_arc_pos(_M_signum, _M_degree, _M_minute, _M_second);
    }

    static constexpr second second_epsilon = 1.0 / 1000.0;
    inline static bool second_equals(second _left, second _right)
    {
        return std::abs(_left - _right) < second_epsilon;
    }

    inline arc_degree static unpack(int _degree, int _minute, arc_degree _second = 0.0)
    {
        return arc_degree(_degree) + (arc_degree(_minute) * 60.0 +  _second) / 3600.0;
    }
};

eph_ns_end

#endif // __SYMBOID_ASTRO_EPH_ARC_COORD_H__
