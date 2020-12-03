
#ifndef __SYMBOID_ASTRO_HORA_QORBISCONFIG_H__
#define __SYMBOID_ASTRO_HORA_QORBISCONFIG_H__

#include "astro/hora/defs.h"
#include "sdk/hosting/qconfig.h"
#include "astro/eph/ecliptic.h"
#include <QColor>

class ASTRO_HORA_API QOrbisConfigNode : public QConfigNode
{
    Q_OBJECT
public:
    QOrbisConfigNode(const QString& id, QAbstractConfig* parent, const char* parentSignal,
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
    Q_CONFIG_PROPERTY(double, cusp, 0.1)

    Q_CONFIG_PROPERTY(double, nod, 1.0)
    Q_CONFIG_PROPERTY(double, lil, 1.0)
};

typedef QAbstractConfig*(QOrbisConfigNode::*QOrbisConfigNodeGetter)() const;

#define Q_ORBIS_CONFIG_NODE(orbisDefaults) \
    Q_CONFIG_NODE_INTERFACE(QOrbisConfigNode,orbis)\
    private: \
        QOrbisConfigNode* _M_orbis = createConfig<QOrbisConfigNode>("orbis", \
                this,SIGNAL(orbisChanged()), orbisDefaults );


struct ASTRO_HORA_API QAspectProperties
{
    QAspectProperties(bool isMain, eph::ecl_pos::dist dist, const QColor& lineColor, double lineWidth)
      : mIsMain(isMain)
      , mDist(dist)
      , mLineColor(lineColor)
      , mLineWidth(lineWidth)
    {}
    const bool mIsMain;
    eph::ecl_pos::dist mDist;
    QColor mLineColor;
    double mLineWidth;
};

class ASTRO_HORA_API QAspectDrawConfig : public QConfigNode
{
    Q_OBJECT

public:
    QAspectDrawConfig(const QString& id, QAbstractConfig* parent, const char* parentSignal,
            double lineWidth, const QColor& lineColor)
        : QConfigNode(id, parent, parentSignal)
        , mLineWidthDefault(lineWidth)
        , mLineColorDefault(lineColor)
    {
    }
private:
    const double mLineWidthDefault;
    const QColor mLineColorDefault;

public:
    Q_CONFIG_PROPERTY(double, lineWidth, mLineWidthDefault)
    Q_CONFIG_PROPERTY(QColor, lineColor, mLineColorDefault)
};

#define Q_ASPECT_DRAW_CONFIG_NODE(aspectProperties) \
    Q_CONFIG_NODE_INTERFACE(QAspectDrawConfig,draw) \
    private: \
        QAspectDrawConfig* _M_draw = createConfig<QAspectDrawConfig>("draw",this,SIGNAL(drawChanged()), \
                aspectProperties.mLineWidth, aspectProperties.mLineColor); \

class ASTRO_HORA_API QAspectConfigNode : public QConfigNode
{
    Q_OBJECT
public:
    QAspectConfigNode(const QString& id, QAbstractConfig* parent, const char* parentSignal,
            const QAspectProperties& properties, bool enabledDefault, const double* orbisDefaults)
        : QConfigNode(id, parent, parentSignal)
        , mProperties(properties)
        , mEnabledDefault(enabledDefault)
        , mOrbisDefaults(orbisDefaults)
    {
    }

private:
    const QAspectProperties mProperties;
    const bool mEnabledDefault;
    const double* mOrbisDefaults;

public:
    Q_CONFIG_PROPERTY(double, dist, mProperties.mDist);
    Q_CONFIG_PROPERTY(bool, isMain, mProperties.mIsMain);
    Q_CONFIG_PROPERTY(bool, enabled, mEnabledDefault);
    Q_ORBIS_CONFIG_NODE(mOrbisDefaults);
    Q_ASPECT_DRAW_CONFIG_NODE(mProperties);
};

#define Q_ASPECT_CONFIG_NODE(name,dist,isMain,lineColor,lineWidth,enabledDefault,...) \
    Q_CONFIG_NODE_INTERFACE(QAspectConfigNode,name) \
    private: \
        const double name##OrbisDefaults[10] = { __VA_ARGS__ }; \
        QAspectConfigNode* _M_##name = createConfig<QAspectConfigNode>(#name,this,SIGNAL(name##Changed()), \
                QAspectProperties(isMain,dist,lineColor,lineWidth), enabledDefault, name##OrbisDefaults ); \

class QHoraObject;

class ASTRO_HORA_API QAspectConfig : public QConfigContainer<QAspectConfigNode>
{
    Q_OBJECT
public:
    QAspectConfig(const QString& id, QAbstractConfig* parentNode, const char* parentSignal)
        : QConfigContainer<QAspectConfigNode>(id, parentNode, parentSignal)
    {
    }

    Q_ASPECT_CONFIG_NODE(conjunction,      0.0, true, "black", 1.0,
                                          true, 4.0, 4.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 2.5, 2.5)

    Q_ASPECT_CONFIG_NODE(opposition,     180.0, true, "red", 0.25,
                                          true, 4.0, 4.0, 3.0, 3.0, 3.0, 3.0, 3.0, 3.0, 2.5, 2.5)

    Q_ASPECT_CONFIG_NODE(trigon,         120.0, true, "green", 0.25,
                                          true, 3.5, 3.5, 3.0, 3.0, 2.5, 2.5, 2.5, 2.5, 2.0, 2.0)

    Q_ASPECT_CONFIG_NODE(quadrat,         90.0, true, "red", 0.25,
                                          true, 3.5, 3.5, 3.0, 3.0, 2.5, 2.5, 2.5, 2.5, 2.0, 2.0)

    Q_ASPECT_CONFIG_NODE(quintile,        72.0, true, "blue", 0.125,
                                         false, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0)

    Q_ASPECT_CONFIG_NODE(sextile,         60.0, true, "green", 0.25,
                                          true, 3.0, 3.0, 2.5, 2.5, 2.0, 2.0, 2.0, 2.0, 1.5, 1.5)



    Q_ASPECT_CONFIG_NODE(semi_sextile,    30.0, false, "darkred", 0.125,
                                         false, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0)

    Q_ASPECT_CONFIG_NODE(quincunx,       150.0, false, "darkred", 0.125,
                                          true, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0)

    Q_ASPECT_CONFIG_NODE(semi_quadrat,    45.0, false, "darkred", 0.125,
                                         false, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0)

    Q_ASPECT_CONFIG_NODE(sesqui_quadrat, 135.0, false, "darkred", 0.125,
                                         false, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0)

    Q_ASPECT_CONFIG_NODE(biquintile,     144.0, false, "blue", 0.125,
                                         false, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0, 1.0)

public:
    const QAspectConfigNode* findConnection(const QHoraObject* leftObject, const QHoraObject* rightObject) const;
};

#endif // __SYMBOID_ASTRO_HORA_QORBISCONFIG_H__
