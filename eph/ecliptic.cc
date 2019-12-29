
#include "astro/ephe/setup.h"
#include "astro/ephe/ecliptic.h"

namespace As {

EclCoords::Lont::Lont(Sy::ArcDegree lontValue, bool isSigned)
    : mValue360(lontValue)
    , mIsSigned(isSigned)
{
    Sy::ArcDegree lowerBound = (mIsSigned ? -180.0 : 0.0), upperBound = lowerBound + 360.0;
    while (mValue360 >= upperBound)
    {
        mValue360 -= 360.0;
    }
    while (mValue360 < lowerBound)
    {
        mValue360 += 360.0;
    }
}

EclCoords::Lont::operator Sy::ArcDegree() const
{
    return mValue360;
}

EclCoords::Lont EclCoords::Lont::operator+(const Sy::ArcDegree& degree)
{
    return EclCoords::Lont(mValue360 + degree, mIsSigned);
}

EclCoords::Lont EclCoords::Lont::operator-(const Sy::ArcDegree& degree)
{
    return EclCoords::Lont(mValue360 - degree, mIsSigned);
}

EclCoords::Lont& EclCoords::Lont::operator+=(const Sy::ArcDegree& degree)
{
    *this = *this + degree;
    return *this;
}

EclCoords::Lont& EclCoords::Lont::operator-=(const Sy::ArcDegree& degree)
{
    *this = *this - degree;
    return *this;
}

EclCoords::EclCoords()
    : mLont(0.0)
    , mLatt(0.0)
    , mDist(0.0)
{
}

EclCoords::EclCoords(Lont lont, Latt latt, Dist dist)
    : mLont(lont)
    , mLatt(latt)
    , mDist(dist)
{
}

ZodCoords::ZodCoords(Sy::ArcDegree arcPos)
    : ArcCoord(arcPos)
    , mSign(ZodSign((mDegree / SIGN_SIZE) + 1))
{
    mDegree = mDegree % SIGN_SIZE;
}

EclPos::EclPos(Lont lont, Latt latt, Dist dist)
    : EclCoords(lont, latt, dist)
{
}

EclPos::Lont EclPos::distTo(const EclPos& rhs) const
{
    return Lont(static_cast<Sy::ArcDegree>(mLont - rhs.mLont), true);
}

EclPos::Lont EclPos::distAbs(const EclPos& rhs) const
{
    Sy::ArcDegree lhsLont = mLont, rhsLont = rhs.mLont;
    Sy::ArcDegree dist = (lhsLont < rhsLont) ? rhsLont - lhsLont : lhsLont - rhsLont;
    if (dist > 180.0)
    {
        dist = 360.0 - dist;
    }
    return dist;
}

EclPos::Lont EclPos::distFwd(const EclPos& rhs) const
{
    return Lont(static_cast<Sy::ArcDegree>(rhs.mLont - mLont), false);
}

EclPos::Lont EclPos::distBwd(const EclPos& rhs) const
{
    return Lont(static_cast<Sy::ArcDegree>(mLont - rhs.mLont), false);
}

} // namespace As
