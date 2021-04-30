
#include "astro/hora/setup.h"
#include "astro/hora/qforecast.h"

QForecast::QForecast(QObject* parent)
    : QObject(parent)
    , mModel(nullptr)
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

QForecastModel* QForecast::model() const
{
    return mModel;
}

void QForecast::setModel(QForecastModel* model)
{
    if (mModel)
    {
        disconnect(mModel, SIGNAL(horaChanged()), this, SLOT(calc()));
    }
    mModel = model;
    if (mModel)
    {
        connect(mModel, SIGNAL(horaChanged()), this, SLOT(calc()));
    }
}

int QForecast::forecastEventCount() const
{
    return mEvents.size();
}

const QForecastEvent* QForecast::forecastEvent(int eventIndex) const
{
    return mEvents.at(eventIndex);
}

QForecastEvent* QForecast::createEvent(const QSigtor* sigtor, const QDateTime& earliestTime)
{
    QSigtor* sigtorCopy = sigtor->clone();
    mModel->initSigtorPos(sigtorCopy, earliestTime);
    QForecastEvent* event = new QForecastEvent(this, sigtorCopy);
event->setPrmsor(sigtor->clone());
    event->setEventExact(earliestTime);
    return event;
}

void QForecast::initEvents()
{
    mEventBuffer.clear();
    QVector<const QSigtor*> sigtors = mModel->sigtorList();
    for (const QSigtor* sigtor : sigtors)
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
    initEvents();
    mEvents.push_back(mEventBuffer.pop());
/*
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
*/
}
