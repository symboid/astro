
#include "astro/calculo/setup.h"
#include "astro/calculo/planet.h"

namespace As {

Planet::Planet(ObjectId objectId, Orbis orbis)
    : ElePoint(orbis)
    , mObject(objectId)
{
}

ObjectId Planet::objectId() const
{
    return mObject.mId;
}

CalcResult Planet::calc(const Time& time)
{
    return mObject.calcPos(time);
}

Sy::String Planet::name() const
{
    return mObject.name();
}

EclPos Planet::eclPos() const
{
    return mObject.mEclPos;
}

EclSpeed Planet::eclSpeed() const
{
    return mObject.mEclSpeed;
}

Sy::ScopePtr<Aspect> Planet::aspect(Aspect::Type aspectType) const
{
    return new Aspect(this, aspectType, 1.0);
}

} // namespace As
