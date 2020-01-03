
#ifndef __SYMBOID_ASTRO_UICONTROLS_QT_QGEONAMESRESTCLIENT_H__
#define __SYMBOID_ASTRO_UICONTROLS_QT_QGEONAMESRESTCLIENT_H__

#include "astro/uicontrols-qt/defs.h"
#include "sdk/network-qt/qrestclient.h"
#include "sdk/arch/mainobject.h"

class ASTRO_UICONTROLS_QT_API QGeoNamesRestClient : public QRestClientJSON
{
    Q_OBJECT
    QML_SINGLETON(GeoNamesRestClient)

public:
    QGeoNamesRestClient(QObject* parent = Q_NULLPTR);
};

#endif // __SYMBOID_ASTRO_UICONTROLS_QT_QGEONAMESRESTCLIENT_H__
