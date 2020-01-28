
#ifndef __SYMBOID_ASTRO_UICONTROLS_QT_QHORASTELLIUM_H__
#define __SYMBOID_ASTRO_UICONTROLS_QT_QHORASTELLIUM_H__

#include "astro/uicontrols-qt/defs.h"
#include <QList>
#include "astro/calculo/planet.h"

hor_ns_begin
typedef basic_planet<eph_proxy> planet;
hor_ns_end

struct QHoraStellium : QList<hor::planet>
{
    QHoraStellium(const eph::arc_degree& equDist, const hor::planet& planet);

    eph::ecl_pos realBegin() const;
    eph::ecl_pos realEnd() const;
    eph::ecl_lont realWidth() const;

    eph::ecl_pos displayBegin() const;
    eph::ecl_pos displayEnd() const;
    eph::ecl_pos displayPos(int planetIndex) const;
    eph::ecl_lont displayWidth() const;

    eph::arc_degree padding() const;
    inline bool isExpanding() const { return padding() >= 0; }
    inline bool isCollapsing() const { return padding() <= 0; }

    eph::ecl_lont leftDist(const QHoraStellium& rhs) const;
    eph::ecl_lont rightDist(const QHoraStellium& rhs) const;
    bool isMergeable(const QHoraStellium& rhs) const;
    QHoraStellium& mergeIn(const QHoraStellium& rhs);

private:
    eph::arc_degree mEquDist;
};


#endif // __SYMBOID_ASTRO_UICONTROLS_QT_QHORASTELLIUM_H__
