
#ifndef __SYMBOID_ASTRO_CALCULO_MAGPOINT_H__
#define __SYMBOID_ASTRO_CALCULO_MAGPOINT_H__

#include "astro/calculo/defs.h"
#include "sdk/basics/stringutils.h"
#include "astro/ephe/ecliptic.h"

namespace As {

class MagPoint
{
public:
    typedef Sy::ArcDegree Orbis;
protected:
    MagPoint(Orbis orbis);
    virtual ~MagPoint();

public:
    virtual Sy::String name() const = 0;
    virtual EclPos eclPos() const = 0;
    virtual EclSpeed eclSpeed() const = 0;

public:
    Orbis mOrbis;
};

class ElePoint : public MagPoint
{
protected:
    ElePoint(Orbis orbis);
};

} // namespace

#endif // __SYMBOID_ASTRO_CALCULO_MAGPOINT_H__
