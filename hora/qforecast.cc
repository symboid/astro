
#include "astro/hora/setup.h"
#include "astro/hora/qforecast.h"
#include <QDebug>

QForecast::QForecast(QObject* parent)
    : QObject(parent)
    , mModel(nullptr)
    , mGeoLatt(0.0)
    , mGeoLont(0.0)
    , mTzDiff(0.0)
    , mAspectList({0, 180, 120, 90, 60, 150, 30})
    , mPrmsorList(nullptr)
{
}

QHoraCoords* QForecast::periodBegin() const
{
    return mPeriodBegin;
}

void QForecast::setPeriodBegin(QHoraCoords* periodBegin)
{
    mPeriodBegin = periodBegin;
}

QHoraCoords* QForecast::periodEnd() const
{
    return mPeriodEnd;
}

void QForecast::setPeriodEnd(QHoraCoords* periodEnd)
{
    mPeriodEnd = periodEnd;
}

qreal QForecast::geoLatt() const
{
    return mGeoLatt;
}

qreal QForecast::geoLont() const
{
    return mGeoLont;
}

qreal QForecast::tzDiff() const
{
    return mTzDiff;
}

void QForecast::setGeoLatt(qreal geoLatt)
{
    mGeoLatt = geoLatt;
}

void QForecast::setGeoLont(qreal geoLont)
{
    mGeoLont = geoLont;
}

void QForecast::setTzDiff(qreal tzDiff)
{
    mTzDiff = tzDiff;
}

QForecastModel* QForecast::model() const
{
    return mModel;
}

void QForecast::setModel(QForecastModel* model)
{
    mModel = model;
}

int QForecast::forecastEventCount() const
{
    return mEvents.size();
}

const QForecastEvent* QForecast::forecastEvent(int eventIndex) const
{
    return mEvents.at(eventIndex);
}

QForecastEvent* QForecast::createEvent(QSigtor* sigtor, QHoraCoords* earliestTime)
{
    QForecastEvent* event = nullptr;

    mModel->initSigtorPos(sigtor, *earliestTime);

    QAspectObjectList::Siblings siblings = mPrmsorList->find(sigtor->eclPos());
    if (siblings.mPrec && siblings.mSucc)
    {
        event = new QForecastEvent(sigtor);
        QHoraCoords* exactCoords = mModel->calcConj(sigtor, earliestTime, siblings);

        event->setEventExact(exactCoords);

        QEclPos sigtorPos = sigtor->eclPos();
        bool precIsCloser = sigtorPos.dist_abs(siblings.mPrec->eclPos()) <
                sigtorPos.dist_abs(siblings.mSucc->eclPos());
        QPrmsor* prmsor = precIsCloser ? siblings.mPrec : siblings.mSucc;
        event->setPrmsor(prmsor);

//        event->setEventBegin(mModel->calcOrbisTime(sigtor, prmsor, exactTime, true));
//        event->setEventEnd(mModel->calcOrbisTime(sigtor, prmsor, exactTime, false));
//qDebug() << "SIGTOR POS:" << sigtor->eclPos()._M_lont.to_arc_degree() << ", "
//         << "PRMSOR POS:" << prmsor->eclPos()._M_lont.to_arc_degree();
    }
    return event;
}

void QForecast::initEvents()
{
    mEventBuffer.clear();
    QVector<QSigtor*> sigtors = mModel->sigtorList();
    for (QSigtor* sigtor : sigtors)
    {
        mEventBuffer.insert(createEvent(sigtor, mPeriodBegin));
    }

    for (QForecastEvent* event : mEvents)
    {
        event->deleteLater();
    }
    mEvents.clear();
    mEvents.reserve(mModel->estimatedEventCount(mPeriodBegin, mPeriodEnd) * 2);
}

void QForecast::calc()
{
    mPrmsorList = mModel->hora() ? mModel->hora()->fetchAspectObjects(mAspectList) : nullptr;

    initEvents();

    QHoraCoords currentTime;
    currentTime = *mPeriodBegin;

    while (currentTime.ephTime() < mPeriodEnd->ephTime())
    {
        QForecastEvent* nextEvent = mEventBuffer.pop();
        if (nextEvent)
        {
            currentTime.setEphTime(nextEvent->eventExact()->ephTime() + eph::basic_calendar<eph_proxy>::days(1.0/24.0));
            mEventBuffer.insert(createEvent(nextEvent->sigtor()->clone(), &currentTime));
            mEvents.push_back(nextEvent);
        }
        else
        {
            break;
        }
    }
}
