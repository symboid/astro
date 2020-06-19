
#ifndef __SYMBOID_ASTRO_EPH_FIXSTAR_H__
#define __SYMBOID_ASTRO_EPH_FIXSTAR_H__

#include "astro/eph/defs.h"
#include <string>
#include "astro/eph/ecliptic.h"
#include "astro/eph/calendar.h"
#include <cstring>

eph_ns_begin

template <class _EphProxy>
class basic_fixstar
{
public:
    typedef typename _EphProxy::fixstar::magnitude magnitude;

public:
    basic_fixstar(const std::string& _name, const std::string& _nomenclature, magnitude _magnitude)
        : _M_nomenclature(_nomenclature)
        , _M_magnitude(_magnitude)
    {
        std::strncpy(_M_name, _name.c_str(), _EphProxy::fixstar::NAME_BUFFER_LENGTH);
    }
    basic_fixstar(const std::string& _name, const std::string& _nomenclature)
        : basic_fixstar(_name, _nomenclature, 0.0)
        , _M_magnitude(_EphProxy::fixstar::calc_magnitude(_M_name))
    {
    }

private:
    char _M_name[_EphProxy::fixstar::NAME_BUFFER_LENGTH + 1];
    std::string _M_nomenclature;
    magnitude _M_magnitude;
    ecl_pos _M_ecl_pos;
    ecl_speed _M_ecl_speed;

public:
    const ecl_pos& ecl_pos() const { return _M_ecl_pos; }
    const ecl_speed& ecl_speed() const { return _M_ecl_speed; }
    const char* name() const { return _M_name; }
    std::string nomenclature() const { return _M_nomenclature; }
    magnitude magn() const { return _M_magnitude; }

public:
    calc_result calc(const basic_time_point<_EphProxy> _time_point)
    {
        struct ecl_pos temp_ecl_pos;
        struct ecl_speed temp_ecl_speed;
        calc_result result = _EphProxy::fixstar::calc_pos(_M_name, _time_point, temp_ecl_pos, temp_ecl_speed);
        if (result == calc_result::SUCCESS)
        {
            _M_ecl_pos = temp_ecl_pos;
            _M_ecl_speed = temp_ecl_speed;
        }
        return result;
    }
};

eph_ns_end

#endif // __SYMBOID_ASTRO_EPH_FIXSTAR_H__
