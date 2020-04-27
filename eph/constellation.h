
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
    virtual ~constellation() = default;

public:
    const std::string _M_name;
    const std::string _M_alpha_name;
    const arc_degree _M_alpha_rel_pos;
    const arc_degree _M_length;

public:
    virtual ecl_pos alpha_pos() const = 0;
    virtual ecl_pos calc_alpha_pos(const calendar_coords& _time) = 0;
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
    ecl_pos calc_alpha_pos(const calendar_coords& _time) override
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

template <zod _zod_sign, int _degree, int _minute, int _second = 0>
struct pack_arc_degree
{
    static constexpr arc_degree value = arc_degree(int(_zod_sign) * 30 - 30 + _degree) + (arc_degree(_minute * 60.0 + _second) / 3600.0);
};

template <zod _zod_sign, int _degree, int _minute, int _second = 0>
using begin_pos = pack_arc_degree<_zod_sign,_degree,_minute,_second>;

template <zod _zod_sign, int _degree, int _minute, int _second = 0>
using end_pos = pack_arc_degree<_zod_sign,_degree,_minute,_second>;

template <zod _zod_sign, int _degree, int _minute, int _second = 0>
using alpha_pos = pack_arc_degree<_zod_sign,_degree,_minute,_second>;

template<class _BeginLont, class _EndLont, class _AlphaLont>
struct constellation_attributes
{
private:
    template<class _LeftLont, class _RightLont>
    struct lont_diff_fwd
    {
        static constexpr const arc_degree value = _RightLont::value < _LeftLont::value ? _RightLont::value - _LeftLont::value + 360.0 : _RightLont::value - _LeftLont::value;
    };
public:
    static constexpr const arc_degree alpha_rel_pos = lont_diff_fwd<_BeginLont,_AlphaLont>::value;
    static constexpr const arc_degree length = lont_diff_fwd<_BeginLont,_EndLont>::value;
};

struct aries : constellation_attributes
    <
        begin_pos<zod::TAU, 2,29>,
          end_pos<zod::TAU,22,56>,
        alpha_pos<zod::TAU, 6,58>
    >
{
    static constexpr const char* name = "Aries";
    static constexpr const char* alpha_name = "Hamal";
};

struct taurus : constellation_attributes
    <
        begin_pos<zod::TAU,20,58>,
          end_pos<zod::GEM,24, 5>,
        alpha_pos<zod::GEM, 9, 5>
    >
{
    static constexpr const char* name = "Taurus";
    static constexpr const char* alpha_name = "Aldebaran";
};

struct gemini : constellation_attributes
    <
        begin_pos<zod::CAN, 0,15>,
          end_pos<zod::CAN,26,32>,
        alpha_pos<zod::CAN,19,33>
    >
{
    static constexpr const char* name = "Gemini";
    static constexpr const char* alpha_name = "Castor";
};

struct cancer : constellation_attributes
    <
        begin_pos<zod::CAN,26,28>,
          end_pos<zod::LEO,15,57>,
        alpha_pos<zod::LEO,12,57>
    >
{
    static constexpr const char* name = "Cancer";
    static constexpr const char* alpha_name = "Acubens";
};

struct leo : constellation_attributes
    <
        begin_pos<zod::LEO,14,36>,
          end_pos<zod::VIR,20,55>,
        alpha_pos<zod::LEO,29, 8>
    >
{
    static constexpr const char* name = "Leo";
    static constexpr const char* alpha_name = "Regulus";
};

struct virgo : constellation_attributes
    <
        begin_pos<zod::VIR,21, 8>,
          end_pos<zod::ESC, 9,26>,
        alpha_pos<zod::LIB,23, 9>
    >
{
    static constexpr const char* name = "Virgo";
    static constexpr const char* alpha_name = "Spica";
};

struct libra : constellation_attributes
    <
        begin_pos<zod::ESC,13,28>,
          end_pos<zod::ESC,29,47>,
        alpha_pos<zod::ESC,14,23>
    >
{
    static constexpr const char* name = "Libra";
    static constexpr const char* alpha_name = "Zuben Elgenubi";
};

struct escorpio : constellation_attributes
    <
        begin_pos<zod::SAG, 0,36>,
          end_pos<zod::SAG,27,21>,
        alpha_pos<zod::SAG, 9, 4>
    >
{
    static constexpr const char* name = "Scorpius";
    static constexpr const char* alpha_name = "Antares";
};

struct sagittarius : constellation_attributes
    <
        begin_pos<zod::CAP, 0,34>,
          end_pos<zod::CAP,26,22>,
        alpha_pos<zod::CAP,15,56>
    >
{
    static constexpr const char* name = "Sagittarius";
    static constexpr const char* alpha_name = "Rukbat";
};

struct capricornus : constellation_attributes
    <
        begin_pos<zod::AQU, 1, 3>,
          end_pos<zod::AQU,25, 8>,
        alpha_pos<zod::AQU, 3, 4>
    >
{
    static constexpr const char* name = "Capricornus";
    static constexpr const char* alpha_name = "Algedi";
};

struct aquarius : constellation_attributes
    <
        begin_pos<zod::AQU,11, 2>,
          end_pos<zod::PIS,19,30>,
        alpha_pos<zod::PIS, 2,39>
    >
{
    static constexpr const char* name = "Aquarius";
    static constexpr const char* alpha_name = "Sadalmelik";
};

struct pisces : constellation_attributes
    <
        begin_pos<zod::PIS,17,53>,
          end_pos<zod::ARI,28,41>,
        alpha_pos<zod::ARI,28,41>
    >
{
    static constexpr const char* name = "Pisces";
    static constexpr const char* alpha_name = "Alrischa";
};

eph_ns_end

#endif // __SYMBOID_ASTRO_EPH_CONSTELLATION_H__
