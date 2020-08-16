
#ifndef __SYMBOID_ASTRO_DB_QGEONAMESTABLEMODEL_H__
#define __SYMBOID_ASTRO_DB_QGEONAMESTABLEMODEL_H__

#include "astro/db/defs.h"
#include <QAbstractTableModel>
#include "sdk/network/qresttablemodel.h"

class ASTRO_DB_API QGeoNamesTableModel : public QRestTableModel
{
    Q_OBJECT

public:
    typedef QGeoNamesTableModel Class;
    static constexpr const char* id = "GeoNamesTableModel";
    static constexpr const char* qml_name = "GeoNamesTableModel";

public:
    QGeoNamesTableModel(QObject* parent = Q_NULLPTR);
};

#endif // __SYMBOID_ASTRO_DB_QGEONAMESTABLEMODEL_H__
