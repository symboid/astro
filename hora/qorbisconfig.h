
#ifndef __SYMBOID_ASTRO_HORA_QORBISCONFIG_H__
#define __SYMBOID_ASTRO_HORA_QORBISCONFIG_H__

#include "astro/hora/defs.h"
#include "sdk/hosting/qconfig.h"
#include "astro/calculo/planet.h"

#define Q_ORBIS_CONFIG_NODE(Name, sunOrbis,monOrbis,merOrbis,venOrbis,marOrbis, \
                                        jupOrbis,satOrbis,uraOrbis,nepOrbis,pluOrbis) \
class ASTRO_HORA_API Q##Name##OrbisConfigNode : public QConfigNode \
{ \
    Q_OBJECT \
public: \
    Q##Name##OrbisConfigNode(const QString& id, QConfigNode* parent, const char* parentSignal) \
        : QConfigNode(id, parent, parentSignal) \
    { \
    } \
public: \
    Q_CONFIG_PROPERTY(double, sun, sunOrbis) \
    Q_CONFIG_PROPERTY(double, mon, monOrbis) \
    Q_CONFIG_PROPERTY(double, mer, merOrbis) \
    Q_CONFIG_PROPERTY(double, ven, venOrbis) \
    Q_CONFIG_PROPERTY(double, mar, marOrbis) \
    Q_CONFIG_PROPERTY(double, jup, jupOrbis) \
    Q_CONFIG_PROPERTY(double, sat, satOrbis) \
    Q_CONFIG_PROPERTY(double, ura, uraOrbis) \
    Q_CONFIG_PROPERTY(double, nep, nepOrbis) \
    Q_CONFIG_PROPERTY(double, plu, pluOrbis) \
    Q_CONFIG_PROPERTY(double, asc, 0.1) \
    Q_CONFIG_PROPERTY(double, mc,  0.1) \
    Q_CONFIG_PROPERTY(double, node, 0.5) \
    Q_CONFIG_PROPERTY(double, cusp, 0.1) \
};

#define Q_ORBIS_CONFIG_NOD1(ClassName, equOrbis) \
    Q_ORBIS_CONFIG_NODE(ClassName, equOrbis,equOrbis,equOrbis,equOrbis,equOrbis,\
                                   equOrbis,equOrbis,equOrbis,equOrbis,equOrbis)

Q_ORBIS_CONFIG_NODE(Conjunction, 4.0, 4.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 2.5, 2.5)
Q_ORBIS_CONFIG_NODE(Opposition,  4.0, 4.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 2.5, 2.5)
Q_ORBIS_CONFIG_NODE(Trigon,      3.5, 3.5, 3.0, 3.0, 2.5, 2.5, 2.5, 2.5, 2.0, 2.0)
Q_ORBIS_CONFIG_NODE(Quadrat,     3.5, 3.5, 3.0, 3.0, 2.5, 2.5, 2.5, 2.5, 2.0, 2.0)
Q_ORBIS_CONFIG_NOD1(Quintile,    1.0)
Q_ORBIS_CONFIG_NODE(Sextile,     3.0, 3.0, 2.5, 2.5, 2.0, 2.0, 2.0, 2.0, 1.5, 1.5)

Q_ORBIS_CONFIG_NOD1(Semisextile, 1.0)
Q_ORBIS_CONFIG_NOD1(Quincunx,    1.0)
Q_ORBIS_CONFIG_NOD1(Semiquadrat, 1.0)
Q_ORBIS_CONFIG_NOD1(Sesquiquadrat, 1.0)
Q_ORBIS_CONFIG_NOD1(Biquintile,  1.0)

class ASTRO_HORA_API QHouseCuspOrbisConfigNode : public QConfigNode
{
    Q_OBJECT
public:
    QHouseCuspOrbisConfigNode(const QString& id, QConfigNode* parentNode, const char* parentSignal)
        : QConfigNode(id, parentNode, parentSignal)
    {
    }

    Q_CONFIG_PROPERTY(double, asc, 2.0)
    Q_CONFIG_PROPERTY(double, dsc, 2.0)
    Q_CONFIG_PROPERTY(double, mc,  2.0)
    Q_CONFIG_PROPERTY(double, ic,  2.0)
    Q_CONFIG_PROPERTY(double, other, 2.0)
};

class ASTRO_HORA_API QOrbisConfig : public QConfigNode
{
    Q_OBJECT
public:
    QOrbisConfig(const QString& id, QConfigNode* parentNode, const char* parentSignal)
        : QConfigNode(id, parentNode, parentSignal)
    {
    }

    Q_CONFIG_NODE(QConjunctionOrbisConfigNode, conjunction)
    Q_CONFIG_NODE(QOppositionOrbisConfigNode, opposition)
    Q_CONFIG_NODE(QTrigonOrbisConfigNode, trigon)
    Q_CONFIG_NODE(QQuadratOrbisConfigNode, quadrat)
    Q_CONFIG_NODE(QQuintileOrbisConfigNode, quintile)
    Q_CONFIG_NODE(QSextileOrbisConfigNode, sextile)

    Q_CONFIG_NODE(QSemisextileOrbisConfigNode, semi_sextile)
    Q_CONFIG_NODE(QQuincunxOrbisConfigNode, quincunx)
    Q_CONFIG_NODE(QSemiquadratOrbisConfigNode, semi_quadrat)
    Q_CONFIG_NODE(QSesquiquadratOrbisConfigNode, sesqui_quadrat)
    Q_CONFIG_NODE(QBiquintileOrbisConfigNode, biquintile)

    Q_CONFIG_NODE(QHouseCuspOrbisConfigNode, house_cusp)
};

class ASTRO_HORA_API OrbisConfig : public hor::a_orbis_config
{
public:
    OrbisConfig(QOrbisConfig* orbisConfig, int index);
private:
    QOrbisConfig* mOrbisConfig;
    QConfigNode* mAspectConfigs[hor::aspect_type_count];
public:
    hor::orbis aspect_orbis(hor::aspect_type _aspect_type) const override;
};


#endif // __SYMBOID_ASTRO_HORA_QORBISCONFIG_H__
