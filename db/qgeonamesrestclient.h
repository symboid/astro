
#ifndef __SYMBOID_ASTRO_DB_QGEONAMESRESTCLIENT_H__
#define __SYMBOID_ASTRO_DB_QGEONAMESRESTCLIENT_H__

#include "astro/db/defs.h"
#include "sdk/network/qrestclient.h"
#include "sdk/arch/mainobject.h"

class ASTRO_DB_API QGeoNamesRestClient : public QRestClient
{
    Q_OBJECT
    QML_SINGLETON_OBJECT(GeoNamesRestClient)

public:
    QGeoNamesRestClient(QObject* parent = Q_NULLPTR);
};

#endif // __SYMBOID_ASTRO_DB_QGEONAMESRESTCLIENT_H__
