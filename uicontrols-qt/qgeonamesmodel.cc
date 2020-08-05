
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/qgeonamesmodel.h"

QGeoNamesModel::QGeoNamesModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

int QGeoNamesModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return mGeoNames.size();
}

QVariant QGeoNamesModel::data(const QModelIndex& index, int role) const
{
    QVariant dataValue;
    int itemIndex = index.row();
    if (0 <= itemIndex && itemIndex < mGeoNames.size())
    {
        const QGeoNameItem& item = mGeoNames[itemIndex];
        switch (role)
        {
        case GeoName: dataValue = item.mName; break;
        case GeoLatt: dataValue = item.mLatt; break;
        case GeoLont: dataValue = item.mLont; break;
        case CountryName: dataValue = item.mCountryName; break;
        case AdminName: dataValue = item.mAdminName; break;
        }
    }
    return dataValue;
}

QHash<int,QByteArray> QGeoNamesModel::roleNames() const
{
    QHash<int,QByteArray> roles(QAbstractListModel::roleNames());
    roles[GeoName] = "geoName";
    roles[GeoLont] = "geoLont";
    roles[GeoLatt] = "geoLatt";
    roles[CountryName] = "countryName";
    roles[AdminName] = "adminName";
    return roles;
}
