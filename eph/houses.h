
#ifndef __SYMBOID_ASTRO_EPH_HOUSES_H__
#define __SYMBOID_ASTRO_EPH_HOUSES_H__

#include "astro/eph/defs.h"
#include "astro/eph/ecliptic.h"
#include "astro/eph/calendar.h"
#include "astro/eph/proxy.h"
#include <vector>

eph_ns_begin

class house_cusp
{
public:
    house_cusp() : house_cusp(~0U,"") {}
    house_cusp(std::size_t _index, const std::string& _name) : _M_index(_index), _M_name(_name), _M_lont(0.0), _M_speed(0.0) {}

protected:
    const std::size_t _M_index;
private:
    const std::string _M_name;
    ecl_lont _M_lont;
    ecl_lont _M_speed;
public:
    ecl_pos pos() const { return _M_lont; }
    void set_lont(const ecl_lont& _ecl_lont) { _M_lont = _ecl_lont; }

    ecl_speed speed() const { return ecl_speed(_M_speed, 0.0); }
    void set_speed(const ecl_lont& _ecl_speed) { _M_speed = _ecl_speed; }

    const std::string& name() const { return _M_name; }
};

struct house_system_horizon {
    static constexpr std::size_t house_count = 12;
    typedef house_cusp cusp;
    static constexpr const char* house_names [house_count + 1] =
    {
        "",
        "I", "2", "3", "IV", "5", "6",
        "VII", "8", "9", "X", "11", "12"
    };
};

struct house_system_placidus : house_system_horizon
{
    template <class _EphProxy>
    static calc_result calc(const basic_time_point<_EphProxy>& _time, arc_degree _geo_lont, arc_degree _geo_latt, ecl_lont _lonts[house_count])
    {
        return _EphProxy::houses::calc(_time, _EphProxy::houses::type::placidus, _geo_lont, _geo_latt, _lonts);
    }
};

class zod_sign_cusp : public house_cusp
{
public:
    zod_sign_cusp() {}
    zod_sign_cusp(std::size_t _index, const std::string& _name)
        : house_cusp(_index, _name)
    {
        if (0 < _index && _index <= 12)
        {
            set_lont((_index - 1) * 30.0);
        }
    }
};

struct house_system_mundan {
    static constexpr std::size_t house_count = 12;
    typedef zod_sign_cusp cusp;
    static constexpr const char* house_names[house_count + 1] =
    {
        "",
        "Ari", "Tau", "Gem", "Can", "Leo", "Vir",
        "Lib", "Esc", "Sag", "Cap", "Aqu", "Pis"
    };

    template <class _EphProxy>
    static calc_result calc(const basic_time_point<_EphProxy>&, arc_degree, arc_degree, ecl_lont _lonts[house_count])
    {
        for (std::size_t house_index = 1; house_index <= 12; ++house_index)
        {
            _lonts[house_index] = ((house_index - 1) * 30.0);
        }
        return calc_result::SUCCESS;
    }
};

template <class _EphProxy, class _HouseSystem>
calc_result calc_houses(std::vector<house_cusp>& _houses, const basic_time_point<_EphProxy>& _time, arc_degree _geo_lont, arc_degree _geo_latt)
{
    static constexpr std::size_t house_count = _HouseSystem::house_count;

    _houses.reserve(house_count + 1);
    _houses.clear();
    _houses.push_back(house_cusp());
    for (std::size_t house_index = 1; house_index <= house_count; ++house_index)
    {
        _houses.push_back(house_cusp(house_index, _HouseSystem::house_names[house_index]));
    }

    calc_result calc_result;
    ecl_lont house_cusp_lonts[house_count + 1];
    calc_result = _HouseSystem::template calc<_EphProxy>(_time, _geo_lont, _geo_latt, house_cusp_lonts);
    for (std::size_t house_index = 0; house_index <= house_count; ++house_index)
    {
        _houses.at(house_index).set_lont(house_cusp_lonts[house_index]);
    }

    static constexpr typename basic_calendar<_EphProxy>::days time_diff(10.0 / 1440.0);
    ecl_lont house_cusp_lonts_next[house_count + 1];
    calc_result = _HouseSystem::template calc<_EphProxy>(_time + time_diff, _geo_lont, _geo_latt, house_cusp_lonts_next);
    _houses.at(0).set_speed(0.0);
    for (std::size_t house_index = 1; house_index <= house_count; ++house_index)
    {
        ecl_lont house_lont_diff = house_cusp_lonts_next[house_index] - house_cusp_lonts[house_index];
        _houses.at(house_index).set_speed(house_lont_diff / time_diff.count());
    }
    return calc_result;
}

eph_ns_end

#endif // __SYMBOID_ASTRO_EPH_HOUSES_H__
