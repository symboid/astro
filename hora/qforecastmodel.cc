
#include "astro/hora/setup.h"
#include "astro/hora/qforecastmodel.h"

QForecastModel::QForecastModel(QObject* parent)
    : QHoraTableModel(Q_NULLPTR, parent)
{
}

int QForecastModel::rowCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant QForecastModel::data(const QModelIndex& index, int role) const
{
    QVariant value("23");
    return value;
}

QHash<int, QByteArray> QForecastModel::roleNames() const
{
    QHash<int, QByteArray> roles(QAbstractTableModel::roleNames());
    return roles;
}

QStringList QForecastModel::headerModel() const
{
    return { "", "Sgtor house" };
}
