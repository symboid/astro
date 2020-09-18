
#include "astro/hora/setup.h"
#include "astro/hora/magitem.h"

MagItem::MagItem(eph::ecl_pos basicEclPos, int radixHouse, hor::aspect_type aspectType, bool aspectIsUpper)
    : mAspect(aspectType)
    , mAspectIsUpper(aspectIsUpper)
    , mBasicEclPos(basicEclPos)
    , mRadixHouse(radixHouse)
{
}

QString MagItem::debugStr() const
{
    QString magItemStr;

    // aspect sign
//    magItemStr += Aspect::Auto(aspectType())->debugStr();

    // item name:
    magItemStr += " ";
    QString itemName = name();
    while (itemName.size() < 3) { itemName = itemName + " "; }
    magItemStr += itemName;
    magItemStr += " ";

    // ecliptic position
    magItemStr += " (";
//    magItemStr += eclPos()->debugStr();
    magItemStr += ")";

    return magItemStr;
}

eph::ecl_pos MagItem::eclPos() const
{
    hor::orbis aspectSize = hor::aspect_dist[mAspectType];
    eph::ecl_lont eclLont = mAspectIsUpper ? mBasicEclPos._M_lont - aspectSize :
                                           mBasicEclPos._M_lont + aspectSize;
    eph::ecl_pos eclPos(eclLont, mBasicEclPos._M_latt, mBasicEclPos._M_dist);
    return eclPos;
}

void MagItem::setEclPos(const eph::ecl_pos& eclPos)
{
    eph::ecl_lont eclLont = eclPos._M_lont;
    eph::ecl_lont eclLatt = eclPos._M_latt;
//    eph::ecl_lont eclSpeed = ;

    hor::orbis aspectSize = hor::aspect_dist[mAspectType];
    eclLont += mAspectIsUpper ? aspectSize : -aspectSize;
    mBasicEclPos = eph::ecl_pos(eclLont, eclLatt);
}

bool MagItem::lt(const MagItem* rhs) const
{
    return rhs != nullptr && eclPos()._M_lont < rhs->eclPos()._M_lont;
}

hor::aspect_type MagItem::aspectType() const
{
    return mAspectType;
}

hor::orbis MagItem::orbis() const
{
    return resolveOrbis(mAspectType);
}

hor::orbis MagItem::orbis(hor::aspect_type aspectType) const
{
    return resolveOrbis(aspectType);
}

hor::orbis MagItem::connectingOrbis() const
{
    return resolveOrbis(mAspectType) / 3.0;
}

hor::orbis MagItem::connectingOrbis(hor::aspect_type aspectType) const
{
    return resolveOrbis(aspectType) / 3.0;
}

//hor::orbis MagItem::direxOrbis() const
//{
//    return 0.0;
//}

eph::zod MagItem::radixSign() const
{
    return mBasicEclPos.zod_coords()._M_sign;
}

int MagItem::radixHouse() const
{
    return mRadixHouse;
}

QString MagItem::radixHouseName() const
{
    static const int HOUSE_COUNT = 12;
    static const char* HOUSE_NAMES[HOUSE_COUNT] =
    {
        "I", "2", "3",
        "IV", "5", "6",
        "VII", "8", "9",
        "X", "11", "12"
    };
    int houseIndex = int(mRadixHouse) - 1;
    return (0 <= houseIndex && houseIndex < HOUSE_COUNT) ? HOUSE_NAMES[houseIndex] : "???";
}

class PlanetItem : public MagItem
{
public:
    PlanetItem(const hor::planet& planet, int radixHouse, hor::aspect_type aspectType = hor::aspect_type::conjunction, bool isUpper = false);

public:
    QString name() const override;
    QString symbolStr(QAstroFont* symbolFont, bool isSgtor = true) const override;
    Type type() const override;
    Index index() const override;

public:
    eph::ecl_pos calcEclPos(eph::time_point calcTime) override;

public:
    MagItem* clone() const override;

public:
    bool isRetrograd() const override;
    hor::orbis resolveOrbis(hor::aspect_type aspectType) const override;
//    hor::orbis direxOrbis() const override;

private:
    hor::planet mPlanet;
};

