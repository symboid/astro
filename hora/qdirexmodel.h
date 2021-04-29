
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
    QVector<QSigtor*> sigtorList() const override;
};

#endif // __SYMBOID_ASTRO_HORA_QDIREXMODEL_H__
