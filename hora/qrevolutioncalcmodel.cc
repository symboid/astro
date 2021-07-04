
#include "astro/hora/setup.h"
#include "astro/hora/qrevolutioncalcmodel.h"
#include "sdk/controls/qcalctask.h"

QRevolutionCalcModel::QRevolutionCalcModel(QOrbisConfigNodeGetter planetConfig, const QEclLont& targetLont)
    : mPlanet(this, planetConfig)
    , mMaxLontSpeed(mPlanet.maxLontSpeed())
    , mTargetLont(targetLont)
    , mCalcTask(nullptr)
{
}

void QRevolutionCalcModel::setCalcTask(QCalcTask *calcTask)
{
    mCalcTask = calcTask;
}

QRevolutionData QRevolutionCalcModel::approxLinear(const QHoraCoords* startCoords)
{
    QEphTime targetTime(startCoords->ephTime());

    if (mCalcTask)
    {
        // calculation of ecliptic pos and speed:
        bool calcOk = mPlanet.calc(targetTime);
        QEclSpeed speed = mPlanet.eclSpeed();
        double distTo = mPlanet.eclPos().dist_fwd(mTargetLont);

        // iteration until distance is as exact as possible:
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

    // TODO: timezone diff value depends on target time itself (not on start time)
    return QRevolutionData(new QHoraCoords(targetTime, startCoords->tzDiff()),  mPlanet.isRetrograd());
}

QRevolutionData QRevolutionCalcModel::approxNext(const QHoraCoords *startCoords)
{
    QEphTime targetTime(startCoords->ephTime());

    if (mCalcTask)
    {
        const qreal TARGET_SPACE = 30.0;
        const qreal TARGET_FOCUS = 1.0;

        bool calcOk = mPlanet.calc(targetTime);
        qreal dist = mPlanet.eclPos().dist_to(mTargetLont);
        // iteration until target pos has been crossed (step through)
        bool isCrossing = false;
        while (!mCalcTask->isAborted() && !mCalcTask->restarted() && calcOk && !isCrossing)
        {
            qreal daysToAdd =
                    qAbs(dist) > TARGET_SPACE ? TARGET_SPACE / mMaxLontSpeed :
                    qAbs(dist) > TARGET_FOCUS ? TARGET_FOCUS / mMaxLontSpeed :
                                                1.0;
            targetTime += daysToAdd * eph::basic_calendar<eph_proxy>::ONE_DAY;

            qreal prevDist = dist;
            calcOk = mPlanet.calc(targetTime);
            dist = mPlanet.eclPos().dist_to(mTargetLont);
            isCrossing = qAbs(dist) < TARGET_SPACE && ((prevDist > 0.0 && dist < 0.0) || (prevDist < 0.0 && dist > 0.0));
        }

        targetTime = approxFineTune(targetTime);
    }

    // TODO: timezone diff value depends on target time itself (not on start time)
    return QRevolutionData(new QHoraCoords(targetTime, startCoords->tzDiff()), mPlanet.isRetrograd());
}

QEphTime QRevolutionCalcModel::approxFineTune(QEphTime conjTime)
{
    bool calcOk = mPlanet.calc(conjTime);
    qreal dist = mPlanet.eclPos().dist_to(mTargetLont);
    QEclSpeed speed = mPlanet.eclSpeed();
    while (!mCalcTask->isAborted() && !mCalcTask->restarted() && calcOk && qAbs(dist) >= APPROX_DIST)
    {
        // linear approximation of ecliptic pos:
        double estmDays = dist / speed._M_lont;
        conjTime += estmDays * eph::basic_calendar<eph_proxy>::ONE_DAY;

        // calculation of ecliptic pos and speed:
        calcOk = mPlanet.calc(conjTime);
        speed = mPlanet.eclSpeed();
        dist = mPlanet.eclPos().dist_to(mTargetLont);
    }
    return conjTime;
}
