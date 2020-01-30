
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/qhorastellium.h"

QHoraStellium::QHoraStellium(const eph::arc_degree& equDist, const hor::planet& planet)
    : mEquDist(equDist)
{
    push_back(planet);
}

eph::ecl_pos QHoraStellium::realBegin() const
{
    return front().pos();
}

eph::ecl_pos QHoraStellium::realEnd() const
{
    return back().pos();
}

eph::ecl_lont QHoraStellium::realWidth() const
{
    return realBegin().dist_fwd(realEnd());
}

eph::ecl_pos QHoraStellium::displayBegin() const
{
    return realBegin()._M_lont - padding();
}

eph::ecl_pos QHoraStellium::displayEnd() const
{
    return realEnd()._M_lont + padding();
}

eph::ecl_pos QHoraStellium::displayPos(int planetIndex) const
{
    return 0 <= planetIndex && planetIndex < size() ? displayBegin()._M_lont + planetIndex * mEquDist : eph::ecl_lont(0);
}

eph::ecl_lont QHoraStellium::displayWidth() const
{
    return (size() - 1) * mEquDist;
}

eph::arc_degree QHoraStellium::padding() const
{
    return (displayWidth() - realWidth()) / 2.0;
}

eph::ecl_lont QHoraStellium::leftDist(const QHoraStellium& rhs) const
{
    return displayBegin().dist_bwd(rhs.displayEnd());
}

eph::ecl_lont QHoraStellium::rightDist(const QHoraStellium& rhs) const
{
    return displayEnd().dist_fwd(rhs.displayBegin());
}

bool QHoraStellium::isMergeable(const QHoraStellium& rhs) const
{
    bool mergeable = mEquDist == rhs.mEquDist;
    if (mergeable)
    {
        //
        //  this: [------]
        //  rhs:      [-------]
        //
        //  this.begin <= rhs.end && rhs.begin <= this.end

        if (realBegin() <= rhs.realEnd() || rhs.realBegin() <= realEnd())
        {
            eph::ecl_pos mergedBegin = realBegin() <= rhs.realBegin() ? realBegin() : rhs.realBegin();
            eph::ecl_pos mergedEnd = realEnd() <= rhs.realEnd() ? rhs.realEnd() : realEnd();
            eph::arc_degree realMergedSize = mergedBegin.dist_fwd(mergedEnd);
            eph::arc_degree imagMergedSize = (size() + rhs.size() - 1) * mEquDist;
            mergeable = realMergedSize < imagMergedSize;
        }
        else
        {
            mergeable = false;
        }
    }
    return mergeable;
}

QHoraStellium& QHoraStellium::mergeIn(const QHoraStellium& rhs)
{
    QList<hor::planet>::iterator thisIt = begin();
    QList<hor::planet>::const_iterator rhsIt = rhs.begin(), rhsEnd = rhs.end();
    while (rhsIt != rhsEnd)
    {
        // rhsIt < thisIt
        if (thisIt == end())
        {
            thisIt = insert(thisIt, *rhsIt);
            ++thisIt;
            ++rhsIt;
        }
        else if (rhsIt->pos().dist_to(thisIt->pos()) >= 0)
        {
            thisIt = insert(thisIt, *rhsIt);
            ++thisIt;
            ++rhsIt;
        }
        else
        {
            ++thisIt;
        }
    }
    return *this;
}
