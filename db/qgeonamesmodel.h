
#ifndef __SYMBOID_ASTRO_DB_QGEONAMESMODEL_H__
#define __SYMBOID_ASTRO_DB_QGEONAMESMODEL_H__

#include "astro/db/defs.h"
#include <QAbstractListModel>
#include "sdk/arch/mainobject.h"
#include <QVector>

struct ASTRO_DB_API QGeoNameItem
{
    QString mName;
    qreal mLont;
    qreal mLatt;
    QString mCountryName;
    QString mAdminName;
};

class ASTRO_DB_API QGeoNamesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    QGeoNamesModel(QObject* parent);

private:
    enum Role
    {
        GeoName,
        GeoLont,
        GeoLatt,
        CountryName,
        AdminName,
    };
public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

private:
    QVector<QGeoNameItem> mGeoNames;
};

#endif // __SYMBOID_ASTRO_DB_QGEONAMESMODEL_H__
