
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
    mModel = model;
}

QForecastEvent* QForecast::createEvent(QSigtor* sigtor, const QDateTime& eventTime)
{
    QForecastEvent* event = new QForecastEvent(this);
    event->setSigtor(sigtor);
    event->setEventTime(eventTime);
    mModel->initSigtorPos(sigtor, eventTime);
    return event;
}

void QForecast::initEvents()
{
    for (QForecastEvent* event : mEventBuffer)
    {
        event->deleteLater();
    }
    mEventBuffer.clear();

    QVector<QSigtor*> sigtors = mModel->sigtorList();
    for (QSigtor* sigtor : sigtors)
    {
        mEventBuffer.push_back(createEvent(sigtor, mPeriodBegin));
    }
}
