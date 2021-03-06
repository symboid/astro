
#include "astro/db/swephcpp.h"
#include <cstring>
#include <fstream>
#include <sstream>
#include "sdk/arch/log.h"

sy_ns_begin(swe)

#if defined(WIN32) || defined(_WIN32) || defined(WIN64)
#   define DIR_SEP_STR "\\"
#else
#   define DIR_SEP_STR "/"
#endif

static std::string eph_dir_path;

void proxy::set_eph_dir_path(const std::string& _eph_dir_path)
{
    char path_buffer[AS_MAXCH];
    std::strncpy(path_buffer, _eph_dir_path.c_str(), AS_MAXCH);
    swe_set_ephe_path(path_buffer);

    eph_dir_path = _eph_dir_path;
}

std::string proxy::get_eph_dir_path()
{
    return eph_dir_path;
}

proxy::clock::time_point proxy::clock::time(const eph::calendar_coords& _calendar_coords)
{
    const int gregFlag = _calendar_coords._M_calendar_type == eph::calendar_type::GREGORIAN ? SE_GREG_CAL : SE_JUL_CAL;
    rep hours = rep(_calendar_coords._M_hour) + _calendar_coords._M_minute/60.0 + _calendar_coords._M_second/3600.0;
    return time_point(duration(swe_julday(_calendar_coords._M_year, _calendar_coords._M_month, _calendar_coords._M_day, hours, gregFlag)));
}

eph::calendar_coords proxy::clock::coords(const time_point& _time_point)
{
    eph::calendar_coords coords;
    coords._M_calendar_type = eph::calendar_type::GREGORIAN;
    rep hours(0.0);
    swe_revjul(_time_point.time_since_epoch().count(), SE_GREG_CAL, &coords._M_year, &coords._M_month, &coords._M_day, &hours);
    coords._M_hour = int(std::floor(hours));
    rep frac_hours = hours - rep(coords._M_hour);
    coords._M_minute = int(std::floor(60.0 * frac_hours));
    rep frac_minutes = 60.0 * frac_hours - rep(coords._M_minute);
    coords._M_second = int(std::floor(60.0 * frac_minutes));
    return coords;
}

static proxy::calc_type _G_calc_type = proxy::calc_type::DEFAULT;

void proxy::set_calc_type(proxy::calc_type _calc_type)
{
    _G_calc_type = _calc_type;
}

eph::calc_result proxy::object::calc_pos(index _object_index, proxy::clock::time_point _time,
        eph::ecl_pos& _ecl_pos, eph::ecl_speed& _ecl_speed)
{
    char error_str[AS_MAXCH];
    double eph_data[6];

    // setting up ephemeris type
    int32 type_flag;
    switch (_G_calc_type)
    {
    case calc_type::JPL: type_flag = SEFLG_JPLEPH; break;
    case calc_type::MOSHIER: type_flag = SEFLG_MOSEPH; break;
    case calc_type::SWISSEPH: type_flag = SEFLG_SWIEPH; break;
    }

    // SEFLG_SPEED = asking for speed calculation
    int32 calc_flag = type_flag | SEFLG_SPEED;

    int ret_flag = swe_calc_ut(_time.time_since_epoch().count(), int(_object_index), calc_flag, eph_data, error_str);
    if (ret_flag < 0)
    {
        _ecl_pos = eph::ecl_pos(0,0,0);
        _ecl_speed = eph::ecl_speed(0,0);
        log_error << "SWEPH OBJECT: " << error_str;
    }
    else
    {
        if (ret_flag != calc_flag)
        {
            log_warning << "SWEPH OBJECT: " << error_str;
        }
        _ecl_pos = eph::ecl_pos(eph_data[0], eph_data[1], eph_data[2]);
        _ecl_speed = eph::ecl_speed(eph_data[3], eph_data[4]);
    }

    return ret_flag >= 0 ? eph::calc_result::SUCCESS : eph::calc_result::FAILED;
}

eph::calc_result proxy::houses::calc(clock::time_point _time, type _house_system_type,
        eph::arc_degree _geo_lont, eph::arc_degree _geo_latt, eph::ecl_lont* _ecl_lont_buffer)
{
    double house_cusp_values[13], asc_mc_values[10];
    if (swe_houses(_time.time_since_epoch().count(), _geo_latt, _geo_lont, int(_house_system_type),
            house_cusp_values, asc_mc_values) == OK)
    {
        for (int h = 0; h < 13; ++h)
        {
            _ecl_lont_buffer[h] = house_cusp_values[h];
        }
        return eph::calc_result::SUCCESS;
    }
    else
    {
        return eph::calc_result::FAILED;
    }
}

eph::calc_result proxy::fixstar::calc_pos(char* _name_buffer, clock::time_point _time,
        eph::ecl_pos& _ecl_pos, eph::ecl_speed& _ecl_speed)
{
    char error_str[AS_MAXCH];
    double eph_data[6];

    // setting up ephemeris type
    int32 type_flag;
    switch (_G_calc_type)
    {
    case calc_type::JPL: type_flag = SEFLG_JPLEPH; break;
    case calc_type::MOSHIER: type_flag = SEFLG_MOSEPH; break;
    case calc_type::SWISSEPH: type_flag = SEFLG_SWIEPH; break;
    }

    // SEFLG_SPEED = asking for speed calculation
    int32 calc_flag = type_flag | SEFLG_SPEED;

    int ret_flag = swe_fixstar2_ut(_name_buffer, _time.time_since_epoch().count(), calc_flag,
            eph_data, error_str);
    if (ret_flag < 0)
    {
        _ecl_pos = eph::ecl_pos(0,0,0);
        _ecl_speed = eph::ecl_speed(0,0);
        log_error << "SWEPH FIXSTAR: " << error_str;
    }
    else
    {
        if (ret_flag != calc_flag)
        {
            log_warning << "SWEPH FIXSTAR: " << error_str;
        }
        _ecl_pos = eph::ecl_pos(eph_data[0], eph_data[1], eph_data[2]);
        _ecl_speed = eph::ecl_speed(eph_data[3], eph_data[4]);
    }

    return ret_flag >= 0 ? eph::calc_result::SUCCESS : eph::calc_result::FAILED;
}

proxy::fixstar::magnitude proxy::fixstar::calc_magnitude(char* _name_buffer)
{
    char error_str[AS_MAXCH];
    magnitude magnitude;
    if (swe_fixstar2_mag(_name_buffer, &magnitude, error_str) != OK)
    {
        magnitude = 0.0;
    }
    return magnitude;
}

sy_ns_end
