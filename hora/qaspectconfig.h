
#ifndef __SYMBOID_ASTRO_HORA_QASPECTCONFIG_H__
#define __SYMBOID_ASTRO_HORA_QASPECTCONFIG_H__

#include "astro/hora/defs.h"
#include "sdk/hosting/qconfig.h"

class ASTRO_HORA_API QAspectConfig : public QConfigNode
{
    Q_OBJECT
public:
    QAspectConfig(const QString& id, QConfigNode* parentNode, const char* parentSignal)
        : QConfigNode(id, parentNode, parentSignal)
    {
    }

    Q_CONFIG_PROPERTY(bool, conjunction, true)
    Q_CONFIG_PROPERTY(bool, opposition, true)
    Q_CONFIG_PROPERTY(bool, trigon, true)
    Q_CONFIG_PROPERTY(bool, quadrat, true)
    Q_CONFIG_PROPERTY(bool, quintile, false)
    Q_CONFIG_PROPERTY(bool, sextile, true)

    Q_CONFIG_PROPERTY(bool, semi_sextile, false)
    Q_CONFIG_PROPERTY(bool, quincunx, true)
    Q_CONFIG_PROPERTY(bool, semi_quadrat, false)
    Q_CONFIG_PROPERTY(bool, sesqui_quadrat, false)
    Q_CONFIG_PROPERTY(bool, biquintile, false)
};

#endif // __SYMBOID_ASTRO_HORA_QASPECTCONFIG_H__
