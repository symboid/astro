
#ifndef __SYMBOID_ASTRO_HORA_QREVOLUTIONCALCMODEL_H__
#define __SYMBOID_ASTRO_HORA_QREVOLUTIONCALCMODEL_H__

#include "astro/hora/defs.h"
#include <QObject>
#include "astro/hora/qhoracoords.h"
#include <QSharedPointer>
#include "astro/hora/qplanet.h"

class QRevolutionCalcModel : public QObject
{
public:
    QRevolutionCalcModel(QOrbisConfigNodeGetter planetConfig, const QEclLont& targetLont);
private:
    QPlanet mPlanet;
    const QEclLont mTargetLont;
    static constexpr double APPROX_DIST = 1.0 / 3600.0;

public:
    QSharedPointer<QHoraCoords> approx(const QHoraCoords* startCoords);
private:
    QSharedPointer<QHoraCoords> approxLinear(const QHoraCoords* startCoords);
};

#endif // __SYMBOID_ASTRO_HORA_QREVOLUTIONCALCMODEL_H__
