
#include "astro/hora/setup.h"
#include "astro/hora/qforecast.h"
#include <QtConcurrent>

QForecast::QForecast(QObject* parent)
    : QObject(parent)
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

QHoraCoords* QForecast::periodBegin() const
{
    return mPeriodBegin.get();
}

void QForecast::setPeriodBegin(QHoraCoords* periodBegin)
{
    mPeriodBegin.reset(periodBegin);
}

QHoraCoords* QForecast::periodEnd() const
{
    return mPeriodEnd.get();
}

void QForecast::setPeriodEnd(QHoraCoords* periodEnd)
{
    mPeriodEnd.reset(periodEnd);
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
}

double QForecast::progress() const
{
    return mPeriodLength != 0.0 ? mPeriodPos / mPeriodLength : 0.0;
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

    QAspectObjectList::Siblings siblings = mPrmsorList->find(sigtor->eclPos());
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

//        event->setEventBegin(mModel->calcOrbisTime(sigtor, prmsor, exactTime, true));
//        event->setEventEnd(mModel->calcOrbisTime(sigtor, prmsor, exactTime, false));
//qDebug() << "SIGTOR POS:" << sigtor->eclPos()._M_lont.to_arc_degree() << ", "
//         << "PRMSOR POS:" << prmsor->eclPos()._M_lont.to_arc_degree();
    }
    return event;
}

void QForecast::run()
{
    // #1. cleaning up
    for (QForecastEvent* event : mEvents)
    {
        event->deleteLater();
    }
    mEvents.clear();
    mEvents.reserve(mModel->estimatedEventCount(mPeriodBegin.get(), mPeriodEnd.get()) * 2);

    if (mModel && mModel->hora() && mPeriodBegin && mPeriodEnd)
    {
        // #2. list of promissors
        mPrmsorList = mModel->hora()->fetchAspectObjects(mAspectList);

        // #3. initial list of events
        QForecastEventBuffer eventBuffer;
        for (QSigtor* sigtor : mModel->sigtorList())
        {
            eventBuffer.insert(createEvent(sigtor, mPeriodBegin.get()));
        }

        // iterating over period
        QHoraCoords currentTime;
        currentTime = *mPeriodBegin;
        mPeriodLength = (mPeriodEnd->ephTime() - mPeriodBegin->ephTime()).count();
        mPeriodPos = 0.0;
        emit progressChanged();
        while (currentTime.ephTime() < mPeriodEnd->ephTime())
        {
            // taking the nearest event
            QForecastEvent* nextEvent = eventBuffer.pop();
            if (nextEvent)
            {
                // inserting one new event
                currentTime.setEphTime(nextEvent->eventExact()->ephTime() + eph::basic_calendar<eph_proxy>::days(1.0/24.0));
                eventBuffer.insert(createEvent(nextEvent->sigtor(), &currentTime));

                // populating event list
                mEvents.push_back(nextEvent);
                mPeriodPos = (nextEvent->eventExact()->ephTime() - mPeriodBegin->ephTime()).count();
                emit progressChanged();
            }
            else
            {
                break;
            }
        }
        eventBuffer.clear();
        mPeriodPos = 0.0;
        emit progressChanged();
        emit recalculated();
    }
}

void QForecast::calcAsync()
{
    QMutexLocker startLocker(&mStartMutex);
    QtConcurrent::run([this]{
        QMutexLocker calcLocker(&mCalcMutex);
        mStartSync.wakeOne();
        run();
    });
    mStartSync.wait(&mStartMutex);
}

QCalcThread::QCalcThread(QObject* parent, QRunnable* calcable)
    : QThread(parent)
    , mStartCounter(0)
    , mCalcable(calcable)
{
}

void QCalcThread::run()
{
    qDebug() << "Waiting for run";
    QMutexLocker calcLocker(&mCalcMutex);
    mStartSync.wakeOne();
    qDebug() << "Started";
    mCalcable->run();
    qDebug() << "Finished";
}

void QCalcThread::startCalc()
{
    qDebug() << "Waiting for start...";
    QMutexLocker startLocker(&mStartMutex);
    ++mStartCounter;
    qDebug() << "Starting" << mStartCounter;
    start();
    qDebug() << "Waiting for started" << mStartCounter;
    mStartSync.wait(&mStartMutex);
    qDebug() << "Started" << mStartCounter;
}
