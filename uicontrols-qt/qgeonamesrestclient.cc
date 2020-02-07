
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/qgeonamesrestclient.h"

QGeoNamesRestClient::QGeoNamesRestClient(QObject* parent)
    : QRestClient(QUrl("http://api.geonames.org"), parent)
{
    setAuthUser("symboid");
}
