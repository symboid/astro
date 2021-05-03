
#include "astro/hora/setup.h"
#include "astro/hora/qdirexmodel.h"

QDirexModel::QDirexModel(QObject* parent)
    : QForecastModel(parent)
{
}

QVector<QSigtor*> QDirexModel::sigtorList() const
{
    QVector<QSigtor*> sigtors;
    if (mHora)
    {
        for (QHora::Planets::ConstIterator planet = mHora->planetsBegin(); planet != mHora->planetsEnd(); ++planet)
        {
            sigtors.push_back(new QPlanetSigtor(*planet));
        }
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

int QDirexModel::estimatedEventCount(const QDateTime& periodBegin, const QDateTime& periodEnd) const
{
    static constexpr double AVG_DIREX_COUNT_PER_MONTH = 2.0;
    double lengthInDays = periodBegin.daysTo(periodEnd);
    double lengthInMonths = lengthInDays < 30.0 ? 1.0 : lengthInDays / 30.0;
    return lengthInMonths * AVG_DIREX_COUNT_PER_MONTH;
}
