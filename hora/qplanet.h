
#ifndef __SYMBOID_ASTRO_HORA_QPLANET_H__
#define __SYMBOID_ASTRO_HORA_QPLANET_H__

#include "astro/hora/defs.h"
#include "astro/hora/qhoraobject.h"

class ASTRO_HORA_API QPlanet : public QHoraObject
{
    Q_OBJECT

public:
    typedef typename eph_proxy::object::index Index;
    QPlanet(QObject* parent, const QString& id, Index index, QOrbisConfigNodeGetter orbisGetter);

    static QString resolveId(QOrbisConfigNodeGetter orbisGetter);
    QPlanet(QObject* parent, QOrbisConfigNodeGetter orbisGetter);

    static constexpr const Index UNDEF = eph_proxy::object::undef;

    static constexpr const Index SUN = eph_proxy::object::sun;
    static constexpr const Index MOON = eph_proxy::object::moon;
    static constexpr const Index MERCURY = eph_proxy::object::mercury;
    static constexpr const Index VENUS = eph_proxy::object::venus;
    static constexpr const Index MARS = eph_proxy::object::mars;
    static constexpr const Index JUPITER = eph_proxy::object::jupiter;
    static constexpr const Index SATURN = eph_proxy::object::saturn;
    static constexpr const Index URANUS = eph_proxy::object::uranus;
    static constexpr const Index NEPTUNE = eph_proxy::object::neptune;
    static constexpr const Index PLUTO = eph_proxy::object::pluto;

    static constexpr const Index CHIRON = eph_proxy::object::chiron;

    static constexpr const Index MEAN_NODE = eph_proxy::object::mean_node;
    static constexpr const Index LILITH = eph_proxy::object::lilith;

private:
    static Index resolveIndex(const QString& id);
public:
    const Index mIndex;

public:
    QEclPos eclPos() const override;
    QEclSpeed eclSpeed() const override;
    virtual bool calc(const QEphTime& ephTime);
protected:
    QEclPos mEclPos;
    QEclSpeed mEclSpeed;

public:
    bool isRetrograd() const;
};

class ASTRO_HORA_API QLunarNode : public QPlanet
{
    Q_OBJECT
public:
    static constexpr const Index DRAGON_HEAD = Index(1000);
    static constexpr const Index DRAGON_TAIL = Index(1001);

public:
    QLunarNode(QObject* parent, Index index);

public:
    bool calc(const QEphTime& ephTime) override;
};

#endif // __SYMBOID_ASTRO_HORA_QPLANET_H__
