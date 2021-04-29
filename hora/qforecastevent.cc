
#include "astro/hora/setup.h"
#include "astro/hora/qforecastevent.h"

QForecastEvent::QForecastEvent(QObject* parent, QSigtor* sigtor)
    : QObject(parent)
    , mSigtor(sigtor)
{
}

QDateTime QForecastEvent::eventBegin() const
{
    return mEventExact;
}

QDateTime QForecastEvent::eventExact() const
{
    return mEventExact;
}

QDateTime QForecastEvent::eventEnd() const
{
    return mEventExact;
}

void QForecastEvent::setEventExact(const QDateTime& eventExact)
{
    if (mEventExact != eventExact)
    {
        mEventExact = eventExact;
        emit eventBeginChanged();
        emit eventExactChanged();
        emit eventEndChanged();
    }
}

const QPrmsor* QForecastEvent::prmsor() const
{
    return mPrmsor;
}

const QSigtor* QForecastEvent::sigtor() const
{
    return mSigtor;
}

QPrmsor* QForecastEvent::prmsor()
{
    return mPrmsor;
}

QSigtor* QForecastEvent::sigtor()
{
    return mSigtor;
}

void QForecastEvent::setPrmsor(QPrmsor* prmsor)
{
    mPrmsor = prmsor;
}

void QForecastEvent::setSigtor(QSigtor* sigtor)
{
    mSigtor = sigtor;
}

void QForecastEventBuffer::clear()
{
    for (QForecastEvent* event : *this)
    {
        event->deleteLater();
    }
    QList<QForecastEvent*>::clear();
}

void QForecastEventBuffer::insert(QForecastEvent* newEvent)
{
    // insert-sorting based on event's exact time
    iterator insertPos = begin(), eventEnd = end();
    while (insertPos != eventEnd && (*insertPos)->eventExact() < newEvent->eventExact())
    {
        insertPos++;
    }
    QList<QForecastEvent*>::insert(insertPos, newEvent);
}

QForecastEvent* QForecastEventBuffer::pop()
{
    return isEmpty() ? nullptr : takeFirst();
}
