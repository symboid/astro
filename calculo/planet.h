
#ifndef __SYMBOID_ASTRO_CALCULO_PLANET_H__
#define __SYMBOID_ASTRO_CALCULO_PLANET_H__

#include "astro/calculo/defs.h"
#include "astro/eph/object.h"
#include <memory>
#include "astro/eph/proxy.h"

hor_ns_begin

typedef eph::arc_degree orbis;

enum aspect_type
{
    none_aspect = 0,

    conjunction = 1,
    opposition = 2,
    trigon = 3,
    quadrat = 4,
    quintile = 5,
    sextile = 6,

    semi_sextile = 7,
    quincunx = 8,
    semi_quadrat = 9,
    sesqui_quadrat = 10,
    biquintile = 11,

    aspect_type_count = 12,
};

const hor::orbis aspect_dist[aspect_type_count] =
{
    0, 0.0, 180.0, 120.0, 90.0, 72.0, 60.0,
    30.0, 150.0, 45.0, 135.0, 144.0
};

inline aspect_type& operator++(aspect_type& _aspect_type)
{
    _aspect_type = aspect_type(int(_aspect_type) + 1);
    return _aspect_type;
}

class a_orbis_config
{
public:
    virtual ~a_orbis_config() = default;
public:
    virtual orbis aspect_orbis(aspect_type _aspect_type) const = 0;
};

class simple_orbis_config : public a_orbis_config
{
public:
    simple_orbis_config(orbis _main_orbis) : _M_main_orbis(_main_orbis) {}
private:
    const orbis _M_main_orbis;
public:
    orbis aspect_orbis(aspect_type _aspect_type) const override
    {
        switch (_aspect_type) {
        case conjunction: case opposition: return _M_main_orbis;
        case trigon: case sextile: case quadrat: return _M_main_orbis * 0.75;
        case none_aspect: return 0.0;
        default: return 1.0;
        }
    }
};

enum class aspect_conn_type
{
    none,
    applicating,
    separating,
};

template <class _EphProxy>
class basic_planet : public eph::basic_object<_EphProxy>
{
public:
    typedef typename _EphProxy::object::index index;
    static constexpr const index sun = _EphProxy::object::sun;
    static constexpr const index moon = _EphProxy::object::moon;
    static constexpr const index mercury = _EphProxy::object::mercury;
    static constexpr const index venus = _EphProxy::object::venus;
    static constexpr const index mars = _EphProxy::object::mars;
    static constexpr const index jupiter = _EphProxy::object::jupiter;
    static constexpr const index saturn = _EphProxy::object::saturn;
    static constexpr const index uranus = _EphProxy::object::uranus;
    static constexpr const index neptune = _EphProxy::object::neptune;
    static constexpr const index pluto = _EphProxy::object::pluto;
    static constexpr const index chiron = _EphProxy::object::chiron;

    static constexpr const index dragon_head = _EphProxy::object::dragon_head;
    static constexpr const index lilith = _EphProxy::object::lilith;

public:
    basic_planet(index _planet_index, const a_orbis_config* _orbis_config)
        : _M_index(_planet_index)
        , _M_orbis_config(_orbis_config)
    {
    }

private:
    index _M_index;
public:
    index get_index() const { return _M_index; }

    eph::calc_result calc_pos(const eph::basic_time_point<_EphProxy>& _time) override
    {
        return _EphProxy::object::calc_pos(_M_index, _time, this->_M_ecl_pos, this->_M_ecl_speed);
    }

private:
    std::shared_ptr<const a_orbis_config> _M_orbis_config;
public:
    orbis aspect_orbis(aspect_type _aspect_type) const
    {
        return _M_orbis_config ? _M_orbis_config->aspect_orbis(_aspect_type) : 1.0;
    }

    template <class _MagPoint>
    aspect_conn_type aspect_conn_type(const _MagPoint&) const
    {
        return aspect_conn_type::none;
    }

    template <class _MagPoint>
    aspect_type aspect_conn(const _MagPoint& _mag_point) const
    {
        hor::orbis dist = this->pos().dist_abs(_mag_point.pos()).to_arc_degree();

        aspect_type a = none_aspect, aspect_conn = none_aspect;
        while (aspect_conn == none_aspect && ++a < aspect_type_count)
        {
            hor::orbis aspect_orbis = this->aspect_orbis(a) + _mag_point.aspect_orbis(a);
            hor::orbis dist_min = dist - aspect_orbis, dist_max = dist + aspect_orbis;
            if (dist_min < aspect_dist[a] && aspect_dist[a] < dist_max)
            {
                aspect_conn = a;
            }
        }
        return  aspect_conn;
    }
};

typedef basic_planet<eph_proxy> planet;

hor_ns_end

#endif // __SYMBOID_ASTRO_CALCULO_PLANET_H__
