
#ifndef __SYMBOID_ASTRO_HORA_QHORACONFIG_H__
#define __SYMBOID_ASTRO_HORA_QHORACONFIG_H__

#include "astro/hora/defs.h"
#include "sdk/arch/mainobject.h"
#include "sdk/hosting/qconfig.h"
#include "astro/hora/qaspectconfig.h"
#include "astro/db/qfixstarconfig.h"
#include <QList>
#include <QMap>

class ASTRO_HORA_API QKarmaConfig : public QConfigNode
{
    Q_OBJECT
public:
    QKarmaConfig(const QString& id, QAbstractConfig* parentNode, const char* parentSignal)
        : QConfigNode(id, parentNode, parentSignal)
    {
    }

    Q_CONFIG_PROPERTY(bool, dragon_head, true)
    Q_CONFIG_PROPERTY(bool, dragon_tail, false)
    Q_CONFIG_PROPERTY(bool, lilith, false)
    Q_CONFIG_PROPERTY(bool, draw_axis, true)
};

class ASTRO_HORA_API QHouseCuspOrbisConfigNode : public QConfigNode
{
    Q_OBJECT
public:
    QHouseCuspOrbisConfigNode(const QString& id, QAbstractConfig* parentNode, const char* parentSignal)
        : QConfigNode(id, parentNode, parentSignal)
    {
    }

    Q_CONFIG_PROPERTY(double, asc, 2.0)
    Q_CONFIG_PROPERTY(double, dsc, 2.0)
    Q_CONFIG_PROPERTY(double, mc,  2.0)
    Q_CONFIG_PROPERTY(double, ic,  2.0)
    Q_CONFIG_PROPERTY(double, other, 2.0)
};

class ASTRO_HORA_API QHoraConfig : public QConfigNode
{
    Q_OBJECT
    QML_SINGLETON(HoraConfig)

public:
    QHoraConfig(QObject* parent = Q_NULLPTR);
    ~QHoraConfig();

public:
    Q_CONFIG_NODE(QAspectConfig, aspects)
    Q_CONFIG_NODE(QFixstarConfig, fixstars)
    Q_CONFIG_NODE(QKarmaConfig, karma)
    Q_CONFIG_NODE(QHouseCuspOrbisConfigNode, house_cusp_orbis)
};


#endif // __SYMBOID_ASTRO_HORA_QHORACONFIG_H__
