
#ifndef __SYMBOID_ASTRO_UICONTROLS_QT_QGEONAMEMODEL_H__
#define __SYMBOID_ASTRO_UICONTROLS_QT_QGEONAMEMODEL_H__

#include "astro/uicontrols-qt/defs.h"
#include <QAbstractListModel>
#include "sdk/arch/mainobject.h"

class ASTRO_UICONTROLS_QT_API QGeoNameModel : public QAbstractListModel
{
    Q_OBJECT
    QML_SINGLETON(GeoNameModel)

public:
    QGeoNameModel(QObject* parent = Q_NULLPTR);

public:
    enum Role
    {
        GeoLont,
        GeoLatt,
        Country,
        TimeZone,
        TimeZoneDiff,
    };
public:
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;
};

#endif // __SYMBOID_ASTRO_UICONTROLS_QT_QGEONAMEMODEL_H__
