
#include "astro/hora/setup.h"
#include "astro/hora/qtransitmodel.h"

QTransitModel::QTransitModel(QObject* parent)
    : QForecastModel(parent)
    , mTzDiff(0.0)
{
}

QVector<QSigtor*> QTransitModel::sigtorList()
{
    QVector<QSigtor*> sigtors;
    if (mHora)
    {
        for (QHora::Planets::ConstIterator planet = mHora->planetsBegin(); planet != mHora->planetsEnd(); ++planet)
        {
            if (planetIsSigtor(*planet))
            {
                sigtors.push_back(new QPlanetSigtor(*planet));
            }
        }
    }
    return sigtors;
}

void QTransitModel::initSigtorPos(QSigtor* sigtor, const QHoraCoords& eventCoords)
{
    if (sigtor)
    {
        sigtor->calcEclPos(eventCoords);
    }
}

QDateTime QTransitModel::calcConj(QSigtor* sigtor, const QDateTime& startTime,
        const QAspectObjectList::Siblings& siblings)
{
    return calcTransitTime(sigtor, startTime, siblings.mPrec->eclPos(), siblings.mSucc->eclPos());
}

int QTransitModel::estimatedEventCount(const QDateTime& periodBegin, const QDateTime& periodEnd) const
{
    static constexpr double AVG_DIREX_COUNT_PER_MONTH = 2.0;
    double lengthInDays = periodBegin.daysTo(periodEnd);
    double lengthInMonths = lengthInDays < 30.0 ? 1.0 : lengthInDays / 30.0;
    return lengthInMonths * AVG_DIREX_COUNT_PER_MONTH;
}

bool QTransitModel::planetIsSigtor(const QPlanet* planet) const
{
    return planet->mIndex > QPlanet::MARS && planet->mIndex <= QPlanet::PLUTO;
}

void QTransitModel::setTzDiff(double tzDiff)
{
    mTzDiff = tzDiff;
}

QDateTime QTransitModel::calcTransitTime(QSigtor* sigtor, const QDateTime& startTime,
        const QEclPos& precPos, const QEclPos& succPos)
{
    QDateTime conjTime = startTime;

    if (sigtor)
    {
        double DIST_TOLERANCE = 1.0 / 3600.0;

        sigtor->calcEclPos(QHoraCoords(startTime, mTzDiff));
        QEclPos conjPos = sigtor->eclPos();
        while (conjPos.dist_abs(precPos) > DIST_TOLERANCE && conjPos.dist_abs(succPos) > DIST_TOLERANCE)
        {
//            qDebug() << "curr pos:" << conjPos._M_lont;
//            qDebug() << "succ pos:" << succPos._M_lont;
//            qDebug() << "prec pos:" << precPos._M_lont;

            // speed of planet at current position (measured in degrees/day):
            QEclSpeed::lont speed = sigtor->eclSpeed()._M_lont;
//            qDebug() << "sigtor speed:" << speed;

            double estmDays = 1.0;

            // approx forward to succeeding pmsor:
            if (speed > 0.0)
            {
                // distance to succeeding promissor in ecliptic degree:
                QEclLont succDist = conjPos.dist_to(succPos);

                // linear estimation of remaining days of progression:
                estmDays = succDist / speed;

//                qDebug() << "succ dist:" << succDist;
//                qDebug() << "estm days:" << estmDays;
            }

            // approx backward to preceeding pmsor:
            else
            {
                // distance to preceding promissor in ecliptic degree:
                QEclLont precDist = conjPos.dist_to(precPos);

                estmDays = precDist / speed;

//                qDebug() << "prec dist:" << precDist;
//                qDebug() << "estm days:" << estmDays;
            }

            if (estmDays > 1.0)
            {
                estmDays = 1.0;
            }
            else if (estmDays < -1.0)
            {
                estmDays = -1.0;
            }

            // stepping forward in time with estimated amount of days:
            conjTime = conjTime.addSecs(qint64(estmDays * 86400.0));

//            qDebug() << "conj time:" << conjTime;
//            qDebug() << "estm days:" << estmDays;

            // recalculation of progression position:
            sigtor->calcEclPos(QHoraCoords(conjTime, mTzDiff));
            conjPos = sigtor->eclPos();
        }
    }

    return conjTime;
}
