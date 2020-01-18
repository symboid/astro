
#ifndef __SYMBOID_ASTRO_EPH_ECLIPTIC_H__
#define __SYMBOID_ASTRO_EPH_ECLIPTIC_H__

#include "astro/eph/defs.h"
#include "astro/eph/arc_coord.h"
#include <cmath>

eph_ns_begin


/// Type for ecliptic longitude.
class ecl_lont
{
public:
    ecl_lont(arc_degree _lont_value = 0.0, bool _is_signed = false)
        : _M_value_360(_lont_value)
        , _M_is_signed(_is_signed)
    {
        arc_degree lower_bound = (_M_is_signed ? -180.0 : 0.0), upper_bound = lower_bound + 360.0;
        while (_M_value_360 >= upper_bound)
        {
            _M_value_360 -= 360.0;
        }
        while (_M_value_360 < lower_bound)
        {
            _M_value_360 += 360.0;
        }
    }

    operator arc_degree() const
    {
        return _M_value_360;
    }
    arc_degree to_arc_degree() const
    {
        return _M_value_360;
    }

    ecl_lont operator+(const arc_degree& _arc_degree)
    {
        return ecl_lont(_M_value_360 + _arc_degree, _M_is_signed);
    }
    ecl_lont operator-(const arc_degree& _arc_degree)
    {
        return ecl_lont(_M_value_360 - _arc_degree, _M_is_signed);
    }
    ecl_lont& operator+=(const arc_degree& _arc_degree)
    {
        *this = *this + _arc_degree;
        return *this;
    }
    ecl_lont& operator-=(const arc_degree& _arc_degree)
    {
        *this = *this - _arc_degree;
        return *this;
    }

private:
    arc_degree _M_value_360;
    bool _M_is_signed;
};

/**
 * @brief The ecliptic coordinate structure.
 *
 * The ecliptic coordinate consits of three parts: ecliptic lattitude, ecliptic longitude and
 * the distance from Earth. The distance can be zero, that means undefined value (for example
 * the ecliptic position of a house cusp).
 */
struct ecl_coords
{
    typedef ecl_lont lont;
    /// Typedef alias for ecliptic lattitude.
    typedef arc_degree latt;
    /// Typedef alias for the distance from Earth.
    typedef double dist;

    /// Default constructor.
    ecl_coords()
        : _M_lont(0.0), _M_latt(0.0), _M_dist(0.0)
    {
    }

    /// Constructor with the three coordinates.
    ecl_coords(lont _lont, latt _latt, dist _dist = dist(0.0))
        : _M_lont(_lont), _M_latt(_latt), _M_dist(_dist)
    {
    }

    /// The ecliptic lattitude measured in degrees.
    lont _M_lont;
    /// The ecliptic longitude measured in degrees.
    latt _M_latt;
    /// The distance from Earth measured in astronomical units.
    dist _M_dist;

    /// Getting ecliptic longitude in form of Sy::ArcCoord.
    inline arc_coord lont_coord() const { return arc_coord(_M_lont); }
    /// Getting ecliptic longitude in form of Sy::ArcCoord.
    inline arc_coord latt_coord() const { return arc_coord(_M_latt); }
};

enum class zod
{
    NON = 0,

    ARI =  1, TAU =  2, GEM =  3,
    CAN =  4, LEO =  5, VIR =  6,
    LIB =  7, ESC =  8, SAG =  9,
    CAP = 10, AQU = 11, PIS = 12,

    COUNT = 12,
};

struct zod_coords : arc_coord
{
    static zod sign(arc_degree _arc_pos)
    {
        int degree(int(std::floor(_arc_pos)) % 360);
        return zod((degree / SIGN_SIZE) + 1);
    }
    static arc_degree sign_pos(arc_degree _arc_pos)
    {
        while (_arc_pos > SIGN_SIZE) { _arc_pos -= SIGN_SIZE; }
        return _arc_pos;
    }
    zod_coords(arc_degree _arc_pos)
        : arc_coord(sign_pos(_arc_pos))
        , _M_sign(sign(_arc_pos))
    {
    }

    const zod _M_sign;

    static constexpr int SIGN_SIZE = 30;
};

struct ecl_pos : ecl_coords
{
    ecl_pos() {}
    ecl_pos(lont _lont, latt _latt = 0.0, dist _dist = 0.0)
        : ecl_coords(_lont, _latt, _dist)
    {
    }

    eph::zod_coords zod_coords() const { return eph::zod_coords(_M_lont); }

    lont dist_to(const ecl_pos& _rhs) const
    {
        return lont(static_cast<arc_degree>(_M_lont - _rhs._M_lont), true);
    }
    lont dist_abs(const ecl_pos& _rhs) const
    {
        arc_degree lhs_lont = _M_lont, rhs_lont = _rhs._M_lont;
        arc_degree dist = (lhs_lont < rhs_lont) ? rhs_lont - lhs_lont : lhs_lont - rhs_lont;
        if (dist > 180.0)
        {
            dist = 360.0 - dist;
        }
        return dist;
    }
    lont dist_fwd(const ecl_pos& _rhs) const
    {
        return lont(static_cast<arc_degree>(_rhs._M_lont - _M_lont), false);
    }
    lont dist_bwd(const ecl_pos& _rhs) const
    {
        return lont(static_cast<arc_degree>(_M_lont - _rhs._M_lont), false);
    }
};

struct ecl_speed
{
    typedef arc_degree lont;
    typedef arc_degree latt;

    ecl_speed()
        : _M_lont(0.0), _M_latt(0.0)
    {
    }

    ecl_speed(lont _lont, latt _latt)
        : _M_lont(_lont), _M_latt(_latt)
    {
    }

    lont _M_lont;
    latt _M_latt;

    inline arc_coord lont_coord() const { return arc_coord(_M_lont); }
    inline arc_coord latt_coord() const { return arc_coord(_M_latt); }
};

eph_ns_end

#endif // __SYMBOID_ASTRO_EPH_ECLIPTIC_H__
