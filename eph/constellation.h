
#ifndef __SYMBOID_ASTRO_EPH_CONSTELLATION_H__
#define __SYMBOID_ASTRO_EPH_CONSTELLATION_H__

#include "astro/eph/defs.h"
#include <string>
#include "astro/eph/ecliptic.h"
#include "astro/eph/calendarcoords.h"

eph_ns_begin

class constellation
{
public:
    constellation(const std::string& _name, const std::string& _alpha_name,
            arc_degree _alpha_rel_pos, arc_degree _length)
        : _M_name(_name)
        , _M_alpha_name(_alpha_name)
        , _M_alpha_rel_pos(_alpha_rel_pos)
        , _M_length(_length)
    {
    }

public:
    const std::string _M_name;
    const std::string _M_alpha_name;
    const arc_degree _M_alpha_rel_pos;
    const arc_degree _M_length;

public:
    virtual ecl_pos alpha_pos() const = 0;
    ecl_lont begin_lont() const { return alpha_pos()._M_lont - _M_alpha_rel_pos; }
    ecl_lont end_lont() const { return begin_lont() + _M_length; }
};

template <class _EphProxy, class _ConstellationAttributes>
class basic_constellation : public constellation
{
public:
    basic_constellation()
        : constellation(_ConstellationAttributes::name, _ConstellationAttributes::alpha_name,
                        _ConstellationAttributes::alpha_rel_pos, _ConstellationAttributes::length)
    {
        calc_alpha_pos({1950,1,1});
    }

private:
    ecl_pos _M_alpha_pos;
    ecl_speed _M_alpha_speed;
public:
    ecl_pos alpha_pos() const override { return _M_alpha_pos; }
    ecl_pos calc_alpha_pos(const calendar_coords& _time)
    {
        calc_result calc_result = _EphProxy::fixstar::calc_pos(
                    this->_M_alpha_name, _EphProxy::clock::time(_time), _M_alpha_pos, _M_alpha_speed);
        if (calc_result != calc_result::SUCCESS)
        {
            _M_alpha_pos = ecl_pos(0,0,0);
            _M_alpha_speed = ecl_speed(0, 0);
        }
        return _M_alpha_pos;
    }
};

template <int _degree, int _minute, int _second>
struct pack_arc_degree
{
    static constexpr arc_degree value = arc_degree(_degree) + (arc_degree(_minute * 60.0 + _second) / 3600.0);
};

struct taurus
{
    static constexpr const char* name = "Taurus";
    static constexpr const char* alpha_name = "Aldebaran";
    static constexpr const arc_degree alpha_rel_pos = pack_arc_degree<18,37,0>::value;
    static constexpr const arc_degree length = pack_arc_degree<33,37,0>::value;
};

eph_ns_end

#endif // __SYMBOID_ASTRO_EPH_CONSTELLATION_H__
