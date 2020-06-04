
#ifndef __SYMBOID_ASTRO_UICONTROLS_QT_QORBISCONFIG_H__
#define __SYMBOID_ASTRO_UICONTROLS_QT_QORBISCONFIG_H__

#include "astro/uicontrols-qt/defs.h"
#include "sdk/hosting/qconfig.h"
#include <QAbstractListModel>

class QOrbisModel : public QAbstractListModel//, public QConfigNode
{
    Q_OBJECT

public:
    QOrbisModel(QObject* parent);

public:
    enum Roles
    {
        ObjectNameRole = Qt::UserRole,
        OrbisRole,
    };
    Q_ENUM(Roles)
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex& index = QModelIndex()) const override;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
/*
public:
    Q_CONFIG_PROPERTY(double, sunOrbis, 1.0)
    Q_CONFIG_PROPERTY(double, monOrbis, 1.0)
    Q_CONFIG_PROPERTY(double, merOrbis, 1.0)
    Q_CONFIG_PROPERTY(double, venOrbis, 1.0)
    Q_CONFIG_PROPERTY(double, marOrbis, 1.0)
    Q_CONFIG_PROPERTY(double, jupOrbis, 1.0)
    Q_CONFIG_PROPERTY(double, satOrbis, 1.0)
    Q_CONFIG_PROPERTY(double, uraOrbis, 1.0)
    Q_CONFIG_PROPERTY(double, nepOrbis, 1.0)
    Q_CONFIG_PROPERTY(double, pluOrbis, 1.0)
*/
private:
    const int mObjectCount = 10;
};

class QOrbisConfig : public QConfig
{
    Q_OBJECT
    QML_SINGLETON(OrbisConfig)
public:
    QOrbisConfig(QObject* parent);
};

#endif // __SYMBOID_ASTRO_UICONTROLS_QT_QORBISCONFIG_H__
