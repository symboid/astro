
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
    virtual void run() = 0;

    Q_PROPERTY(bool running READ running WRITE setRunning NOTIFY runningChanged)
private:
    bool mRunning;
public:
    bool running() const;
    void setRunning(bool running);
signals:
    void runningChanged();
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

    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
public:
    double progress() const;
private:
    QEphTime::rep mPeriodPos;
    QEphTime::rep mPeriodLength;
signals:
    void progressChanged();
    void recalculated();
    void aborted();

public slots:
    void run() override;

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
