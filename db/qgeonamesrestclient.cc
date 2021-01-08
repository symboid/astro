
#include "astro/db/setup.h"
#include "astro/db/qgeonamesrestclient.h"

QGeoNamesRestClient::QGeoNamesRestClient(QObject* parent)
    : QRestClient(parent)
{
    setApiAddress("api.geonames.org");
    setSecure(false);
    setAuthUser("symboid");
}
