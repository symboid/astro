
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
    QHoraCoords* mPeriodBegin;
    QHoraCoords* mPeriodEnd;

public:
    QForecastModel* model() const;
    void setModel(QForecastModel* model);
private:
    QForecastModel* mModel;

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

private:
    void initEvents();
private:
    QForecastEventBuffer mEventBuffer;
public slots:
    void calc();

public:
    int forecastEventCount() const;
    const QForecastEvent* forecastEvent(int eventIndex) const;
private:
    QForecastEvent* createEvent(QSigtor* sigtor, QHoraCoords* earliestTime);
    QVector<QForecastEvent*> mEvents;
    QVector<qreal> mAspectList;
    QSharedPointer<QAspectObjectList> mPrmsorList;
};

#endif // __SYMBOID_ASTRO_HORA_QFORECAST_H__
