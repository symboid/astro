
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/qgeonamesrestclient.h"

QGeoNamesRestClient::QGeoNamesRestClient(QObject* parent)
    : QRestClient(parent)
{
    setApiAddress(QUrl("http://api.geonames.org"));
    setAuthUser("symboid");
}
