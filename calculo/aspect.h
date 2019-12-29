
#ifndef __SYMBOID_ASTRO_CALCULO_ASPECT_H__
#define __SYMBOID_ASTRO_CALCULO_ASPECT_H__

#include "astro/calculo/defs.h"
#include "astro/calculo/magpoint.h"

namespace As {

class Aspect : public MagPoint
{
public:
    enum Type
    {
        NONE = ~0,
        CONJUNCTION = 0,
        OPPOSITION = 180,
        TRIGON = 120,
        QUADRAT = 90,
        SEXTIL = 60,
    };
public:
    Aspect(const ElePoint* elePoint);
    Aspect(const ElePoint* elePoint, Type type, Orbis orbis);

private:
    const ElePoint* mElePoint;
    Type mType;

public:
    Sy::String name() const override;
    EclPos eclPos() const override;
    EclSpeed eclSpeed() const override;
};

struct AspectConnection
{
    AspectConnection(const ElePoint* elePoint, const MagPoint* magPoint);
    const ElePoint* mElePoint;
    const MagPoint* mMagPoint;
    Aspect::Type aspectType() const;
    enum Type
    {
        NONE,
        APPLICATING,
        SEPARATING,
    };
    Type type() const;
};

} // namespace As

#endif // __SYMBOID_ASTRO_CALCULO_ASPECT_H__
