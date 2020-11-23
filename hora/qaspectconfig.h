
#ifndef __SYMBOID_ASTRO_HORA_QASPECTCONFIG_H__
#define __SYMBOID_ASTRO_HORA_QASPECTCONFIG_H__

#include "astro/hora/defs.h"
#include "sdk/hosting/qconfig.h"

class ASTRO_HORA_API QAspectConfig : public QConfigNode
{
    Q_OBJECT
    QML_SINGLETON(AspectConfig)
public:
    QAspectConfig(QConfigNode* parentNode);
    ~QAspectConfig();

    Q_CONFIG_PROPERTY(bool, conjunction, true, tr("Conjunction"))
    Q_CONFIG_PROPERTY(bool, opposition, true, tr("Opposition"))
    Q_CONFIG_PROPERTY(bool, trigon, true, tr("Trigon"))
    Q_CONFIG_PROPERTY(bool, quadrat, true, tr("Quadrat"))
    Q_CONFIG_PROPERTY(bool, sextile, true, tr("Sextile"))
    Q_CONFIG_PROPERTY(bool, quintile, false, tr("Quintile"))

    Q_CONFIG_PROPERTY(bool, semisextile, false, tr("Semi-sextile"))
    Q_CONFIG_PROPERTY(bool, quincunx, true, tr("Quincunx"))
    Q_CONFIG_PROPERTY(bool, semiquadrat, false, tr("Semi-quadrat"))
    Q_CONFIG_PROPERTY(bool, sesquiquadrat, false, tr("Sesqui-quadrat"))
    Q_CONFIG_PROPERTY(bool, biquintile, false, tr("Biquintile"))
};

#endif // __SYMBOID_ASTRO_HORA_QASPECTCONFIG_H__
