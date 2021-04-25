
#ifndef __SYMBOID_ASTRO_HORA_QFORECAST_H__
#define __SYMBOID_ASTRO_HORA_QFORECAST_H__

#include "astro/hora/defs.h"
#include "astro/hora/qforecastmodel.h"
#include <QDateTime>

class QForecast
{
public:
    QForecast();

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
};

#endif // __SYMBOID_ASTRO_HORA_QFORECAST_H__