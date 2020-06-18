
#ifndef __SYMBOID_ASTRO_EPH_CONSTELLATION_H__
#define __SYMBOID_ASTRO_EPH_CONSTELLATION_H__

#include "astro/eph/defs.h"
#include <string>
#include "astro/eph/ecliptic.h"
#include "astro/eph/calendarcoords.h"
#include <cstring>

eph_ns_begin

class constellation
{
public:
    enum type
    {
        RED,
        BLUE,
        GREEN,
        PURPLE,
        BROWN,
        BLACK,
    };

public:
    constellation(const std::string& _name, const std::string& _alpha_name,
            arc_degree _alpha_rel_pos, arc_degree _length, type _type, int _sphere)
        : _M_name(_name)
        , _M_alpha_rel_pos(_alpha_rel_pos)
        , _M_length(_length)
        , _M_type(_type)
        , _M_sphere(_sphere)
    {
        std::strncpy(_M_alpha_name, _alpha_name.c_str(), 256);
    }
    virtual ~constellation() = default;

public:
    const std::string _M_name;
    char _M_alpha_name[257];
    const arc_degree _M_alpha_rel_pos;
    const arc_degree _M_length;
    const type _M_type;
    const int _M_sphere;

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
                        _ConstellationAttributes::alpha_rel_pos, _ConstellationAttributes::length,
                        _ConstellationAttributes::type, _ConstellationAttributes::sphere)
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
    static constexpr int sphere = 1;
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
    static constexpr const constellation::type type = constellation::RED;
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
    static constexpr const constellation::type type = constellation::BLUE;
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
    static constexpr const constellation::type type = constellation::RED;
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
    static constexpr const constellation::type type = constellation::BLUE;
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
    static constexpr const constellation::type type = constellation::RED;
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
    static constexpr const constellation::type type = constellation::BLUE;
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
    static constexpr const constellation::type type = constellation::RED;
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
    static constexpr const constellation::type type = constellation::BLUE;
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
    static constexpr const constellation::type type = constellation::RED;
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
    static constexpr const constellation::type type = constellation::BLUE;
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
    static constexpr const constellation::type type = constellation::RED;
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
    static constexpr const constellation::type type = constellation::BLUE;
};

struct orion : constellation_attributes
    <
        begin_pos<zod::GEM,11,13>,
          end_pos<zod::CAN, 2,14>,
        alpha_pos<zod::GEM,28, 3>
    >
{
    static constexpr const char* name = "Orion";
    static constexpr const char* alpha_name = "Betelgeuse";
    static constexpr const constellation::type type = constellation::GREEN;
    static constexpr int sphere = 2;
};

struct serpens_caput : constellation_attributes
    <
        begin_pos<zod::ESC,13,35>,
          end_pos<zod::SAG, 1,36>,
        alpha_pos<zod::ESC,21,22>
    >
{
    static constexpr const char* name = "Serpens Caput";
    static constexpr const char* alpha_name = "Unukalhai";
    static constexpr const constellation::type type = constellation::GREEN;
    static constexpr int sphere = 2;
};

struct ophiuchus : constellation_attributes
    <
        begin_pos<zod::SAG, 1,36>,
          end_pos<zod::CAP, 0,49>,
        alpha_pos<zod::SAG,21,45>
    >
{
    static constexpr const char* name = "Ophiuchus";
    static constexpr const char* alpha_name = "Rasalhague";
    static constexpr const constellation::type type = constellation::GREEN;
    static constexpr int sphere = 2;
};

struct serpens_cauda : constellation_attributes
    <
        begin_pos<zod::CAP, 0, 6>,
          end_pos<zod::CAP,15, 4>,
        alpha_pos<zod::CAP,15, 4>
    >
{
    static constexpr const char* name = "Serpens Cauda";
    static constexpr const char* alpha_name = "Alya";
    static constexpr const constellation::type type = constellation::GREEN;
    static constexpr int sphere = 2;
};

