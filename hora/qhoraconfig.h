
#ifndef __SYMBOID_ASTRO_HORA_QHORACONFIG_H__
#define __SYMBOID_ASTRO_HORA_QHORACONFIG_H__

#include "astro/hora/defs.h"
#include "sdk/arch/mainobject.h"
#include "astro/calculo/hora.h"
#include "sdk/controls/qjsonsyncnode.h"
#include <QList>
#include <QSet>

class ASTRO_HORA_API QHoraConfig : public QJsonSyncFile
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
};


#endif // __SYMBOID_ASTRO_HORA_QHORACONFIG_H__
