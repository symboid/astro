
#include "astro/hora/setup.h"
#include "astro/hora/qtransitmodel.h"

QTransitModel::QTransitModel(QObject* parent)
    : QForecastModel(parent)
    , mTzDiff(0.0)
{
}

void QTransitModel::resetMasterSigtors()
{
    for (QSigtor* sigtor : mMasterSigtors)
    {
        sigtor->deleteLater();
    }
    mMasterSigtors.clear();

    if (mHora)
    {
        for (QHora::Planets::ConstIterator planet = mHora->planetsBegin(); planet != mHora->planetsEnd(); ++planet)
        {
            if (planetIsSigtor(*planet))
            {
                mMasterSigtors.push_back(new QPlanetSigtor(*planet));
            }
        }
    }
}

void QTransitModel::initSigtorPos(QSigtor* sigtor, const QHoraCoords& eventCoords)
{
    if (sigtor)
    {
        sigtor->calcEclPos(eventCoords);
    }
}

QHoraCoords* QTransitModel::calcConj(QSigtor* sigtor, const QHoraCoords* startTime,
        const QAspectObjectList::Siblings& siblings)
{
    return calcTransitCoords(sigtor, startTime, siblings.mPrec->eclPos(), siblings.mSucc->eclPos());
}

int QTransitModel::estimatedEventCount(const QHoraCoords* periodBegin, const QHoraCoords* periodEnd) const
{
    static constexpr double AVG_DIREX_COUNT_PER_MONTH = 2.0;
    double lengthInDays = (periodEnd->ephTime()-periodBegin->ephTime()).count();
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

QHoraCoords* QTransitModel::calcTransitCoords(QSigtor* sigtor, const QHoraCoords* startTime,
        const QEclPos& precPos, const QEclPos& succPos)
{
    QEphTime conjTime = startTime->ephTime();
    const eph::arc_degree geoLatt = startTime->geoLatt();
    const eph::arc_degree geoLont = startTime->geoLont();
    const double tzDiff = startTime->tzDiff();

    if (sigtor)
    {
        double DIST_TOLERANCE = 1.0 / 3600.0;

        sigtor->calcEclPos(*startTime);
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
            conjTime += eph::basic_calendar<eph_proxy>::days(estmDays);

//            qDebug() << "conj time:" << conjTime;
//            qDebug() << "estm days:" << estmDays;

            // recalculation of progression position:
            sigtor->calcEclPos(conjTime, geoLatt, geoLont);
            conjPos = sigtor->eclPos();
        }
    }

    QHoraCoords* conjCoords = new QHoraCoords(conjTime);
    conjCoords->setGeoLatt(geoLatt);
    conjCoords->setGeoLont(geoLont);
    conjCoords->setTzDiff(tzDiff);
    return conjCoords;
}
