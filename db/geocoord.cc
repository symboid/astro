
#include "astro/db/setup.h"
#include "astro/db/geocoord.h"

eph::arc_coord::degree QGeoLattSectionCalc::raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    return (sectionIndex == north_index) ? sectionDegree : -sectionDegree;
}

eph::arc_coord::signum QGeoLattSectionCalc::signum(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    Q_UNUSED(sectionDegree);
    return sectionIndex == 1 ? -1 : 1;
}

eph::arc_coord::degree QGeoLattSectionCalc::degree(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const
{
    rawDegree *= signum;
    if (rawDegree < -sectionMax() || rawDegree > sectionMax())
    {
        return sectionMax();
    }
    else
    {
        return qAbs(rawDegree);
    }
}

int QGeoLattSectionCalc::index(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const
{
    Q_UNUSED(rawDegree);
    return signum != -1 ? north_index : south_index;
}

eph::arc_coord::degree QGeoLontSectionCalc::raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    return (sectionIndex == east_index) ? sectionDegree : -sectionDegree;
}

eph::arc_coord::signum QGeoLontSectionCalc::signum(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    Q_UNUSED(sectionDegree);
    return sectionIndex == 1 ? -1 : 1;
}

eph::arc_coord::degree QGeoLontSectionCalc::normalizeDegree(eph::arc_coord::degree rawDegree)
{
    while (rawDegree >= 180) rawDegree -= 360;
    while (rawDegree < -180) rawDegree += 360;
    return rawDegree;
}

eph::arc_coord::degree QGeoLontSectionCalc::degree(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const
{
    return qAbs(normalizeDegree(signum * rawDegree));
}

int QGeoLontSectionCalc::index(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const
{
    Q_UNUSED(rawDegree);
    return signum != -1 ? east_index : west_index;
}

