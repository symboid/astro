
#ifndef __SYMBOID_ASTRO_DB_QGEONAMESMODEL_H__
#define __SYMBOID_ASTRO_DB_QGEONAMESMODEL_H__

#include "astro/db/defs.h"
#include "sdk/uicontrols-qt/qjsonsyncmodel.h"

class ASTRO_DB_API QGeoNameItem : public QJsonSyncNode
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "GeoNameItem";

    QGeoNameItem(QObject* parent = Q_NULLPTR) : QJsonSyncNode(parent) {}

    Q_PROPERTY(QString geoName MEMBER mGeoName CONSTANT)
    QString mGeoName;

    Q_PROPERTY(qreal geoLont MEMBER mGeoLont CONSTANT)
    qreal mGeoLont;

    Q_PROPERTY(qreal geoLatt MEMBER mGeoLatt CONSTANT)
    qreal mGeoLatt;

    Q_PROPERTY(QString countryName MEMBER mCountryName CONSTANT)
    QString mCountryName;

    Q_PROPERTY(QString adminName MEMBER mAdminName CONSTANT)
    QString mAdminName;
};

class ASTRO_DB_API QGeoNamesModel : public QJsonSyncModel<QGeoNameItem>
{
    Q_OBJECT

public:
    QGeoNamesModel(QObject* parent = Q_NULLPTR)
        : QJsonSyncModel<QGeoNameItem>(parent)
    {
    }
};

#endif // __SYMBOID_ASTRO_DB_QGEONAMESMODEL_H__