PlanetItem::PlanetItem(const hor::planet& planet, int radixHouse, hor::aspect_type aspectType, bool isUpper)
    : MagItem(planet.pos(), radixHouse, aspectType, isUpper)
    , mPlanet(planet)
{
}

QString PlanetItem::name() const
{
//    return mPlanet.name().le;
}

QString PlanetItem::symbolStr(QAstroFont* symbolFont, bool) const
{
    QString planetStr = symbolFont ? symbolFont->objectLetter(mPlanet.get_index()) : "";
    return planetStr;
}

MagItem::Index PlanetItem::index() const
{
    return int(mPlanet.get_index());
}

MagItem::Type PlanetItem::type() const
{
    return MagItem::PLANET;
}

eph::ecl_pos PlanetItem::calcEclPos(eph::time_point calcTime)
{
    mPlanet.calc_pos(calcTime);
    mBasicEclPos = mPlanet.pos();
    return eclPos();
}

bool PlanetItem::isRetrograd() const
{
    return mPlanet.is_retrograd();
}

hor::orbis PlanetItem::resolveOrbis(hor::aspect_type aspectType) const
{
    return mPlanet.aspect_orbis(aspectType);
}

//hor::orbis PlanetItem::direxOrbis() const
//{
//    return mPlanet->direxOrbis();
//}

MagItem* PlanetItem::clone() const
{
    MagItem* planetCopy = new PlanetItem(mPlanet, mRadixHouse, mAspectType, mAspectIsUpper);
    planetCopy->setEclPos(eclPos());
    return planetCopy;
}

class HouseCuspItem : public MagItem
{
public:
    HouseCuspItem(int houseNum, const hor::hora_coords& calcPos);

public:
    QString name() const override;
    QString symbolStr(QAstroFont* symbolFont, bool isSgtor = true) const override;
    Index index() const override;
    Type type() const override;

public:
    eph::ecl_pos calcEclPos(eph::time_point calcTime) override;

public:
    MagItem* clone() const override;

public:
    bool isRetrograd() const override;

public:
    hor::orbis resolveOrbis(hor::aspect_type aspectType) const override;

private:
    hor::hora_coords mCalcPos;
//    const SyConfig* mOrbisConfig;
};

HouseCuspItem::HouseCuspItem(int houseNum, const hor::hora_coords& calcPos)
    : MagItem(eph::ecl_pos(0.0), houseNum)
    , mCalcPos(calcPos)
//    , mOrbisConfig(sy_config[ORBIS_CONFIG_MAGN_PATH])
{
    eph::time_point calcTime =
            eph::basic_calendar<eph_proxy>::time(calcPos._M_calendar_coords);
    calcTime -= calcPos._M_time_zone_diff;
    calcEclPos(calcTime);
//    if (!mOrbisConfig)
//    {
//        sy_error("Missing orbis config at path '%s'! Using %0.2f for house cusp orbis.",
//                ORBIS_CONFIG_MAGN_PATH.cStr(), ORBIS_DEFAULT);
//    }
}

QString HouseCuspItem::name() const
{
    static const int HOUSE_COUNT = 12;
    static const char* HOUSE_CUSP_NAMES[HOUSE_COUNT] =
    {
        "ASC", "2", "3",
        "IC", "5", "6",
        "DSC", "8", "9",
        "MC", "11", "12"
    };
    int houseIndex = int(mRadixHouse) - 1;
    return (0 <= houseIndex && houseIndex < HOUSE_COUNT) ? HOUSE_CUSP_NAMES[houseIndex] : "???";
}

QString HouseCuspItem::symbolStr(QAstroFont*, bool isSgtor) const
{
    return isSgtor ? name() : radixHouseName();
}

MagItem::Index HouseCuspItem::index() const
{
    return 0;
}

MagItem::Type HouseCuspItem::type() const
{
    return (int(mRadixHouse) % 3 == 1) ? AXIS : HOUSE_CUSP;
}

eph::ecl_pos HouseCuspItem::calcEclPos(eph::time_point calcTime)
{
//    mCalcPos = Se::RadixPos::Params(calcTime, mCalcPos->geoLont(), mCalcPos->geoLatt());
//
//    Se::HouseSys::Auto houseSys(mHouseSysType);
//    houseSys->calcPos(mCalcPos);
//
//    setEclPos(houseSys->houseCusp(mRadixHouse));
    return eclPos();
}

