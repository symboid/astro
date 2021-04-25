
#include "astro/hora/setup.h"
#include "astro/hora/qforecastevent.h"

QForecastEvent::QForecastEvent(QObject* parent)
    : QObject(parent)
{
}

QDateTime QForecastEvent::eventBegin() const
{
    return mEventTime;
}

QDateTime QForecastEvent::eventEnd() const
{
    return mEventTime;
}

void QForecastEvent::setEventTime(const QDateTime& eventTime)
{
    if (mEventTime != eventTime)
    {
        mEventTime = eventTime;
        emit eventBeginChanged();
        emit eventEndChanged();
    }
}
