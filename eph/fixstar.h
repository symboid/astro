
#ifndef __SYMBOID_ASTRO_EPH_FIXSTAR_H__
#define __SYMBOID_ASTRO_EPH_FIXSTAR_H__

#include "astro/eph/defs.h"
#include <string>
#include "astro/eph/object.h"
#include <cstring>

eph_ns_begin

template <class _EphProxy>
class fixstar_data
{
public:
    typedef typename _EphProxy::fixstar::magnitude magnitude;

public:
    fixstar_data(const std::string& _name, const std::string& _nomenclature, magnitude _magnitude)
        : _M_nomenclature(_nomenclature)
        , _M_magnitude(_magnitude)
    {
        std::strncpy(_M_name, _name.c_str(), _EphProxy::fixstar::NAME_BUFFER_LENGTH);
    }
    fixstar_data(const std::string& _name, const std::string& _nomenclature)
        : fixstar_data(_name, _nomenclature, 0.0)
        , _M_magnitude(_EphProxy::fixstar::calc_magnitude(_M_name))
    {
    }

protected:
    char _M_name[_EphProxy::fixstar::NAME_BUFFER_LENGTH + 1];
    std::string _M_nomenclature;
    magnitude _M_magnitude;

public:
    const char* name() const { return _M_name; }
    std::string nomenclature() const { return _M_nomenclature; }
    magnitude magn() const { return _M_magnitude; }
};

template <class _EphProxy>
class basic_fixstar : public basic_object<_EphProxy>, public fixstar_data<_EphProxy>
{
public:
    basic_fixstar(const std::string& _name, const std::string& _nomenclature,
                  typename fixstar_data<_EphProxy>::magnitude _magnitude)
        : fixstar_data<_EphProxy>(_name, _nomenclature, _magnitude)
    {
    }
    basic_fixstar(const std::string& _name, const std::string& _nomenclature)
        : fixstar_data<_EphProxy>(_name, _nomenclature)
    {
    }
    basic_fixstar(const fixstar_data<_EphProxy>& _data)
        : fixstar_data<_EphProxy>(_data)
    {
    }
public:
    calc_result calc_pos(const basic_time_point<_EphProxy>& _time_point) override
    {
        struct ecl_pos temp_ecl_pos;
        struct ecl_speed temp_ecl_speed;
        calc_result result = _EphProxy::fixstar::calc_pos(this->_M_name, _time_point, temp_ecl_pos, temp_ecl_speed);
        if (result == calc_result::SUCCESS)
        {
            this->_M_ecl_pos = temp_ecl_pos;
            this->_M_ecl_speed = temp_ecl_speed;
        }
        return result;
    }
};

eph_ns_end

#endif // __SYMBOID_ASTRO_EPH_FIXSTAR_H__
