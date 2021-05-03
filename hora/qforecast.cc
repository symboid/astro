
#include "astro/hora/setup.h"
#include "astro/hora/qforecast.h"
#include <QDebug>

QForecast::QForecast(QObject* parent)
    : QObject(parent)
    , mModel(nullptr)
    , mGeoLatt(0.0)
    , mGeoLont(0.0)
    , mTzDiff(0.0)
    , mPrmsorList(nullptr)
{
}

QDateTime QForecast::periodBegin() const
{
    return mPeriodBegin;
}

void QForecast::setPeriodBegin(const QDateTime& periodBegin)
{
    mPeriodBegin = periodBegin;
}

QDateTime QForecast::periodEnd() const
{
    return mPeriodEnd;
}

void QForecast::setPeriodEnd(const QDateTime& periodEnd)
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

QForecastEvent* QForecast::createEvent(QSigtor* sigtor, const QDateTime& earliestTime)
{
    QForecastEvent* event = nullptr;

    QHoraCoords initCoords;
    initCoords.setDateTime(earliestTime);
    initCoords.setGeoLatt(mGeoLatt);
    initCoords.setGeoLont(mGeoLont);
    initCoords.setTzDiff(mTzDiff);
    mModel->initSigtorPos(sigtor, initCoords);

    QAspectObjectList::Siblings siblings = mPrmsorList->find(sigtor->eclPos());
if (siblings.mSucc)
{
QPrmsor* prmsor = siblings.mPrec;
event = new QForecastEvent(this, sigtor);
event->setPrmsor(prmsor);
event->setEventExact(earliestTime);
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
//    mModel->initCalc();
    mPrmsorList = mModel->hora() ? & mModel->hora()->allAspectObjects() : nullptr;
    if (mPrmsorList)
    {
        qDebug() << "------------------------------------";
        for (QPrmsor* prmsor : *mPrmsorList)
        {
//            qDebug() << "PRMSOR:" << prmsor->abbrName() << "POS:" << prmsor->eclPos()._M_lont.to_arc_degree();
        }
        qDebug() << "------------------------------------";
    }
    initEvents();
    while (!mEventBuffer.isEmpty()) mEvents.push_back(mEventBuffer.pop());
return;

    QDateTime currentTime = mPeriodBegin;
    while (currentTime < mPeriodEnd)
    {
        QForecastEvent* nextEvent = mEventBuffer.pop();
        if (nextEvent)
        {
            QDateTime currentTime = nextEvent->eventExact().addSecs(3600);
            mEventBuffer.insert(createEvent(nextEvent->sigtor()->clone(), currentTime));
            mEvents.push_back(nextEvent);
        }
    }
}
