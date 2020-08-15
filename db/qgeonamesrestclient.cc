
#include "astro/db/setup.h"
#include "astro/db/qgeonamesrestclient.h"

QGeoNamesRestClient::QGeoNamesRestClient(QObject* parent)
    : QRestClient(parent)
{
    setApiAddress(QUrl("http://api.geonames.org"));
    setAuthUser("symboid");
}
