
#ifndef __SYMBOID_ASTRO_UICONTROLS_QT_QHORASTELLIUM_H__
#define __SYMBOID_ASTRO_UICONTROLS_QT_QHORASTELLIUM_H__

#include "astro/uicontrols-qt/defs.h"
#include <QList>
#include "astro/calculo/planet.h"

template <class Object>
struct Stellium : QList<Object>
{

    Stellium(const eph::arc_degree& equDist, const Object& object)
        : mEquDist(equDist)
    {
        this->push_back(object);
    }

    eph::ecl_pos realBegin() const
    {
        return this->front().pos();
    }

    eph::ecl_pos realEnd() const
    {
        return this->back().pos();
    }

    eph::ecl_lont realWidth() const
    {
        return realBegin().dist_fwd(realEnd());
    }

    eph::ecl_pos displayBegin() const
    {
        return realBegin()._M_lont - padding();
    }

    eph::ecl_pos displayEnd() const
    {
        return realEnd()._M_lont + padding();
    }

    eph::ecl_pos displayPos(int planetIndex) const
    {
        return 0 <= planetIndex && planetIndex < this->size() ? displayBegin()._M_lont + planetIndex * mEquDist : eph::ecl_lont(0);
    }

    eph::ecl_lont displayWidth() const
    {
        return (this->size() - 1) * mEquDist;
    }

    eph::arc_degree padding() const
    {
        return (displayWidth() - realWidth()) / 2.0;
    }

    eph::ecl_lont leftDist(const Stellium& rhs) const
    {
        return displayBegin().dist_bwd(rhs.displayEnd());
    }

    eph::ecl_lont rightDist(const Stellium& rhs) const
    {
        return displayEnd().dist_fwd(rhs.displayBegin());
    }

    bool isMergeable(const Stellium& rhs) const
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
                eph::arc_degree imagMergedSize = (this->size() + rhs.size() - 1) * mEquDist;
                mergeable = realMergedSize < imagMergedSize;
            }
            else
            {
                mergeable = false;
            }
        }
        return mergeable;
    }

    Stellium& mergeIn(const Stellium& rhs)
    {
        typename QList<Object>::iterator thisIt = this->begin();
        typename QList<Object>::const_iterator rhsIt = rhs.begin(), rhsEnd = rhs.end();
        while (rhsIt != rhsEnd)
        {
            // rhsIt < thisIt
            if (thisIt == this->end())
            {
                thisIt = this->insert(thisIt, *rhsIt);
                ++thisIt;
                ++rhsIt;
            }
            else if (rhsIt->pos().dist_to(thisIt->pos()) >= 0)
            {
                thisIt = this->insert(thisIt, *rhsIt);
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

private:
    eph::arc_degree mEquDist;

public:
    struct List : QList<Stellium>
    {
        List(eph::arc_degree equDist) : mEquDist(equDist) {}
        eph::arc_degree mEquDist;
        void add(const Object& object)
        {
            Stellium newStellium(mEquDist, object);

            typename QList<Stellium>::iterator stellium = this->begin();
            while (stellium != this->end())
            {
                if (newStellium.isMergeable(*stellium))
                {
                    newStellium.mergeIn(*stellium);
                    this->erase(stellium);
                    stellium = this->begin();
                }
                else
                {
                    ++stellium;
                }
            }
            this->push_back(newStellium);

        }
    };
};

typedef Stellium<hor::planet> QHoraStellium;

#endif // __SYMBOID_ASTRO_UICONTROLS_QT_QHORASTELLIUM_H__
