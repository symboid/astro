
#ifndef __SYMBOID_ASTRO_HORA_QDIREXMODEL_H__
#define __SYMBOID_ASTRO_HORA_QDIREXMODEL_H__

#include "astro/hora/defs.h"
#include "astro/hora/qforecastmodel.h"

class ASTRO_HORA_API QDirexModel : public QForecastModel
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "DirexModel";

public:
    QDirexModel(QObject* parent = nullptr);

private slots:
    void resetMasterSigtors() override;

public:
    void initSigtorPos(QSigtor* sigtor, const QHoraCoords& eventCoords) override;
    QHoraCoords* calcConj(QSigtor* sigtor, const QHoraCoords* startTime,
            const QMagItemList::Siblings& siblings) override;

public:
    int estimatedEventCount(const QHoraCoords* perdioBegin, const QHoraCoords* periodEnd) const override;
};

#endif // __SYMBOID_ASTRO_HORA_QDIREXMODEL_H__
