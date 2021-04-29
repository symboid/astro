
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
    QForecastEvent* createEvent(QSigtor* sigtor, const QDateTime& eventTime);
public:
    void initEvents();
private:
    QVector<QForecastEvent*> mEventBuffer;
};

#endif // __SYMBOID_ASTRO_HORA_QFORECAST_H__
