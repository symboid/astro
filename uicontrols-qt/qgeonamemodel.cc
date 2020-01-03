
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/qgeonamemodel.h"

QGeoNameModel::QGeoNameModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int QGeoNameModel::rowCount(const QModelIndex& parent) const
{
    return 0;
}

QVariant QGeoNameModel::data(const QModelIndex& index, int role) const
{
    return QVariant();
}

QHash<int,QByteArray> QGeoNameModel::roleNames() const
{
    return QAbstractListModel::roleNames();
}
