
#include "astro/hora/setup.h"
#include "astro/hora/qforecast.h"

QForecast::QForecast(QObject* parent)
    : QCalcable(parent)
    , mPeriodBegin(nullptr)
    , mPeriodEnd(nullptr)
    , mModel(nullptr)
    , mGeoLatt(0.0)
    , mGeoLont(0.0)
    , mTzDiff(0.0)
    , mAspectList({0, 180, 120, 90, 60, 150, 30})
    , mPrmsorList(nullptr)
{
}

QForecast::~QForecast()
{
}

QHoraCoords* QForecast::periodBegin() const
{
    return mPeriodBegin.get();
}

void QForecast::setPeriodBegin(QHoraCoords* periodBegin)
{
    if (mPeriodBegin.get() != periodBegin)
    {
        deleteParam(mPeriodBegin.get());
        mPeriodBegin.reset(periodBegin);
        addParam(mPeriodBegin.get());
        emit periodBeginChanged();
    }
}

QHoraCoords* QForecast::periodEnd() const
{
    return mPeriodEnd.get();
}

void QForecast::setPeriodEnd(QHoraCoords* periodEnd)
{
    if (mPeriodEnd.get() != periodEnd)
    {
        deleteParam(mPeriodEnd.get());
        mPeriodEnd.reset(periodEnd);
        addParam(mPeriodEnd.get());
        emit periodEndChanged();
    }
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
    return mModel.get();
}

void QForecast::setModel(QForecastModel* model)
{
    mModel.reset(model);
    mCalcTask->invoke();
}

int QForecast::forecastEventCount() const
{
    return mEvents.size();
}

const QForecastEvent* QForecast::forecastEvent(int eventIndex) const
{
    return mEvents.at(eventIndex);
}

QForecastEvent* QForecast::createEvent(const QSigtor* masterSigtor, QHoraCoords* earliestTime)
{
    QForecastEvent* event = nullptr;
    QSigtor* sigtor = masterSigtor->clone();

    mModel->initSigtorPos(sigtor, *earliestTime);

    QMagItemList::Siblings siblings = mPrmsorList->find(sigtor->eclPos());
    if (siblings.mPrec && siblings.mSucc)
    {
        event = new QForecastEvent(sigtor);
        sigtor->setParent(event);

        QHoraCoords* exactCoords = mModel->calcConj(sigtor, earliestTime, siblings);

        event->setEventExact(exactCoords);

        QEclPos sigtorPos = sigtor->eclPos();
        bool precIsCloser = sigtorPos.dist_abs(siblings.mPrec->eclPos()) <
                sigtorPos.dist_abs(siblings.mSucc->eclPos());
        QPrmsor* prmsor = precIsCloser ? siblings.mPrec : siblings.mSucc;
        event->setPrmsor(prmsor);
    }
    return event;
}

void QForecast::calc()
{
    // #1. cleaning up
    for (QForecastEvent* event : mEvents)
    {
        event->deleteLater();
    }
    mEvents.clear();

    if (mCalcTask && mModel && mModel->hora() && mPeriodBegin && mPeriodEnd)
    {
        mEvents.reserve(mModel->estimatedEventCount(mPeriodBegin.get(), mPeriodEnd.get()) * 2);

        // #2. list of promissors
        mPrmsorList = mModel->hora()->fetchAspectItems(mAspectList);

        // #3. initial list of events
        QForecastEventBuffer eventBuffer;
        for (QSigtor* sigtor : mModel->sigtorList())
        {
            eventBuffer.insert(createEvent(sigtor, mPeriodBegin.get()));
        }

        // iterating over period
        QHoraCoords currentTime;
        currentTime = *mPeriodBegin;
        mCalcTask->setProgressTotal((mPeriodEnd->ephTime() - mPeriodBegin->ephTime()).count());
        while (!mCalcTask->isAborted() && !mCalcTask->restarted() && currentTime.ephTime() < mPeriodEnd->ephTime())
        {
            // taking the nearest event
            QForecastEvent* nextEvent = eventBuffer.pop();
            if (!nextEvent)
            {
                break;
            }
            else
            {
                // inserting one new event
                currentTime.setEphTime(nextEvent->eventExact()->ephTime() + eph::basic_calendar<eph_proxy>::days(1.0/24.0));
                eventBuffer.insert(createEvent(nextEvent->sigtor(), &currentTime));

                // populating event list
                mEvents.push_back(nextEvent);
                mCalcTask->setProgressPos((nextEvent->eventExact()->ephTime() - mPeriodBegin->ephTime()).count());
            }
        }
        eventBuffer.clear();
    }
    else
    {
        mCalcTask->abort();
    }
}
