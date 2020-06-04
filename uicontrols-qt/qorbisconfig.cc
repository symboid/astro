
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/qorbisconfig.h"

QOrbisModel::QOrbisModel(QObject* parent)
    : QAbstractListModel(parent)
//    , QConfigNode(parent)
{
}

QHash<int, QByteArray> QOrbisModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[ObjectNameRole] = "object_name";
    roles[OrbisRole] = "orbis";
    return roles;
}

int QOrbisModel::rowCount(const QModelIndex& index) const
{
    return mObjectCount;
}

QVariant QOrbisModel::data(const QModelIndex& index, int role) const
{
    QVariant orbisData;

    return orbisData;
}

QOrbisConfig::QOrbisConfig(QObject* parent)
{

}
