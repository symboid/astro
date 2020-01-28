
#ifndef __SYMBOID_ASTRO_EPH_OBJECT_H__
#define __SYMBOID_ASTRO_EPH_OBJECT_H__

#include "astro/eph/defs.h"
#include "astro/eph/ecliptic.h"
#include "astro/eph/calendar.h"
#include "astro/eph/proxy.h"
#include <string>
#include <algorithm>

eph_ns_begin


template <typename _EphProxy>
class basic_object
{
public:
    typedef typename _EphProxy::object::index index;

public:
    basic_object(const index& _index) : _M_index(_index) {}

private:
    index _M_index;
public:
    index get_index() const { return _M_index; }

private:
    ecl_pos _M_ecl_pos;
public:
    ecl_pos pos() const { return _M_ecl_pos; }

private:
    ecl_speed _M_ecl_speed;
public:
    ecl_speed speed() const { return _M_ecl_speed; }
    bool is_retrograd() const { return _M_ecl_speed._M_lont < 0.0; }

public:
    calc_result calc_pos(const basic_time_point<_EphProxy>& _time)
    {
        return _EphProxy::object::calc_pos(_M_index, _time, _M_ecl_pos, _M_ecl_speed);
    }
};

template <class _EphProxy, typename _EphProxy::object::index _object_index>
class static_object : public basic_object<_EphProxy>
{
public:
    static_object() : basic_object<_EphProxy>(_object_index) {}
};

template <class _EphProxy>
using SUN = static_object<_EphProxy, _EphProxy::object::index::sun>;

template <class _EphProxy>
calc_result approx_transit_pos(basic_object<_EphProxy>& object, basic_time_point<_EphProxy> approx_time, const ecl_pos& _prec_pos, const ecl_pos& _succ_pos)
{
    typedef typename basic_calendar<_EphProxy>::days days;
    constexpr days ONE_DAY = basic_calendar<_EphProxy>::ONE_DAY;

    static constexpr ecl_pos::dist DIST_TOLERANCE = 1.0 / 3600.0;

    while (object.calc_pos(approx_time) == calc_result::SUCCESS &&
           object.pos().dist_abs(_prec_pos) > DIST_TOLERANCE &&
           object.pos().dist_abs(_succ_pos) > DIST_TOLERANCE)
    {
        // speed of object at current position (measured in degrees/day):
        ecl_speed::lont speed(object.speed()._M_lont);
        const ecl_pos& approx_pos = speed > 0.0 ? _succ_pos : _prec_pos;
        days estm_days(object.pos().dist_to(approx_pos) / speed);

        // stepping forward in time with estimated amount of time (truncated to one day):
        approx_time += std::clamp(estm_days, -ONE_DAY, ONE_DAY);

    }
    return calc_result::SUCCESS;
}

template <class _EphProxy>
calc_result approx_transit_pos(basic_object<_EphProxy>& object, basic_time_point<_EphProxy>& conj_time, const ecl_pos& _transit_pos)
{
    typedef typename basic_calendar<_EphProxy>::days days;
    constexpr days ONE_DAY = basic_calendar<_EphProxy>::ONE_DAY;

    static constexpr ecl_pos::dist DIST_TOLERANCE = 1.0 / 3600.0;

    calc_result calc_result = calc_result::FAILED;
    while ((calc_result = object.calc_pos(conj_time)) == calc_result::SUCCESS && object.pos().dist_abs(_transit_pos) > DIST_TOLERANCE)
    {
        // speed of object at current position is measured in degrees/day:
        // distance to transit position in ecliptic degree:
        days estm_days(object.pos().dist_to(_transit_pos) / object.speed()._M_lont);

        // stepping forward in time with estimated amount of days:
        conj_time += std::clamp(estm_days, -ONE_DAY, ONE_DAY);
    }

    return calc_result;
}

template <class _EphProxy>
calc_result approx_transit_raw(basic_object<_EphProxy> object, basic_time_point<_EphProxy>& raw_time, const ecl_pos& _transit_pos)
{
    typedef typename basic_calendar<_EphProxy>::days days;
    constexpr days ONE_DAY = basic_calendar<_EphProxy>::ONE_DAY;

    calc_result calc_result = calc_result::FAILED;
    ecl_coords::dist prev_dist = 360.0;
    ecl_coords::dist dist_fwd = 0;
    // TODO: retrograd !!!
    while ((calc_result = object.calc_pos(raw_time)) == calc_result::SUCCESS && prev_dist > (dist_fwd = object.pos().dist_fwd(_transit_pos)))
    //while (prevDist > distFwd)
    {
        // speed of object at current position (measured in degrees/day):
        days estm_days(ONE_DAY);

        // stepping forward in time with estimated amount of days:
        raw_time += estm_days;

        prev_dist = dist_fwd;

        // recalculation of transit position:
        //successCalc = calcPos(signTime);
        //distFwd = mEclPos.distFwd(transitPos);
    }

    return calc_result;
}

template <class _EphProxy>
calc_result calc_transit_time(const basic_object<_EphProxy>& object, basic_time_point<_EphProxy>& transit_time, const ecl_pos& _transit_pos)
{
    basic_object<_EphProxy> transit_object(object.get_index());
    return approx_transit_raw(transit_object, transit_time, _transit_pos) == calc_result::SUCCESS
        && approx_transit_pos(transit_object, transit_time, _transit_pos) == calc_result::SUCCESS
         ? calc_result::SUCCESS : calc_result::FAILED;
}

eph_ns_end

#endif // __SYMBOID_ASTRO_EPH_OBJECT_H__
