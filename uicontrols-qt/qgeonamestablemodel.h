
#ifndef __SYMBOID_ASTRO_UICONTROLS_QT_QGEONAMESTABLEMODEL_H__
#define __SYMBOID_ASTRO_UICONTROLS_QT_QGEONAMESTABLEMODEL_H__

#include "astro/uicontrols-qt/defs.h"
#include <QAbstractTableModel>
#include "sdk/network-qt/qresttablemodel.h"

class ASTRO_UICONTROLS_QT_API QGeoNamesTableModel : public QRestTableModel
{
    Q_OBJECT

public:
    typedef QGeoNamesTableModel Class;
    static constexpr const char* id = "GeoNamesTableModel";
    static constexpr const char* qml_name = "GeoNamesTableModel";

public:
    QGeoNamesTableModel(QObject* parent = Q_NULLPTR);
};

#endif // __SYMBOID_ASTRO_UICONTROLS_QT_QGEONAMESTABLEMODEL_H__
