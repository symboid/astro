
#ifndef __SYMBOID_ASTRO_HORA_QHOUSECUSP_H__
#define __SYMBOID_ASTRO_HORA_QHOUSECUSP_H__

#include "astro/hora/defs.h"
#include "astro/hora/qhoraobject.h"

class ASTRO_HORA_API QHouseSystem : public QObject
{
    Q_OBJECT
public:
    QHouseSystem(QObject* parent);

    enum Type
    {
        PLACIDUS = int(eph_proxy::houses::type::placidus),
        KOCH = int(eph_proxy::houses::type::koch),
        REGIOMONTANUS = int(eph_proxy::houses::type::regiomontanus),
        CAMPANUS = int(eph_proxy::houses::type::campanus),
        EQUAL = int(eph_proxy::houses::type::equal),
    };
    Type mType = PLACIDUS;

    static constexpr int HOUSE_COUNT = 12;
    QEclLont mEclLonts[HOUSE_COUNT + 1];
    QEclSpeed mEclSpeed[HOUSE_COUNT + 1];

    bool calc(eph::basic_time_point<eph_proxy> horaTime, eph::arc_degree geoLont, eph::arc_degree geoLatt);
};

class ASTRO_HORA_API QHouseCusp : public QHoraObject
{
    Q_OBJECT

private:
    static QString resolveName(int houseIndex);
    static QOrbisConfigNodeGetter resolveConfigGetter(int houseIndex);
public:
    QHouseCusp(QObject* parent, const QHouseSystem* houseSystem, int houseIndex);

public:
    const QHouseSystem* mHouseSystem;
    const int mHouseIndex;

public:
    QEclPos eclPos() const override;
    QEclSpeed eclSpeed() const override;
    QString abbrName() const override;
    QString symbol(const QAstroFont* font) const override;
    QColor drawColor() const override;
};

#endif // __SYMBOID_ASTRO_HORA_QHOUSECUSP_H__
