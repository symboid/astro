
#ifndef __SYMBOID_ASTRO_CALCULO_HORA_H__
#define __SYMBOID_ASTRO_CALCULO_HORA_H__

#include "astro/calculo/defs.h"
#include "astro/calculo/planet.h"
#include "astro/eph/houses.h"
#include "astro/eph/fixstar.h"
#include <list>
#include <set>
#include <vector>

namespace hor {

struct hora_coords
{
    eph::calendar_coords _M_calendar_coords;
    std::chrono::minutes _M_time_zone_diff;
    eph::arc_degree _M_geo_lont;
    eph::arc_degree _M_geo_latt;
};

template <class _EphProxy>
class basic_hora
{
private:
    std::vector<eph::house_cusp> _M_houses;
public:
    const std::vector<eph::house_cusp>& houses() const { return _M_houses; }
public:
    template <class _HouseSystem>
    eph::calc_result calc(const hora_coords& _hora_coords)
    {
        eph::basic_time_point<_EphProxy> hora_time =
                eph::basic_calendar<_EphProxy>::time(_hora_coords._M_calendar_coords);
        hora_time -= _hora_coords._M_time_zone_diff;

        eph::calc_result calc_result = eph::calc_result::SUCCESS;
        calc_result = eph::calc_houses<_EphProxy, _HouseSystem>(_M_houses, hora_time,
                 _hora_coords._M_geo_lont, _hora_coords._M_geo_latt);

        typename planet_list::iterator pIt = _M_planets.begin();
        while (calc_result == eph::calc_result::SUCCESS && pIt != _M_planets.end())
        {
            calc_result = pIt->calc_pos(hora_time);
            pIt++;
        }
        return calc_result;
    }

public:
    typedef std::vector<basic_planet<_EphProxy>> planet_list;
    typedef typename planet_list::iterator planet_it;
    typedef typename planet_list::const_iterator planet_it_const;
private:
    planet_list _M_planets;
public:
    void add_planet(const basic_planet<_EphProxy>& _planet)
    {
        _M_planets.push_back(_planet);
    }
    const planet_list& planets() const { return _M_planets; }
    planet_it_const planetsBegin() const
    {
        return _M_planets.begin();
    }
    planet_it planetsBegin()
    {
        return _M_planets.begin();
    }
    planet_it_const planetsEnd() const
    {
        return _M_planets.end();
    }
    planet_it planetsEnd()
    {
        return _M_planets.end();
    }
    size_t planet_count() const { return _M_planets.size(); }
    const basic_planet<_EphProxy>& planet(size_t _planet_index) const { return _M_planets.at(_planet_index); }

public:
    typedef std::list<eph::basic_fixstar<_EphProxy>> fixstars;
    typedef typename fixstars::const_iterator fixstar_const_it;
private:
    fixstars _M_fixstars;
public:
    fixstar_const_it fixstars_begin() const { return _M_fixstars.begin(); }
    fixstar_const_it fixstars_end() const { return _M_fixstars.end(); }
    template <class _Fixstars>
    eph::calc_result calc_fixstars(const hora_coords& _hora_coords, _Fixstars& _fixstars)
    {
        eph::basic_time_point<_EphProxy> hora_time =
                eph::basic_calendar<_EphProxy>::time(_hora_coords._M_calendar_coords);
        hora_time -= _hora_coords._M_time_zone_diff;

        _M_fixstars.clear();

        eph::calc_result result = eph::calc_result::SUCCESS;

        typedef typename _Fixstars::Container::iterator Iterator;
        for (Iterator fixstar_data = _fixstars.begin(), f_end = _fixstars.end();
             result == eph::calc_result::SUCCESS && fixstar_data != f_end; ++fixstar_data)
        {
            if (_fixstars.filter_match(*fixstar_data))
            {
                eph::basic_fixstar<eph_proxy> fixstar(fixstar_data->get());
                result = fixstar.calc_pos(hora_time);
                if (result == eph::calc_result::SUCCESS)
                {
                    bool conjuncting = false;
                    const eph::ecl_pos fixstar_pos = fixstar.pos();
                    const hor::orbis fixstar_orbis = (*fixstar_data)->orbis();
                    for (planet_it_const planet = _M_planets.begin(), p_end = _M_planets.end();
                            !conjuncting && planet != p_end; ++planet)
                    {
                        conjuncting = (planet->pos().dist_abs(fixstar_pos) < fixstar_orbis);
                    }
//                    for (std::vector<eph::house_cusp>::const_iterator house = _M_houses.begin(), h_end = _M_houses.end();
//                            !conjuncting && house != h_end; ++house)
//                    {
//                        conjuncting = (house->pos().dist_abs(fixstar_pos) < fixstar_orbis);
//                    }
                    if (conjuncting)
                    {
                        _M_fixstars.push_back(fixstar);
                    }
                }
            }
        }
        return result;
    }
};

}

#endif // __SYMBOID_ASTRO_CALCULO_HORA_H__
