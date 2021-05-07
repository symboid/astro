
#ifndef __SYMBOID_ASTRO_HORA_QFORECAST_H__
#define __SYMBOID_ASTRO_HORA_QFORECAST_H__

#include "astro/hora/defs.h"
#include "astro/hora/qforecastmodel.h"
#include "astro/hora/qforecastevent.h"

class ASTRO_HORA_API QForecast : public QObject
{
    Q_OBJECT

public:
    QForecast(QObject* parent = nullptr);

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

public slots:
    void calc();

public:
    int forecastEventCount() const;
    const QForecastEvent* forecastEvent(int eventIndex) const;
private:
    QForecastEvent* createEvent(const QSigtor* sigtor, QHoraCoords* earliestTime);
    QVector<QForecastEvent*> mEvents;
    QVector<qreal> mAspectList;
    QSharedPointer<QAspectObjectList> mPrmsorList;
    QMutex mCalcMutex;
};

#endif // __SYMBOID_ASTRO_HORA_QFORECAST_H__
