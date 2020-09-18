
#ifndef __SYMBOID_ASTRO_HORA_MAGITEM_H__
#define __SYMBOID_ASTRO_HORA_MAGITEM_H__

#include "astro/hora/defs.h"
#include "astro/calculo/hora.h"
#include <QString>
#include "astro/controls/qastrofont.h"

hor_ns_begin
typedef basic_hora<eph_proxy> hora;
hor_ns_end

eph_ns_begin
typedef basic_time_point<eph_proxy> time_point;
eph_ns_end

class MagItem
{
public:
    MagItem(eph::ecl_pos basicEclPos, int radixHouse,
            hor::aspect_type aspectType = hor::aspect_type::conjunction, bool aspectIsUpper = false);

/// Name of item.
public:
    virtual QString name() const = 0;
    QString debugStr() const;
    virtual QString symbolStr(QAstroFont* symbolFont, bool isSgtor = true) const = 0;
    typedef int Index;
    virtual Index index() const = 0;

/// Type of item.
public:
    enum Type
    {
        PLANET,
        AXIS,
        HOUSE_CUSP,
        SIGN_BORDER,
    };
    virtual Type type() const = 0;

/// Type of aspect.
public:
    virtual hor::aspect_type aspectType() const;

/// Ecliptic position.
public:
    virtual eph::ecl_pos calcEclPos(eph::time_point julDay) = 0;
    eph::ecl_pos eclPos() const;
    virtual void setEclPos(const eph::ecl_pos& eclPos);
    virtual bool lt(const MagItem* rhs) const;

/// Orbis calculation.
public:
    virtual hor::orbis orbis() const;
    virtual hor::orbis orbis(hor::aspect_type aspectType) const;
    virtual hor::orbis connectingOrbis() const;
    virtual hor::orbis connectingOrbis(hor::aspect_type aspectType) const;
//    virtual hor::orbis direxOrbis() const;
private:
    virtual hor::orbis resolveOrbis(hor::aspect_type aspectType) const = 0;

/// Copying.
public:
    virtual MagItem* clone() const = 0;

public:
    virtual bool isRetrograd() const = 0;

/// Radix attributes.
    virtual eph::zod radixSign() const;
    virtual int radixHouse() const;
    virtual QString radixHouseName() const;

protected:
    hor::aspect_type mAspectType;
    bool mAspectIsUpper;
    eph::ecl_pos mBasicEclPos;
    int mRadixHouse;


};

class MagItems : public std::list<MagItem*>
{
public:
    struct Siblings
    {
        Siblings(MagItem* prec, MagItem* succ) : mPrec(prec), mSucc(succ) {}
        MagItem* mPrec;
        MagItem* mSucc;
    };
    inline Siblings findSiblings(const MagItem* item)
    {
        iterator precIt = end();
        iterator succIt = end();
        iterator it = begin();
        while (it != end() && (*it)->lt(item))
        {
            precIt = it;
            ++it;
        }
        if (precIt != end() && ++(precIt) != end())
        {
            succIt = ++(precIt);
        }
        // no preceeding item means:
        // this ecliptic position is lesser than every single item's position in list
        else
        {
            // this means: the preceeding item is the last one:
            // and the succeeding item is the first one:
            succIt = begin();
            precIt = --end();
        }
        return Siblings(*precIt, *succIt);
    }
    void insert(MagItem* item)
    {

    }
    void deleteElements()
    {

    }
};

class MagItemFactory
{
public:
    MagItemFactory(const hor::hora* hora);

public:
    MagItem* createPlanetItem(int planetIndex,
            hor::aspect_type aspectType = hor::aspect_type::conjunction, bool aspectIsUpper = false);
    MagItem* createHouseCuspItem(int houseNum);
    MagItem* createSignBorderItem(eph::zod zodSign);
private:
    const hor::hora* mHora;
};

#endif // __SYMBOID_ASTRO_HORA_MAGITEM_H__
