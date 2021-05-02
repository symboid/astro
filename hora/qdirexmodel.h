
#ifndef __SYMBOID_ASTRO_HORA_QDIREXMODEL_H__
#define __SYMBOID_ASTRO_HORA_QDIREXMODEL_H__

#include "astro/hora/defs.h"
#include "astro/hora/qforecastmodel.h"

class QDirexModel : public QForecastModel
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "DirexModel";

public:
    QDirexModel(QObject* parent = nullptr);

public:
    void initCalc(const QDateTime& radixTime) override;
    QVector<QSigtor*> sigtorList() const override;
    void initSigtorPos(QSigtor* sigtor, const QHoraCoords& eventCoords) override;

public:
    int estimatedEventCount(const QDateTime& perdioBegin, const QDateTime& periodEnd) const override;

private:
    QDateTime mRadixTime;
};

#endif // __SYMBOID_ASTRO_HORA_QDIREXMODEL_H__
