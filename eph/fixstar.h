
#ifndef __SYMBOID_ASTRO_EPH_FIXSTAR_H__
#define __SYMBOID_ASTRO_EPH_FIXSTAR_H__

#include "astro/eph/defs.h"
#include <string>
#include "astro/eph/object.h"
#include <cstring>
#include <list>

eph_ns_begin

template <class _EphProxy>
class fixstar_data
{
public:
    typedef typename _EphProxy::fixstar::magnitude magnitude;

public:
    fixstar_data(const std::string& _nomenclature, magnitude _magnitude)
        : _M_nomenclature(_nomenclature)
        , _M_magnitude(_magnitude)
    {
        _M_name_buffer[0] = ',';
        std::strncpy(_M_name_buffer + 1, _nomenclature.c_str(), _EphProxy::fixstar::NAME_BUFFER_LENGTH - 1);
    }
    fixstar_data(const fixstar_data& _src)
        : _M_nomenclature(_src._M_nomenclature)
        , _M_magnitude(_src._M_magnitude)
    {
        std::strncpy(_M_name_buffer, _src._M_name_buffer, _EphProxy::fixstar::NAME_BUFFER_LENGTH);
    }

public:
    mutable char _M_name_buffer[_EphProxy::fixstar::NAME_BUFFER_LENGTH + 1];
    const std::string _M_nomenclature;
    const magnitude _M_magnitude;

public:
    virtual std::string name() const
    {
        return _M_nomenclature;
    }
    virtual std::string consltn() const = 0;
    virtual bool is_ecliptic() const = 0;
};

template <class _EphProxy>
class basic_fixstar : public basic_object<_EphProxy>
{
public:
    basic_fixstar(const fixstar_data<_EphProxy>* _data)
        : _M_data(_data)
    {
    }
    basic_fixstar()
        : basic_fixstar(nullptr)
    {
    }

private:
    const fixstar_data<_EphProxy>* _M_data;
public:
    const fixstar_data<_EphProxy>* data() const { return _M_data; }

public:
    calc_result calc_pos(const basic_time_point<_EphProxy>& _time_point) override
    {
        struct ecl_pos temp_ecl_pos;
        struct ecl_speed temp_ecl_speed;
        calc_result result = _EphProxy::fixstar::calc_pos(this->_M_data->_M_name_buffer, _time_point, temp_ecl_pos, temp_ecl_speed);
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
