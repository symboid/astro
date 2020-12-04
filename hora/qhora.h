
#ifndef __SYMBOID_ASTRO_HORA_QHORA_H__
#define __SYMBOID_ASTRO_HORA_QHORA_H__

#include "astro/hora/defs.h"
#include <QObject>
#include "astro/hora/qhoraconfig.h"
#include <QList>
#include "astro/hora/qhousecusp.h"
#include "astro/hora/qplanet.h"
#include "astro/db/fixstars.h"

struct QHoraCoords
{
    eph::calendar_coords mCalendarCoords;
    std::chrono::minutes mTimeZoneDiff;
    eph::arc_degree mGeoLont;
    eph::arc_degree mGeoLatt;
};

class QHora : public QObject
{
    Q_OBJECT

public:
    QHora(QObject* parent);

private:
    arh::main_object<QHoraConfig> mHoraConfig;

private:
    QHouseSystem mHouseSystem;
    QHouseCusp* mHouseCusps[QHouseSystem::HOUSE_COUNT + 1];
public:
    QHouseCusp*const* housesBegin() const;
    QHouseCusp*const* housesEnd() const;
    const QHouseCusp* house(int index) const;

public:
    typedef QVector<QPlanet*> Planets;
private:
    Planets mPlanets;
public:
    Planets::ConstIterator planetsBegin() const;
    Planets::ConstIterator planetsEnd() const;
    int planetCount() const;
    const QPlanet* planet(int index) const;
private slots:
    void updatePlanets();
signals:
    void planetsUpdated();

public:
    typedef QList<eph::basic_fixstar<eph_proxy>> ConjunctingFixstars;
private:
    ConjunctingFixstars mConjunctingFixstars;
    arh::main_object<Fixstars> mAllFixstars;
public:
    ConjunctingFixstars::ConstIterator fixstarsBegin() const;
    ConjunctingFixstars::ConstIterator fixstarsEnd() const;

public:
    bool calc(const QHoraCoords& horaCoords, QHouseSystem::Type houseSystemType);
};

#endif // __SYMBOID_ASTRO_HORA_QHORA_H__
