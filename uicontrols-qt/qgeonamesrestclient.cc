
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/qgeonamesrestclient.h"

QGeoNamesRestClient::QGeoNamesRestClient(QObject* parent)
    : QRestClientJSON(QUrl("http://api.geonames.org"), parent)
{
    setAuthUser("symboid");
}
