
#ifndef __SYMBOID_ASTRO_HORA_QFORECASTMODEL_H__
#define __SYMBOID_ASTRO_HORA_QFORECASTMODEL_H__

#include "astro/hora/defs.h"
#include <QObject>
#include "astro/hora/qforecastevent.h"
#include "astro/hora/qhora.h"

class QForecastModel : public QObject
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "ForecastModel";

public:
    QForecastModel(QObject* parent = nullptr);

public:
    QHora* hora() const;
    void setHora(QHora* hora);
protected:
    QHora* mHora;
signals:
    void horaChanged();

public:
    virtual void initCalc(const QDateTime& radixTime) = 0;
    virtual QVector<QSigtor*> sigtorList() const = 0;
    virtual void initSigtorPos(QSigtor* sigtor, const QDateTime& eventTime) = 0;

public:
    virtual int estimatedEventCount(const QDateTime& perdioBegin, const QDateTime& periodEnd) const = 0;
};

#endif // __SYMBOID_ASTRO_HORA_QFORECASTMODEL_H__
