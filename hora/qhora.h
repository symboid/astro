
#ifndef __SYMBOID_ASTRO_HORA_QHORA_H__
#define __SYMBOID_ASTRO_HORA_QHORA_H__

#include "astro/hora/defs.h"
#include <QObject>
#include "astro/hora/qhoracoords.h"
#include "astro/hora/qhoraconfig.h"
#include <QList>
#include "astro/hora/qhousecusp.h"
#include "astro/hora/qplanet.h"
#include "astro/hora/qaspectobject.h"
#include "astro/hora/qmagitem.h"
#include "astro/db/fixstars.h"
#include "sdk/controls/qcalctask.h"

class QHora : public QCalcable
{
    Q_OBJECT

public:
    QHora(QObject* parent);

private:
    arh::main_object<QHoraConfig> mHoraConfig;

private:
    QHouseSystem* mHouseSystem;
    QHouseCusp* mHouseCusps[QHouseSystem::HOUSE_COUNT + 1];
public:
    QHouseCusp*const* housesBegin() const;
    QHouseCusp*const* housesEnd() const;
    const QHouseCusp* house(int index) const;

public:
    typedef QVector<QPlanet*> Planets;
private:
    Planets mPlanets;
    QList<QAspectObject*> mAspectObjects;
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
    QSharedPointer<QMagItemList> fetchAspectItems(const QVector<qreal>& dists) const;
    const QSharedPointer<QMagItemList> regularAspectItems() const;
private:
    QSharedPointer<QMagItemList> mRegularAspectItems;

public:
    QHoraCoords* coords() const;
    void setCoords(QHoraCoords* coords);
    QHouseSystem::Type houseSystemType() const;
    void setHouseSystemType(const QHouseSystem::Type& houseSystemType);
    void calc() override;
private:
    QHoraCoords* mCoords;
signals:
    void coordsChanged();
    void houseSystemTypeChanged();

private slots:
    void onCalcTaskChanged();
};

#endif // __SYMBOID_ASTRO_HORA_QHORA_H__
