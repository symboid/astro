
#include "astro/hora/setup.h"
#include "astro/hora/qrevolutioncalcmodel.h"

QRevolutionCalcModel::QRevolutionCalcModel(QOrbisConfigNodeGetter planetConfig, const QEclLont& targetLont)
    : mPlanet(this, planetConfig)
    , mTargetLont(targetLont)
{
}

QSharedPointer<QHoraCoords> QRevolutionCalcModel::approx(const QHoraCoords* startCoords)
{
    switch (mPlanet.mIndex)
    {
    case QPlanet::SUN: case QPlanet::MOON: return approxLinear(startCoords);
    default: return approxLinear(startCoords);
    }
}

QSharedPointer<QHoraCoords> QRevolutionCalcModel::approxLinear(const QHoraCoords* startCoords)
{
    QEphTime targetTime(startCoords->ephTime());

    // calculation of ecliptic pos and speed:
    bool calcOk = mPlanet.calc(targetTime);
    QEclSpeed speed = mPlanet.eclSpeed();
    double distTo = mPlanet.eclPos().dist_fwd(mTargetLont);

    // iteration until distance is close enough:
    while (calcOk && std::abs(distTo) >= APPROX_DIST)
    {
        // linear approximation of ecliptic pos:
        double nDays = distTo / speed._M_lont;
        targetTime += nDays * eph::basic_calendar<eph_proxy>::ONE_DAY;

        // calculation of ecliptic pos and speed:
        calcOk = mPlanet.calc(targetTime);
        speed = mPlanet.eclSpeed();
        distTo = mPlanet.eclPos().dist_to(mTargetLont);
    }

    return QSharedPointer<QHoraCoords>(new QHoraCoords(targetTime));
}
