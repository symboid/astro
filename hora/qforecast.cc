
#include "astro/hora/setup.h"
#include "astro/hora/qforecast.h"

QForecast::QForecast(QObject* parent)
    : QCalcTask(parent)
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

void QForecast::calc()
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
        setProgressTotal((mPeriodEnd->ephTime() - mPeriodBegin->ephTime()).count());
        while (!isAborted() && !restarted() && currentTime.ephTime() < mPeriodEnd->ephTime())
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
                setProgressPos((nextEvent->eventExact()->ephTime() - mPeriodBegin->ephTime()).count());
            }
        }
        eventBuffer.clear();
    }
}

QCalcTask::QCalcTask(QObject* parent)
    : QObject(parent)
    , mExecutionThread(nullptr)
    , mRunning(false)
    , mProgressPos(00L)
    , mProgressTotal(00L)
{
}

void QCalcTask::run()
{
    setRunning(true);
    setProgressPos(0LL);
    setProgressTotal(0LL);

    calc();

    setProgressPos(0LL);
    setProgressTotal(0LL);
    if (!restarted())
    {
        emit isAborted() ? aborted() : finished();
    }
    setRunning(false);
}

void QCalcTask::setExecutionThread(QThread* executionThread)
{
    mExecutionThread = executionThread;
}

QThread* QCalcTask::executionThread() const
{
    return mExecutionThread;
}

bool QCalcTask::running() const
{
    return mRunning;
}

void QCalcTask::setRunning(bool running)
{
    if (mRunning != running)
    {
        mRunning = running;
        emit runningChanged();
    }
}

qreal QCalcTask::progress() const
{
    return mProgressTotal != 0LL ? qreal(mProgressPos) / qreal(mProgressTotal) : 0LL;
}

void QCalcTask::setProgressPos(qint64 progressPos)
{
    if (mProgressPos != progressPos)
    {
        mProgressPos = progressPos;
        emit progressChanged();
    }
}

void QCalcTask::setProgressTotal(qint64 progressTotal)
{
    if (mProgressTotal != progressTotal)
    {
        mProgressTotal = progressTotal;
        emit progressChanged();
    }
}

bool QCalcTask::isAborted() const
{
    return mExecutionThread->isInterruptionRequested();
}

bool QCalcTask::restarting()
{
    QMutexLocker lock(&mRestartMutex);
    bool isRestarted = mRestartCount > 0;
    mRestartCount = 0;
    return isRestarted;
}

bool QCalcTask::restarted()
{
    QMutexLocker lock(&mRestartMutex);
    return mRestartCount > 0;
}

void QCalcTask::setRestarted()
{
    QMutexLocker lock(&mRestartMutex);
    mRestartCount++;
}

QCalcThread::QCalcThread(QObject* parent, QCalcTask* calcTask)
    : QThread(parent)
    , mCalcTask(calcTask)
{
    mCalcTask->setExecutionThread(this);
}

void QCalcThread::run()
{
    QMutexLocker calcLocker(&mCalcMutex);
    mCalcTask->run();
    while (mCalcTask->restarting())
    {
        mCalcTask->run();
    }
}

void QCalcThread::startCalc()
{
    if (isRunning())
    {
        mCalcTask->setRestarted();
    } else
    {
        start();
    }
}