struct cetus : constellation_attributes
    <
        begin_pos<zod::ARI, 0,13>,
          end_pos<zod::TAU,14,24>,
        alpha_pos<zod::TAU,13,37>
    >
{
    static constexpr const char* name = "Cetus";
    static constexpr const char* alpha_name = "Menkar";
    static constexpr const constellation::type type = constellation::BROWN;
    static constexpr int sphere = 2;
};

struct eridanus : constellation_attributes
    <
        begin_pos<zod::PIS,14,36>,
          end_pos<zod::GEM,14,35>,
        alpha_pos<zod::PIS,14,36>
    >
{
    static constexpr const char* name = "Eridanus";
    static constexpr const char* alpha_name = "Achernar";
    static constexpr const constellation::type type = constellation::PURPLE;
    static constexpr int sphere = 3;
};

struct monoceros : constellation_attributes
    <
        begin_pos<zod::CAN, 3,33>,
          end_pos<zod::LEO, 4,27>,
        alpha_pos<zod::CAN,28,35>
    >
{
    static constexpr const char* name = "Monoceros";
    static constexpr const char* alpha_name = ",alMon";
    static constexpr const constellation::type type = constellation::PURPLE;
    static constexpr int sphere = 2;
};

struct hydra : constellation_attributes
    <
        begin_pos<zod::LEO, 9,37>,
          end_pos<zod::ESC,17,47>,
        alpha_pos<zod::LEO,26,35>
    >
{
    static constexpr const char* name = "Hydra";
    static constexpr const char* alpha_name = "Alphard";
    static constexpr const constellation::type type = constellation::BROWN;
    static constexpr int sphere = 3;
};

struct sextans : constellation_attributes
    <
        begin_pos<zod::VIR, 1,14>,
          end_pos<zod::VIR, 9,25>,
        alpha_pos<zod::VIR, 3,25>
    >
{
    static constexpr const char* name = "Sextans";
    static constexpr const char* alpha_name = ",alSex";
    static constexpr const constellation::type type = constellation::BROWN;
    static constexpr int sphere = 2;
};

struct crater : constellation_attributes
    <
        begin_pos<zod::VIR,23, 0>,
          end_pos<zod::LIB, 5,24>,
        alpha_pos<zod::VIR,23, 0>
    >
{
    static constexpr const char* name = "Crater";
    static constexpr const char* alpha_name = "Alkes";
    static constexpr const constellation::type type = constellation::PURPLE;
    static constexpr int sphere = 2;
};

struct corvus : constellation_attributes
    <
        begin_pos<zod::LIB,10, 2>,
          end_pos<zod::LIB,16,40>,
        alpha_pos<zod::LIB,11,33>
    >
{
    static constexpr const char* name = "Corvus";
    static constexpr const char* alpha_name = "Alchiba";
    static constexpr const constellation::type type = constellation::BLACK;
    static constexpr int sphere = 2;
};

struct aquila : constellation_attributes
    <
        begin_pos<zod::CAP,15,21>,
          end_pos<zod::AQU, 4,20>,
        alpha_pos<zod::AQU, 1, 4>
    >
{
    static constexpr const char* name = "Aquila";
    static constexpr const char* alpha_name = "Altair";
    static constexpr const constellation::type type = constellation::PURPLE;
    static constexpr int sphere = 2;
};

struct delphinus : constellation_attributes
    <
        begin_pos<zod::AQU,13,22>,
          end_pos<zod::AQU,18,41>,
        alpha_pos<zod::AQU,16,41>
    >
{
    static constexpr const char* name = "Delphinus";
    static constexpr const char* alpha_name = "Sualocin";
    static constexpr const constellation::type type = constellation::PURPLE;
    static constexpr int sphere = 2;
};

struct equuleus : constellation_attributes
    <
        begin_pos<zod::AQU,17,53>,
          end_pos<zod::AQU,24,44>,
        alpha_pos<zod::AQU,22,25>
    >
{
    static constexpr const char* name = "Equuleus";
    static constexpr const char* alpha_name = "Kitalpha";
    static constexpr const constellation::type type = constellation::BROWN;
    static constexpr int sphere = 3;
};

eph_ns_end

#endif // __SYMBOID_ASTRO_EPH_CONSTELLATION_H__
