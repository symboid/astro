
#ifndef __SYMBOID_ASTRO_HORA_QTRANSITMODEL_H__
#define __SYMBOID_ASTRO_HORA_QTRANSITMODEL_H__

#include "astro/hora/defs.h"
#include "astro/hora/qforecastmodel.h"

class ASTRO_HORA_API QTransitModel : public QForecastModel
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "TransitModel";

public:
    QTransitModel(QObject* parent = nullptr);

public:
    QVector<QSigtor*> sigtorList() override;
    void initSigtorPos(QSigtor* sigtor, const QHoraCoords& eventCoords) override;
    QDateTime calcConj(QSigtor* sigtor, const QDateTime& startTime,
            const QAspectObjectList::Siblings& siblings) override;

public:
    int estimatedEventCount(const QDateTime& periodBegin, const QDateTime& periodEnd) const override;

private:
    virtual bool planetIsSigtor(const QPlanet* planet) const;

public:
    void setTzDiff(double tzDiff);
    QDateTime calcTransitTime(QSigtor* sigtor, const QDateTime& startTime,
            const QEclPos& precPos, const QEclPos& succPos);
private:
    double mTzDiff;
};

#endif // __SYMBOID_ASTRO_HORA_QTRANSITMODEL_H__
