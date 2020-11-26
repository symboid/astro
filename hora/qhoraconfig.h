
#ifndef __SYMBOID_ASTRO_HORA_QHORACONFIG_H__
#define __SYMBOID_ASTRO_HORA_QHORACONFIG_H__

#include "astro/hora/defs.h"
#include "sdk/arch/mainobject.h"
#include "astro/calculo/hora.h"
#include "sdk/hosting/qconfig.h"
#include "astro/hora/qaspectconfig.h"
#include "astro/hora/qorbisconfig.h"
#include "astro/db/qfixstarconfig.h"
#include <QList>
#include <QMap>

class ASTRO_HORA_API QKarmaConfig : public QConfigNode
{
    Q_OBJECT
public:
    QKarmaConfig(const QString& id, QConfigNode* parentNode, const char* parentSignal)
        : QConfigNode(id, parentNode, parentSignal)
    {
    }

    Q_CONFIG_PROPERTY(bool, dragon_head, true)
    Q_CONFIG_PROPERTY(bool, dragon_tail, false)
    Q_CONFIG_PROPERTY(bool, lilith, false)
    Q_CONFIG_PROPERTY(bool, draw_axis, true)
};

class ASTRO_HORA_API QHoraConfig : public QConfigNode
{
    Q_OBJECT
    QML_SINGLETON(HoraConfig)

public:
    QHoraConfig(QObject* parent = Q_NULLPTR);
    ~QHoraConfig();

public:
    typedef QList<hor::planet> Planets;
    Planets mPlanets;
    typedef QMap<hor::aspect_type, QConfigNode*> AspectTypes;
    AspectTypes mAspectTypes;

    Q_CONFIG_NODE(QAspectConfig, aspects)
    Q_CONFIG_NODE(QOrbisConfig, orbis)
    Q_CONFIG_NODE(QFixstarConfig, fixstars)
    Q_CONFIG_NODE(QKarmaConfig, karma)
};


#endif // __SYMBOID_ASTRO_HORA_QHORACONFIG_H__
