
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
        : QConfigNode(id, tr(#Name), parent, parentSignal) \
    { \
    } \
public: \
    Q_CONFIG_PROPERTY(double, sun, sunOrbis, tr("Sun")) \
    Q_CONFIG_PROPERTY(double, mon, monOrbis, tr("Moon")) \
    Q_CONFIG_PROPERTY(double, mer, merOrbis, tr("Mercury")) \
    Q_CONFIG_PROPERTY(double, ven, venOrbis, tr("Venus")) \
    Q_CONFIG_PROPERTY(double, mar, marOrbis, tr("Mars")) \
    Q_CONFIG_PROPERTY(double, jup, jupOrbis, tr("Jupiter")) \
    Q_CONFIG_PROPERTY(double, sat, satOrbis, tr("Saturn")) \
    Q_CONFIG_PROPERTY(double, ura, uraOrbis, tr("Uranus")) \
    Q_CONFIG_PROPERTY(double, nep, nepOrbis, tr("Neptune")) \
    Q_CONFIG_PROPERTY(double, plu, pluOrbis, tr("Pluto")) \
    Q_CONFIG_PROPERTY(double, asc, 0.1,      tr("Asc")) \
    Q_CONFIG_PROPERTY(double, mc,  0.1,      tr("MC")) \
    Q_CONFIG_PROPERTY(double, node, 0.5,     tr("Node")) \
    Q_CONFIG_PROPERTY(double, cusp, 0.1,     tr("House cusp")) \
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
        : QConfigNode(id, tr("Before house cusp"), parentNode, parentSignal)
    {
    }

    Q_CONFIG_PROPERTY(double, asc, 2.0, tr("Asc"))
    Q_CONFIG_PROPERTY(double, dsc, 2.0, tr("Dsc"))
    Q_CONFIG_PROPERTY(double, mc,  2.0, tr("MC"))
    Q_CONFIG_PROPERTY(double, ic,  2.0, tr("IC"))
    Q_CONFIG_PROPERTY(double, other, 2.0, tr("Other"))
};

class ASTRO_HORA_API QOrbisConfig : public QConfigSync
{
    Q_OBJECT
public:
    QOrbisConfig(const QString& id, QConfigNode* parentNode, const char* parentSignal)
        : QConfigSync(id, tr("Orbis"), parentNode, parentSignal)
    {
    }

    Q_CONFIG_NODE(QConjunctionOrbisConfigNode, conjunction)
    Q_CONFIG_NODE(QOppositionOrbisConfigNode, opposition)
    Q_CONFIG_NODE(QTrigonOrbisConfigNode, trigon)
    Q_CONFIG_NODE(QQuadratOrbisConfigNode, quadrat)
    Q_CONFIG_NODE(QQuintileOrbisConfigNode, quintil)
    Q_CONFIG_NODE(QSextileOrbisConfigNode, sextil)

    Q_CONFIG_NODE(QSemisextileOrbisConfigNode, semisextil)
    Q_CONFIG_NODE(QQuincunxOrbisConfigNode, quincunx)
    Q_CONFIG_NODE(QSemiquadratOrbisConfigNode, semiquadrat)
    Q_CONFIG_NODE(QSesquiquadratOrbisConfigNode, sesquiquadrat)
    Q_CONFIG_NODE(QBiquintileOrbisConfigNode, biquintil)

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
