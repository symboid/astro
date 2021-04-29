
#ifndef __SYMBOID_ASTRO_HORA_QFORECAST_H__
#define __SYMBOID_ASTRO_HORA_QFORECAST_H__

#include "astro/hora/defs.h"
#include "astro/hora/qforecastmodel.h"
#include <QDateTime>
#include "astro/hora/qforecastevent.h"

class QForecast : public QObject
{
    Q_OBJECT

public:
    QForecast(QObject* parent = nullptr);

public:
    QDateTime periodBegin() const;
    void setPeriodBegin(const QDateTime& periodBegin);
    QDateTime periodEnd() const;
    void setPeriodEnd(const QDateTime& periodEnd);
private:
    QDateTime mPeriodBegin;
    QDateTime mPeriodEnd;

public:
    QForecastModel* model() const;
    void setModel(QForecastModel* model);
private:
    QForecastModel* mModel;

private:
    QForecastEvent* createEvent(const QSigtor* sigtor, const QDateTime& eventTime);
    void initEvents();
private:
    QForecastEventBuffer mEventBuffer;
    QVector<QForecastEvent*> mEvents;
public:
    void calc();
};

#endif // __SYMBOID_ASTRO_HORA_QFORECAST_H__
