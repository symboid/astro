
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
    Q_PROPERTY(const QHora* hora MEMBER mHora WRITE setHora NOTIFY horaChanged)
    const QHora* hora() const;
    void setHora(const QHora* hora);
protected:
    const QHora* mHora;
signals:
    void horaChanged();

public:
    virtual QVector<const QSigtor*> sigtorList() const = 0;
    virtual void initSigtorPos(QSigtor* sigtor, const QDateTime& eventTime) = 0;

public:
    virtual int estimatedEventCount(const QDateTime& perdioBegin, const QDateTime& periodEnd) const = 0;
};

#endif // __SYMBOID_ASTRO_HORA_QFORECASTMODEL_H__