MagItem* HouseCuspItem::clone() const
{
    return new HouseCuspItem(mRadixHouse, mCalcPos);
}

bool HouseCuspItem::isRetrograd() const
{
    return false;
}

hor::orbis HouseCuspItem::resolveOrbis(hor::aspect_type) const
{
//    return mOrbisConfig ? mOrbisConfig->value<hor::orbis>() : ORBIS_DEFAULT;
}

class SignBorderItem : public MagItem
{
public:
    SignBorderItem(eph::zod zodSign, int radixHouse);

public:
    QString name() const override;
    QString symbolStr(QAstroFont* symbolFont, bool isSgtor = true) const override;
    Index index() const override;
    Type type() const override;

public:
    eph::ecl_pos calcEclPos(eph::time_point calcTime) override;

public:
    MagItem* clone() const override;

public:
    bool isRetrograd() const override;

public:
    hor::orbis resolveOrbis(hor::aspect_type aspectType) const override;

private:
    eph::zod mZodSign;
};

SignBorderItem::SignBorderItem(eph::zod zodSign, int radixHouse)
    : MagItem(eph::ecl_pos((int(zodSign) - 1) * 30.0, 0.0, 0.0), radixHouse)
    , mZodSign(zodSign)
{
}

QString SignBorderItem::name() const
{
    switch (mZodSign)
    {
    case eph::zod::ARI: return "Ari";
    case eph::zod::TAU: return "Tau";
    case eph::zod::GEM: return "Gem";
    case eph::zod::CAN: return "Can";
    case eph::zod::LEO: return "Leo";
    case eph::zod::VIR: return "Vir";
    case eph::zod::LIB: return "Lib";
    case eph::zod::ESC: return "Esc";
    case eph::zod::SAG: return "Sag";
    case eph::zod::CAP: return "Cap";
    case eph::zod::AQU: return "Aqu";
    case eph::zod::PIS: return "Pis";
    default:        return "???";
    }
}

QString SignBorderItem::symbolStr(QAstroFont* symbolFont, bool) const
{
    QString zodStr;
    zodStr = symbolFont ? symbolFont->zodLetter(mZodSign) : "";
    return zodStr;
}

MagItem::Index SignBorderItem::index() const
{
    return -1;
}

MagItem::Type SignBorderItem::type() const
{
    return SIGN_BORDER;
}

eph::ecl_pos SignBorderItem::calcEclPos(eph::time_point)
{
    return eclPos();
}

MagItem* SignBorderItem::clone() const
{
    return new SignBorderItem(mZodSign, mRadixHouse);
}

bool SignBorderItem::isRetrograd() const
{
    return false;
}

hor::orbis SignBorderItem::resolveOrbis(hor::aspect_type) const
{
    return 0.0;
}

MagItemFactory::MagItemFactory(const hor::hora* hora)
    : mHora(hora)
{
}

MagItem* MagItemFactory::createPlanetItem(int planetIndex, hor::aspect_type aspectType, bool aspectIsUpper)
{
    int radixHouse = mHora->planetHouse(planetIndex);
    return new PlanetItem(mHora->planet(planetIndex), radixHouse, aspectType, aspectIsUpper);
}

MagItem* MagItemFactory::createHouseCuspItem(int houseNum)
{
    return new HouseCuspItem(mHora->houseSysType(), houseNum, mHora->radixPos());
}

MagItem* MagItemFactory::createSignBorderItem(eph::zod zodSign)
{
    eph::ecl_pos signPos((int(zodSign) - 1) * 30.0, 0.0, 0.0);

    int radixHouse = 0;
    for (int houseNum = 1; !radixHouse && houseNum < 12; houseNum++)
    {
        eph::ecl_pos houseCuspPos = mHora->houses()[houseNum].pos();
        eph::ecl_pos nextCuspPos = mHora->houses()[houseNum + 1].pos();
        if (signPos.dist_to(houseCuspPos) > 0.0 && nextCuspPos.dist_to(signPos) >= 0.0)
        {
            radixHouse = houseNum;
        }
    }
    if (!radixHouse)
    {
        radixHouse = 12;
    }
    return new SignBorderItem(zodSign, radixHouse);
}

