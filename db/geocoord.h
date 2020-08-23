
#ifndef __SYMBOID_ASTRO_DB_GEOCOORD_H__
#define __SYMBOID_ASTRO_DB_GEOCOORD_H__

#include "astro/db/defs.h"
#include "astro/controls/arccoord.h"

struct ASTRO_DB_API QGeoLattSectionCalc : QSectionCalc
{
    Q_OBJECT
public:

    static constexpr const char* qml_name = "GeoLattSectionCalc";

    eph::arc_coord::degree raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const override;
    virtual eph::arc_coord::signum signum(int sectionIndex, eph::arc_coord::degree sectionDegree) const override;
    eph::arc_coord::degree degree(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const override;
    int index(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const override;
    Q_INVOKABLE virtual int sectionCount() const override { return 2; }
    Q_INVOKABLE virtual int sectionMin() const override { return 0; }
    Q_INVOKABLE virtual int sectionMax() const override { return 90; }
    Q_INVOKABLE virtual QStringList values() const override { return { tr("N"), tr("S") }; }

    static constexpr int north_index = 0;
    static constexpr int south_index = 1;
};

struct ASTRO_DB_API QGeoLontSectionCalc : QSectionCalc
{
    Q_OBJECT
public:

    static constexpr const char* qml_name = "GeoLontSectionCalc";

    eph::arc_coord::degree raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const override;
    virtual eph::arc_coord::signum signum(int sectionIndex, eph::arc_coord::degree sectionDegree) const override;
    eph::arc_coord::degree degree(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const override;
    int index(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const override;
    Q_INVOKABLE virtual int sectionCount() const override { return 2; }
    Q_INVOKABLE virtual int sectionMin() const override { return 0; }
    Q_INVOKABLE virtual int sectionMax() const override { return 180; }
    Q_INVOKABLE virtual QStringList values() const override { return { tr("E"), tr("W") }; }

    static constexpr int east_index = 0;
    static constexpr int west_index = 1;

    static eph::arc_coord::degree normalizeDegree(eph::arc_coord::degree rawDegree);
};


#endif // __SYMBOID_ASTRO_DB_GEOCOORD_H__
