
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
    virtual QVector<QSigtor*> sigtorList() = 0;
    virtual void initSigtorPos(QSigtor* sigtor, const QHoraCoords& eventCoords) = 0;
    virtual QHoraCoords* calcConj(QSigtor* sigtor, const QHoraCoords* startTime,
            const QAspectObjectList::Siblings& siblings) = 0;

public:
    virtual int estimatedEventCount(const QHoraCoords* periodBegin, const QHoraCoords* periodEnd) const = 0;
};

#endif // __SYMBOID_ASTRO_HORA_QFORECASTMODEL_H__
