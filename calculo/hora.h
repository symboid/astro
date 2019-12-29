
#ifndef __SYMBOID_ASTRO_CALCULO_HORA_H__
#define __SYMBOID_ASTRO_CALCULO_HORA_H__

#include "astro/calculo/defs.h"
#include "astro/ephe/jultime.h"
#include "astro/ephe/houses.h"
#include "astro/calculo/magpoint.h"
#include "astro/calculo/planet.h"
#include <list>
#include <set>

namespace As {

class HoraHouses
{
public:
    HoraHouses();

public:
    class HouseCusp : public MagPoint
    {
    public:
        HouseCusp();
        HouseCusp(const As::Houses* houses, As::Houses::HouseIndex houseIndex);

    private:
        const As::Houses* mHouses;
        As::Houses::HouseIndex mHouseIndex;

    public:
        Sy::String name() const override;
        EclPos eclPos() const override;
        EclSpeed eclSpeed() const override;
    };
protected:
    As::Houses mHouses;
    HouseCusp mHouseCusps[As::Houses::HOUSE_COUNT + 1];
public:
    static constexpr Houses::HouseIndex HOUSES_BEGIN = Houses::BEGIN_INDEX;
    static constexpr Houses::HouseIndex HOUSES_END = Houses::END_INDEX;
    static constexpr int HOUSES_COUNT = Houses::HOUSE_COUNT;
    const HouseCusp& houseCusp(Houses::HouseIndex houseIndex) const;
};

class HoraPlanets
{
protected:
    HoraPlanets();

public:
    typedef std::list<Planet> Planets;
    typedef Planets::const_iterator ConstPlanet;
protected:
    Planets mPlanets;
public:
    void addPlanet(const Planet& planet);
    ConstPlanet beginPlanet() const { return mPlanets.begin(); }
    ConstPlanet endPlanet() const { return mPlanets.end(); }

public:
    typedef std::set<Aspect::Type> AspectTypes;
    typedef AspectTypes::const_iterator ConstAspectType;
protected:
    AspectTypes mAspectTypes;
    std::list<AspectConnection> mAspectConnections;
public:
    void addAspectType(const Aspect::Type& aspectType);
    ConstAspectType beginAspectType() const { return mAspectTypes.begin(); }
    ConstAspectType endAspecType() const { return mAspectTypes.end(); }
};

class HoraZodiac
{
public:
    HoraZodiac();

public:
    class SignBorder : public MagPoint
    {
    public:
        SignBorder(ZodSign zodSign = Z_NON);
    private:
        ZodSign mZodSign;

    public:
        Sy::String name() const override;
        EclPos eclPos() const override;
        EclSpeed eclSpeed() const override;
    };

public:
    static constexpr int ZOD_COUNT = Z_COUNT;
protected:
    SignBorder mSignBorders[ZOD_COUNT];
public:
    const SignBorder& zodSignBorder(int signBorderIndex) const;
};

class Hora : public HoraHouses, public HoraPlanets, public HoraZodiac
{
public:
    CalcResult calc(const Time& horaTime, Sy::ArcDegree geoLont, Sy::ArcDegree geoLatt,
            As::Houses::Type housesType);
};

} // namespace As

#include "astro/eph/houses.h"
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
    typedef std::list<basic_planet<_EphProxy>> planet_list;
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
};

}

#endif // __SYMBOID_ASTRO_CALCULO_HORA_H__
