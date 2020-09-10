
#ifndef __SYMBOID_ASTRO_HORA_QFORECASTMODEL_H__
#define __SYMBOID_ASTRO_HORA_QFORECASTMODEL_H__

#include "astro/hora/defs.h"
#include "astro/hora/qhoraitemsmodel.h"

class ASTRO_HORA_API QForecastModel : public QHoraTableModel
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "ForecastModel";

public:
    QForecastModel(QObject* parent = Q_NULLPTR);

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public:
    QStringList headerModel() const override;
};

#endif // __SYMBOID_ASTRO_HORA_QFORECASTMODEL_H__
