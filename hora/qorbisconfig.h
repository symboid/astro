
#ifndef __SYMBOID_ASTRO_HORA_QORBISCONFIG_H__
#define __SYMBOID_ASTRO_HORA_QORBISCONFIG_H__

#include "astro/hora/defs.h"
#include "sdk/hosting/qconfig.h"
#include "astro/calculo/planet.h"

#define Q_ORBIS_CONFIG_NODE(Name, sunOrbis,monOrbis,merOrbis,venOrbis,marOrbis, \
                                        jupOrbis,satOrbis,uraOrbis,nepOrbis,pluOrbis) \
class Q##Name##ConfigNode : public QConfigNode \
{ \
    Q_OBJECT \
public: \
    Q##Name##ConfigNode(QConfigNode* parent, const char* parentSignal) \
        : QConfigNode(tr(#Name), parent, parentSignal) \
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
Q_ORBIS_CONFIG_NOD1(Quintil,     1.0)
Q_ORBIS_CONFIG_NODE(Sextil,      3.0, 3.0, 2.5, 2.5, 2.0, 2.0, 2.0, 2.0, 1.5, 1.5)

Q_ORBIS_CONFIG_NOD1(Semisextil,  1.0)
Q_ORBIS_CONFIG_NOD1(Quincunx,    1.0)
Q_ORBIS_CONFIG_NOD1(Semiquadrat, 1.0)
Q_ORBIS_CONFIG_NOD1(Sesquiquadrat, 1.0)
Q_ORBIS_CONFIG_NOD1(Biquintil,   1.0)

class QHouseCuspConfigNode : public QConfigNode
{
    Q_OBJECT
public:
    QHouseCuspConfigNode(QConfigNode* parentNode, const char* parentSignal)
        : QConfigNode(tr("Before house cusp"), parentNode, parentSignal)
    {
    }

    Q_CONFIG_PROPERTY(double, asc, 2.0, tr("Asc"))
    Q_CONFIG_PROPERTY(double, dsc, 2.0, tr("Dsc"))
    Q_CONFIG_PROPERTY(double, mc,  2.0, tr("MC"))
    Q_CONFIG_PROPERTY(double, ic,  2.0, tr("IC"))
    Q_CONFIG_PROPERTY(double, other, 2.0, tr("Other"))
};

class QSynastryConfigNode : public QConfigNode
{
    Q_OBJECT
public:
    QSynastryConfigNode(QConfigNode* parentNode, const char* parentSignal)
        : QConfigNode(tr("Synastry"), parentNode, parentSignal)
    {
    }

    Q_CONFIG_PROPERTY(double, multiplier, 0.3, tr("Orbis multiplier"))
};

class QOrbisConfig : public QConfigNode
{
    Q_OBJECT
    QML_SINGLETON(OrbisConfig)
public:
    QOrbisConfig(QConfigNode* parentNode);

    Q_CONFIG_NODE(QConjunctionConfigNode, conjunction)
    Q_CONFIG_NODE(QOppositionConfigNode, opposition)
    Q_CONFIG_NODE(QTrigonConfigNode, trigon)
    Q_CONFIG_NODE(QQuadratConfigNode, quadrat)
    Q_CONFIG_NODE(QQuintilConfigNode, quintil)
    Q_CONFIG_NODE(QSextilConfigNode, sextil)

    Q_CONFIG_NODE(QSemisextilConfigNode, semisextil)
    Q_CONFIG_NODE(QQuincunxConfigNode, quincunx)
    Q_CONFIG_NODE(QSemiquadratConfigNode, semiquadrat)
    Q_CONFIG_NODE(QSesquiquadratConfigNode, sesquiquadrat)
    Q_CONFIG_NODE(QBiquintilConfigNode, biquintil)

    Q_CONFIG_NODE(QHouseCuspConfigNode, house_cusp)
    Q_CONFIG_NODE(QSynastryConfigNode, synastry)
};

class OrbisConfig : public hor::a_orbis_config
{
public:
    OrbisConfig(int index);
private:
    arh::main_object<QOrbisConfig> mOrbisConfig;
    QConfigNode* mAspectConfigs[hor::aspect_type_count];
public:
    hor::orbis aspect_orbis(hor::aspect_type _aspect_type) const override;
};


#endif // __SYMBOID_ASTRO_HORA_QORBISCONFIG_H__
