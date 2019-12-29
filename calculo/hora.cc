
#include "astro/calculo/setup.h"
#include "astro/calculo/hora.h"

namespace As {

HoraHouses::HouseCusp::HouseCusp()
    : MagPoint(0.0)
    , mHouses(nullptr)
    , mHouseIndex(0)
{
}

HoraHouses::HouseCusp::HouseCusp(const As::Houses* houses, As::Houses::HouseIndex houseIndex)
    : MagPoint(0.10)
    , mHouses(houses)
    , mHouseIndex(houseIndex)
{
}

Sy::String HoraHouses::HouseCusp::name() const
{
    switch (mHouseIndex)
    {
    case  1: return "I";
    case  4: return "IV";
    case  7: return "VII";
    case 10: return "X";
    default: return mHouseIndex <= 12 ? std::to_string(mHouseIndex) : "???";
    }
}

EclPos HoraHouses::HouseCusp::eclPos() const
{
    return mHouses ? mHouses->houseCuspPos(mHouseIndex) : EclPos(0.0);
}

EclSpeed HoraHouses::HouseCusp::eclSpeed() const
{
    return mHouses ? mHouses->houseCuspSpeed(mHouseIndex) : EclSpeed(0.0, 0.0);
}

HoraHouses::HoraHouses()
{
    mHouseCusps[Houses::NULL_INDEX] = HouseCusp(&mHouses, Houses::NULL_INDEX);
    for (Houses::HouseIndex h = mHouses.BEGIN_INDEX; h < mHouses.END_INDEX; ++h)
    {
        mHouseCusps[h] = HouseCusp(&mHouses, h);
    }
}

const HoraHouses::HouseCusp& HoraHouses::houseCusp(Houses::HouseIndex houseIndex) const
{
    return mHouses.BEGIN_INDEX <= houseIndex && houseIndex < mHouses.END_INDEX ? mHouseCusps[houseIndex] : mHouseCusps[mHouses.NULL_INDEX];
}

HoraPlanets::HoraPlanets()
{
}

void HoraPlanets::addPlanet(const Planet& planet)
{
    mPlanets.push_back(planet);
}

void HoraPlanets::addAspectType(const Aspect::Type& aspectType)
{
    mAspectTypes.insert(aspectType);
    if (aspectType != Aspect::NONE && aspectType != Aspect::CONJUNCTION && aspectType != Aspect::OPPOSITION)
    {
        mAspectTypes.insert(Aspect::Type(-aspectType));
    }
}

HoraZodiac::HoraZodiac()
{
    for (int z = 0; z < ZOD_COUNT; ++z)
    {
        mSignBorders[z] = SignBorder(ZodSign(z + 1));
    }
}

HoraZodiac::SignBorder::SignBorder(ZodSign zodSign)
    : MagPoint(0.0)
    , mZodSign(zodSign)
{
}

Sy::String HoraZodiac::SignBorder::name() const
{
    static const Sy::String zodNames[ZOD_COUNT] =
    {
        "Ari", "Tau", "Gem", "Can", "Leo", "Vir",
        "Lib", "Esc", "Sag", "Cap", "Aqu", "Pis"
    };
    int zodIndex = int(mZodSign) - 1;
    return 0 <= zodIndex && zodIndex < ZOD_COUNT ? zodNames[zodIndex] : "???";
}

EclPos HoraZodiac::SignBorder::eclPos() const
{
    int zodIndex = int(mZodSign) - 1;
    return EclPos(zodIndex * 30.0);
}

EclSpeed HoraZodiac::SignBorder::eclSpeed() const
{
    return EclSpeed(0.0, 0.0);
}

const HoraZodiac::SignBorder& HoraZodiac::zodSignBorder(int signBorderIndex) const
{
    static const SignBorder nullBorder(Z_NON);
    return 0 <= signBorderIndex && signBorderIndex < ZOD_COUNT ? mSignBorders[signBorderIndex] : nullBorder;
}

CalcResult Hora::calc(const Time& horaTime, Sy::ArcDegree geoLont, Sy::ArcDegree geoLatt,
        As::Houses::Type housesType)
{
    CalcResult calcResult;
    calcResult = mHouses.calc(housesType, horaTime, geoLont, geoLatt);
    if (calcResult == CALC_SUCCESS)
    {
        for (std::list<Planet>::iterator planet = mPlanets.begin(), end = mPlanets.end();
                calcResult == CALC_SUCCESS && planet != end; ++planet)
        {
            calcResult = planet->calc(horaTime);
        }
    }

    if (calcResult == CALC_SUCCESS)
    {
        mAspectConnections.clear();
        for (std::list<Planet>::iterator planet = mPlanets.begin(), end = mPlanets.end();
                calcResult == CALC_SUCCESS && planet != end; ++planet)
        {
            std::list<Planet>::iterator magPoint = planet;
            while (++magPoint != end)
            {
                AspectConnection aspectConnection(&*planet, &*magPoint);
                if (mAspectTypes.find(aspectConnection.aspectType()) != mAspectTypes.end())
                {
                    mAspectConnections.push_back(aspectConnection);
                }
            }
        }
    }

    return calcResult;
}

} // namespace As
