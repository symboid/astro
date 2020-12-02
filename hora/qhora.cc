
#include "astro/hora/setup.h"
#include "astro/hora/qhora.h"

QHora::QHora(QObject* parent)
    : QObject(parent)
{
    for (int h = 0; h < QHouseSystem::HOUSE_COUNT; ++h)
    {
        mHouseCusps[h] = new QHouseCusp(this, &mHouseSystem, h + 1);
    }
    mHouseCusps[QHouseSystem::HOUSE_COUNT] = nullptr;

    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::sunNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::monNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::merNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::venNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::marNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::jupNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::satNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::uraNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::nepNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::pluNode));

    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::nodNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::lilNode));
}

QHouseCusp*const* QHora::housesBegin() const
{
    return & mHouseCusps[0];
}

QHouseCusp*const* QHora::housesEnd() const
{
    return & mHouseCusps[QHouseSystem::HOUSE_COUNT];
}

const QHouseCusp* QHora::house(int index) const
{
    return mHouseCusps[index];
}

QHora::Planets::ConstIterator QHora::planetsBegin() const
{
    return mPlanets.begin();
}

QHora::Planets::ConstIterator QHora::planetsEnd() const
{
    return mPlanets.end();
}

int QHora::planetCount() const
{
    return mPlanets.size();
}

const QPlanet* QHora::planet(int index) const
{
    return mPlanets.at(index);
}

QHora::ConjunctingFixstars::ConstIterator QHora::fixstarsBegin() const
{
    return mConjunctingFixstars.begin();
}

QHora::ConjunctingFixstars::ConstIterator QHora::fixstarsEnd() const
{
    return mConjunctingFixstars.end();
}

bool QHora::calc(const QHoraCoords& horaCoords, QHouseSystem::Type houseSystemType)
{
    // hora time corrected with time zone data
    QEphTime horaTime = eph::basic_calendar<eph_proxy>::time(horaCoords.mCalendarCoords);
    horaTime -= horaCoords.mTimeZoneDiff;

    bool calcResult = true;

    // getting house cusp positions
    mHouseSystem.mType = houseSystemType;
    calcResult = mHouseSystem.calc(horaTime, horaCoords.mGeoLont, horaCoords.mGeoLatt);
    for (int h = 0; h < QHouseSystem::HOUSE_COUNT; ++h)
    {
        mHouseCusps[h]->calc(horaTime);
    }

    // getting planet positions
    for (Planets::iterator planet = mPlanets.begin(), end = mPlanets.end(); calcResult && planet < end; ++planet)
    {
        calcResult = (*planet)->calc(horaTime);
    }

    // getting fixstar data (if enabled)
    mConjunctingFixstars.clear();
    if (mHoraConfig->fixstars()->enabled())
    {
        typedef typename Fixstars::Container::iterator Iterator;
        for (Iterator fixstarData = mAllFixstars->begin(), fEnd = mAllFixstars->end();
             calcResult && fixstarData != fEnd; ++fixstarData)
        {
            if (mAllFixstars->filter_match(*fixstarData))
            {
                eph::basic_fixstar<eph_proxy> fixstar(fixstarData->get());
                calcResult = (fixstar.calc_pos(horaTime) == eph::calc_result::SUCCESS);
                if (calcResult)
                {
                    bool conjuncting = false;
                    const eph::ecl_pos fixstarPos = fixstar.pos();
                    const QOrbisValue fixstarOrbis = (*fixstarData)->orbis();
                    for (Planets::ConstIterator planet = mPlanets.begin(), pEnd = mPlanets.end();
                            !conjuncting && planet != pEnd; ++planet)
                    {
                        conjuncting = ((*planet)->eclPos().dist_abs(fixstarPos) < fixstarOrbis);
                    }
    //                    for (std::vector<eph::house_cusp>::const_iterator house = _M_houses.begin(), h_end = _M_houses.end();
    //                            !conjuncting && house != h_end; ++house)
    //                    {
    //                        conjuncting = (house->pos().dist_abs(fixstar_pos) < fixstar_orbis);
    //                    }
                    if (conjuncting)
                    {
                        mConjunctingFixstars.push_back(fixstar);
                    }
                }
            }
        }
    }
    return calcResult;
}
