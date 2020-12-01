
#ifndef __SYMBOID_ASTRO_CALCULO_PLANET_H__
#define __SYMBOID_ASTRO_CALCULO_PLANET_H__

#include "astro/calculo/defs.h"
#include "astro/eph/object.h"
#include <memory>
#include "astro/eph/proxy.h"

hor_ns_begin

typedef eph::arc_degree orbis;

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
    basic_planet(index _planet_index)
        : _M_index(_planet_index)
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
};

typedef basic_planet<eph_proxy> planet;

hor_ns_end

#endif // __SYMBOID_ASTRO_CALCULO_PLANET_H__
