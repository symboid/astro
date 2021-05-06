
#include "astro/hora/setup.h"
#include "astro/hora/qdirexmodel.h"

QDirexModel::QDirexModel(QObject* parent)
    : QForecastModel(parent)
{
}

QVector<QSigtor*> QDirexModel::sigtorList()
{
    QVector<QSigtor*> sigtors;
    if (mHora)
    {
        for (QHora::Planets::ConstIterator planet = mHora->planetsBegin(); planet != mHora->planetsEnd(); ++planet)
        {
            sigtors.push_back(new QPlanetSigtor(*planet));
        }
        sigtors.push_back(new QHouseCuspSigtor(mHora->house(1)));
        sigtors.push_back(new QHouseCuspSigtor(mHora->house(4)));
        sigtors.push_back(new QHouseCuspSigtor(mHora->house(7)));
        sigtors.push_back(new QHouseCuspSigtor(mHora->house(10)));
    }
    return sigtors;
}

void QDirexModel::initSigtorPos(QSigtor* sigtor, const QHoraCoords& eventCoords)
{
    if (mHora && sigtor)
    {
        const QHoraCoords& horaCoords = mHora->coords();
        sigtor->calcEclPos(horaCoords);
        QEclLont yearSum = double((eventCoords.ephTime() - horaCoords.ephTime()).count()) / 365.25;
        sigtor->setEclPos(QEclPos(sigtor->eclPos()._M_lont + yearSum, sigtor->eclPos()._M_latt));
    }
}

QHoraCoords* QDirexModel::calcConj(QSigtor* sigtor, const QHoraCoords* startTime,
        const QAspectObjectList::Siblings& siblings)
{
    QEphTime conjTime = startTime->ephTime();
    if (sigtor && siblings.mSucc)
    {
        qreal yearDist = sigtor->eclPos().dist_to(siblings.mSucc->eclPos());
        conjTime += eph::basic_calendar<eph_proxy>::days(yearDist * 365.35);
        sigtor->setEclPos(siblings.mSucc->eclPos());
    }
    QHoraCoords* conjCoords = new QHoraCoords(conjTime);
    conjCoords->setGeoLatt(startTime->geoLatt());
    conjCoords->setGeoLont(startTime->geoLont());
    conjCoords->setTzDiff(startTime->tzDiff());
    return conjCoords;
}

int QDirexModel::estimatedEventCount(const QHoraCoords* periodBegin, const QHoraCoords* periodEnd) const
{
    static constexpr double AVG_DIREX_COUNT_PER_MONTH = 2.0;
    double lengthInDays = (periodEnd->ephTime()-periodBegin->ephTime()).count();
    double lengthInMonths = lengthInDays < 30.0 ? 1.0 : lengthInDays / 30.0;
    return int(lengthInMonths * AVG_DIREX_COUNT_PER_MONTH);
}
