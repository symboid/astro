
#ifndef __SYMBOID_ASTRO_HORA_QFORECAST_H__
#define __SYMBOID_ASTRO_HORA_QFORECAST_H__

#include "astro/hora/defs.h"
#include "astro/hora/qforecastmodel.h"
#include "astro/hora/qforecastevent.h"
#include <QThread>

class ASTRO_HORA_API QCalcTask : public QObject
{
    Q_OBJECT

public:
    QCalcTask(QObject* parent);

public:
    virtual void calc() = 0;
    void run();

public:
    void setExecutionThread(QThread* executionthread);
    QThread* executionThread() const;
private:
    QThread* mExecutionThread;

    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
private:
    bool mRunning;
public:
    bool running() const;
    void setRunning(bool running);
signals:
    void runningChanged();

public:
    Q_PROPERTY(qreal progress READ progress NOTIFY progressChanged)
    qreal progress() const;
protected:
    void setProgressPos(qint64 progressPos);
    void setProgressTotal(qint64 progressTotal);
private:
    qint64 mProgressPos;
    qint64 mProgressTotal;
signals:
    void progressChanged();

public:
    bool isAborted() const;
signals:
    void finished();
    void aborted();

public:
    bool restarting();
    bool restarted();
    void setRestarted();
    QMutex mRestartMutex;
    int mRestartCount;
};

class ASTRO_HORA_API QCalcThread : public QThread
{
    Q_OBJECT
public:
    QCalcThread(QObject* parent, QCalcTask* calcTask);

private:
    void run() override;
public:
    void startCalc();

private:
    QCalcTask* mCalcTask;
    QMutex mCalcMutex;
};


class ASTRO_HORA_API QForecast : public QCalcTask
{
    Q_OBJECT

public:
    QForecast(QObject* parent);

public:
    QHoraCoords* periodBegin() const;
    void setPeriodBegin(QHoraCoords* periodBegin);
    QHoraCoords* periodEnd() const;
    void setPeriodEnd(QHoraCoords* periodEnd);
private:
    QScopedPointer<QHoraCoords> mPeriodBegin;
    QScopedPointer<QHoraCoords> mPeriodEnd;

public:
    QForecastModel* model() const;
    void setModel(QForecastModel* model);
private:
    QScopedPointer<QForecastModel> mModel;

public:
    qreal geoLatt() const;
    qreal geoLont() const;
    qreal tzDiff() const;
    void setGeoLatt(qreal geoLatt);
    void setGeoLont(qreal geoLont);
    void setTzDiff(qreal tzDiff);
private:
    qreal mGeoLatt;
    qreal mGeoLont;
    qreal mTzDiff;

public slots:
    void calc() override;

public:
    int forecastEventCount() const;
    const QForecastEvent* forecastEvent(int eventIndex) const;
private:
    QForecastEvent* createEvent(const QSigtor* sigtor, QHoraCoords* earliestTime);
    QVector<QForecastEvent*> mEvents;
    QVector<qreal> mAspectList;
    QSharedPointer<QAspectObjectList> mPrmsorList;
};

#endif // __SYMBOID_ASTRO_HORA_QFORECAST_H__
