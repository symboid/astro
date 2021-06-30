
#include "astro/hora/setup.h"
#include "astro/hora/qrevolutioncalcmodel.h"
#include "sdk/controls/qcalctask.h"

QRevolutionCalcModel::QRevolutionCalcModel(QOrbisConfigNodeGetter planetConfig, const QEclLont& targetLont)
    : mPlanet(this, planetConfig)
    , mTargetLont(targetLont)
    , mCalcTask(nullptr)
{
}

void QRevolutionCalcModel::setCalcTask(QCalcTask *calcTask)
{
    mCalcTask = calcTask;
}

QSharedPointer<QHoraCoords> QRevolutionCalcModel::approx(const QHoraCoords* startCoords)
{
    switch (mPlanet.mIndex)
    {
    case QPlanet::SUN: case QPlanet::MOON: return approxLinear(startCoords);
    case QPlanet::JUPITER: case QPlanet::SATURN: case QPlanet::URANUS:
    case QPlanet::NEPTUNE: case QPlanet::PLUTO: return approxSlowPlanet(startCoords);
    default: return approxLinear(startCoords);
    }
}

QSharedPointer<QHoraCoords> QRevolutionCalcModel::approxLinear(const QHoraCoords* startCoords)
{
    QEphTime targetTime(startCoords->ephTime());

    if (mCalcTask)
    {
        // calculation of ecliptic pos and speed:
        bool calcOk = mPlanet.calc(targetTime);
        QEclSpeed speed = mPlanet.eclSpeed();
        double distTo = mPlanet.eclPos().dist_fwd(mTargetLont);

        // iteration until distance is close enough:
        while (!mCalcTask->isAborted() && !mCalcTask->restarted() && calcOk && qAbs(distTo) >= APPROX_DIST)
        {
            // linear approximation of ecliptic pos:
            double estmDays = distTo / speed._M_lont;
            targetTime += estmDays * eph::basic_calendar<eph_proxy>::ONE_DAY;

            // calculation of ecliptic pos and speed:
            calcOk = mPlanet.calc(targetTime);
            speed = mPlanet.eclSpeed();
            distTo = mPlanet.eclPos().dist_to(mTargetLont);
        }
    }

    return QSharedPointer<QHoraCoords>(new QHoraCoords(targetTime, startCoords->tzDiff()));
}

QSharedPointer<QHoraCoords> QRevolutionCalcModel::approxSlowPlanet(const QHoraCoords *startCoords)
{
    QEphTime targetTime(startCoords->ephTime());

    if (mCalcTask)
    {
        // calculation of ecliptic pos and speed:
        bool calcOk = mPlanet.calc(targetTime);
        QEclSpeed speed = mPlanet.eclSpeed();
        double distTo = mPlanet.eclPos().dist_to(mTargetLont);

        double prevDist = distTo;
        while (!mCalcTask->isAborted() && !mCalcTask->restarted() && calcOk && prevDist * distTo > 0.0)
        {
            targetTime += eph::basic_calendar<eph_proxy>::ONE_DAY;

            // calculation of ecliptic pos and speed:
            calcOk = mPlanet.calc(targetTime);
            prevDist = distTo;
            speed = mPlanet.eclSpeed();
            distTo = mPlanet.eclPos().dist_to(mTargetLont);
        }

        // iteration until distance is close enough:
        while (!mCalcTask->isAborted() && !mCalcTask->restarted() && calcOk && qAbs(distTo) >= APPROX_DIST)
        {
            // linear approximation of ecliptic pos:
            double estmDays = distTo / speed._M_lont;
            if (estmDays >  1.0) estmDays =  1.0;
            if (estmDays < -1.0) estmDays = -1.0;
            targetTime += estmDays * eph::basic_calendar<eph_proxy>::ONE_DAY;

            // calculation of ecliptic pos and speed:
            calcOk = mPlanet.calc(targetTime);
            speed = mPlanet.eclSpeed();
            distTo = mPlanet.eclPos().dist_to(mTargetLont);
        }
    }

    return QSharedPointer<QHoraCoords>(new QHoraCoords(targetTime, startCoords->tzDiff()));
}
