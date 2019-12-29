
#include "astro/calculo/setup.h"
#include "astro/calculo/aspect.h"

namespace As {

Aspect::Aspect(const ElePoint* elePoint)
    : MagPoint(elePoint ? elePoint->mOrbis : 0.0)
    , mElePoint(elePoint)
    , mType(CONJUNCTION)
{
}

Aspect::Aspect(const ElePoint* elePoint, Type type, Orbis orbis)
    : MagPoint(orbis)
    , mElePoint(elePoint)
    , mType(type)
{
}

Sy::String Aspect::name() const
{
    Sy::String aspectName;
    if (mElePoint)
    {
        Sy::String nameSuffix;
        if (mType == CONJUNCTION)
        {
            aspectName = mElePoint->name();
        }
        else switch (mType)
        {
        case OPPOSITION:    nameSuffix = " opposition";break;
        case TRIGON:        nameSuffix = " trigon";break;
        case QUADRAT:       nameSuffix = " quadrat";break;
        default:            nameSuffix = " ?aspect?";
        }
        aspectName = mElePoint->name() + nameSuffix;
    }
    return aspectName;
}

EclPos Aspect::eclPos() const
{
    EclPos aspectPos(0.0);
    if (mElePoint != nullptr)
    {
        aspectPos = mElePoint->eclPos();
        aspectPos.mLont += EclCoords::Lont(mType);
    }
    return aspectPos;
}

EclSpeed Aspect::eclSpeed() const
{
    return mElePoint != nullptr ? mElePoint->eclSpeed() : EclSpeed(0.0,0.0);
}

AspectConnection::AspectConnection(const ElePoint* elePoint, const MagPoint* magPoint)
    : mElePoint(elePoint)
    , mMagPoint(magPoint)
{
}

Aspect::Type AspectConnection::aspectType() const
{
    Aspect::Type aspectType = Aspect::NONE;

    EclCoords::Lont dist = mElePoint->eclPos().distAbs(mMagPoint->eclPos());
    MagPoint::Orbis orbis = mElePoint->mOrbis + mMagPoint->mOrbis;
    EclCoords::Lont distMin = dist - orbis, distMax = dist + orbis;

    if (distMin <= Aspect::CONJUNCTION && Aspect::CONJUNCTION <= distMax)
    {
        aspectType = Aspect::CONJUNCTION;
    }
    else if (distMin <= Aspect::OPPOSITION && Aspect::OPPOSITION <= distMax)
    {
        aspectType = Aspect::OPPOSITION;
    }
    else if (distMin <= Aspect::TRIGON && Aspect::TRIGON <= distMax)
    {
        aspectType = Aspect::TRIGON;
    }
    else if (distMin <= Aspect::QUADRAT && Aspect::QUADRAT <= distMax)
    {
        aspectType = Aspect::QUADRAT;
    }
    else if (distMin <= Aspect::SEXTIL && Aspect::SEXTIL <= distMax)
    {
        aspectType = Aspect::SEXTIL;
    }
    return aspectType;
}

AspectConnection::Type AspectConnection::type() const
{
    if (!mElePoint || !mMagPoint)
    {
        return NONE;
    }
    else if (mElePoint->eclPos().distFwd(mMagPoint->eclPos()) < 180.0)
    {
        return mElePoint->eclSpeed().mLont > mMagPoint->eclSpeed().mLont ? APPLICATING : SEPARATING;
    }
    else
    {
        return mElePoint->eclSpeed().mLont < mMagPoint->eclSpeed().mLont ? APPLICATING : SEPARATING;
    }
}

} // namespace As
