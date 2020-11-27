
#ifndef __SYMBOID_ASTRO_HORA_QORBISCONFIG_H__
#define __SYMBOID_ASTRO_HORA_QORBISCONFIG_H__

#include "astro/hora/defs.h"
#include "sdk/hosting/qconfig.h"
#include "astro/calculo/planet.h"

class ASTRO_HORA_API QOrbisConfigNode : public QConfigNode
{
    Q_OBJECT
public:
    QOrbisConfigNode(const QString& id, QConfigNode* parent, const char* parentSignal,
            const double* orbisDefaults)
        : QConfigNode(id, parent, parentSignal)
        , mOrbisDefaults(orbisDefaults)
    {
    }

private:
    const double* mOrbisDefaults;

public:
    Q_CONFIG_PROPERTY(double, sun, mOrbisDefaults[0])
    Q_CONFIG_PROPERTY(double, mon, mOrbisDefaults[1])
    Q_CONFIG_PROPERTY(double, mer, mOrbisDefaults[2])
    Q_CONFIG_PROPERTY(double, ven, mOrbisDefaults[3])
    Q_CONFIG_PROPERTY(double, mar, mOrbisDefaults[4])
    Q_CONFIG_PROPERTY(double, jup, mOrbisDefaults[5])
    Q_CONFIG_PROPERTY(double, sat, mOrbisDefaults[6])
    Q_CONFIG_PROPERTY(double, ura, mOrbisDefaults[7])
    Q_CONFIG_PROPERTY(double, nep, mOrbisDefaults[8])
    Q_CONFIG_PROPERTY(double, plu, mOrbisDefaults[9])
    Q_CONFIG_PROPERTY(double, asc, 0.1)
    Q_CONFIG_PROPERTY(double, mc,  0.1)
    Q_CONFIG_PROPERTY(double, node, 0.5)
    Q_CONFIG_PROPERTY(double, cusp, 0.1)
};

#define Q_ORBIS_CONFIG_NODE(orbisDefaults) \
    Q_CONFIG_NODE_INTERFACE(QOrbisConfigNode,orbis)\
    private: \
        QOrbisConfigNode* _M_orbis = new QOrbisConfigNode("orbis",this,SIGNAL(orbisChanged()), \
                orbisDefaults );


class ASTRO_HORA_API QAspectConfigNode : public QConfigNode
{
    Q_OBJECT
public:
    QAspectConfigNode(const QString& id, QConfigNode* parent, const char* parentSignal,
            bool enabledDefault, const double* orbisDefaults)
        : QConfigNode(id, parent, parentSignal)
        , mEnabledDefault(enabledDefault)
        , mOrbisDefaults(orbisDefaults)
    {
    }

private:
    const bool mEnabledDefault;
    const double* mOrbisDefaults;

public:
    Q_CONFIG_PROPERTY(bool, enabled, mEnabledDefault);
    Q_ORBIS_CONFIG_NODE(mOrbisDefaults);
};

#define Q_ASPECT_CONFIG_NODE(name,enabledDefault,...) \
    Q_CONFIG_NODE_INTERFACE(QAspectConfigNode,name) \
    private: \
        const double name##OrbisDefaults[10] = { __VA_ARGS__ }; \
        QAspectConfigNode* _M_##name = new QAspectConfigNode(#name,this,SIGNAL(name##Changed()), \
                enabledDefault, name##OrbisDefaults ); \

class ASTRO_HORA_API QAspectConfig : public QConfigNode
{
    Q_OBJECT
public:
    QAspectConfig(const QString& id, QConfigNode* parentNode, const char* parentSignal)
        : QConfigNode(id, parentNode, parentSignal)
    {
    }

    Q_ASPECT_CONFIG_NODE(conjunction,    true,  4.0, 4.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 2.5, 2.5)
    Q_ASPECT_CONFIG_NODE(opposition,     true,  4.0, 4.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 2.5, 2.5)
    Q_ASPECT_CONFIG_NODE(trigon,         true,  3.5, 3.5, 3.0, 3.0, 2.5, 2.5, 2.5, 2.5, 2.0, 2.0)
    Q_ASPECT_CONFIG_NODE(quadrat,        true,  3.5, 3.5, 3.0, 3.0, 2.5, 2.5, 2.5, 2.5, 2.0, 2.0)
    Q_ASPECT_CONFIG_NODE(quintile,       false, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0)
    Q_ASPECT_CONFIG_NODE(sextile,        true,  3.0, 3.0, 2.5, 2.5, 2.0, 2.0, 2.0, 2.0, 1.5, 1.5)

    Q_ASPECT_CONFIG_NODE(semi_sextile,   false, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0)
    Q_ASPECT_CONFIG_NODE(quincunx,       false, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0)
    Q_ASPECT_CONFIG_NODE(semi_quadrat,   false, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0)
    Q_ASPECT_CONFIG_NODE(sesqui_quadrat, false, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0)
    Q_ASPECT_CONFIG_NODE(biquintile,     false, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0)
};

class ASTRO_HORA_API OrbisConfig : public hor::a_orbis_config
{
public:
    OrbisConfig(QAspectConfig* aspectConfig, int index);
private:
    QConfigNode* mAspectConfigs[hor::aspect_type_count];
public:
    hor::orbis aspect_orbis(hor::aspect_type _aspect_type) const override;
};


#endif // __SYMBOID_ASTRO_HORA_QORBISCONFIG_H__
