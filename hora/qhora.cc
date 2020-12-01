
#include "astro/hora/setup.h"
#include "astro/hora/qhora.h"

QHora::QHora(QObject* parent)
    : QObject(parent)
{
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

bool QHora::calc(const QHoraCoords& horaCoords)
{
    // hora time corrected with time zone data
    eph::basic_time_point<eph_proxy> horaTime =
            eph::basic_calendar<eph_proxy>::time(horaCoords.mCalendarCoords);
    horaTime -= horaCoords.mTimeZoneDiff;

    bool calcResult = true;

    // getting house cusp positions

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
                    const hor::orbis fixstarOrbis = (*fixstarData)->orbis();
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
