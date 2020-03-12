
#ifndef __SYMBOID_ASTRO_CALCULO_PLANET_H__
#define __SYMBOID_ASTRO_CALCULO_PLANET_H__

#include "astro/calculo/defs.h"
#include "astro/eph/object.h"

hor_ns_begin

typedef eph::arc_degree orbis;

typedef int aspect_type;

static constexpr const aspect_type none_aspect = 0;
static constexpr const aspect_type conjunction = 1;
static constexpr const aspect_type opposition = 2;
static constexpr const aspect_type trigon = 3;
static constexpr const aspect_type quadrat = 4;
static constexpr const aspect_type quintil = 5;
static constexpr const aspect_type sextil = 6;

static constexpr const aspect_type aspect_type_count = 7;

enum class aspect_conn_type
{
    NONE,
    APPLICATING,
    SEPARATING,
};

template <class _EphProxy>
class basic_planet : public eph::basic_object<_EphProxy>
{
public:
    typedef typename eph::basic_object<_EphProxy>::index index;
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

    static constexpr const index dragon_head = _EphProxy::object::dragon_head;
    static constexpr const index lilith = _EphProxy::object::lilith;

public:
    basic_planet(index _planet_index, orbis _main_orbis)
        : eph::basic_object<_EphProxy>(_planet_index)
        , _M_main_orbis(_main_orbis)
    {
    }

private:
    orbis _M_main_orbis;
public:
    orbis main_orbis() const { return _M_main_orbis; }
    orbis aspect_orbis(aspect_type _aspect_type) const
    {
        switch (_aspect_type) {
        case conjunction: case opposition: return main_orbis();
        case trigon: case sextil: case quadrat: return main_orbis() * 0.75;
        case none_aspect: return 0.0;
        default: return 1.0;
        }
    }

    template <class _MagPoint>
    aspect_conn_type aspect_conn_type(const _MagPoint& _mag_point) const
    {
        return aspect_conn_type::NONE;
    }

    template <class _MagPoint>
    aspect_type aspect_conn(const _MagPoint& _mag_point) const
    {
        const hor::orbis aspect_dist[aspect_type_count] =
        {
            0, 0.0, 180.0, 120.0, 90.0, 72.0, 60.0
        };

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

hor_ns_end

#endif // __SYMBOID_ASTRO_CALCULO_PLANET_H__
