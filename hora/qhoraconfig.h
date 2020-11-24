
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
#include <QSet>

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
    typedef QSet<hor::aspect_type> AspectTypes;
    AspectTypes mAspectTypes;

    Q_CONFIG_NODE(QAspectConfig, aspects)
    Q_CONFIG_NODE(QOrbisConfig, orbis)
    Q_CONFIG_NODE(QFixstarConfig, fixstars)
};


#endif // __SYMBOID_ASTRO_HORA_QHORACONFIG_H__
