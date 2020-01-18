
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/arccoord.h"

QArcCoord::QArcCoord(QObject* parent)
    : QObject(parent)
    , mSectionCalc(new QNoneSectionCalc)
    , mSection(0)
    , mDegree(0)
    , mMinute(0)
    , mSecond(0)
    , mFracSecond(0.0)
{
}

QSectionCalc* QArcCoord::sectionCalc() const
{
    return mSectionCalc.get();
}

int QArcCoord::section() const
{
    return mSection;
}

QArcCoord::Degree QArcCoord::degree() const
{
    return mDegree;
}

QArcCoord::Minute QArcCoord::minute() const
{
    return mMinute;
}

QArcCoord::Second QArcCoord::second() const
{
    return mSecond;
}

QArcCoord::FracSecond QArcCoord::fracSecond() const
{
    return mFracSecond;
}

QArcCoord::ArcDegree QArcCoord::arcDegree() const
{
    Degree rawDegree = mSectionCalc->raw(mSection, mDegree);
    return eph::arc_coord::calc_arc_pos(rawDegree < 0.0 ? -1 : 1, rawDegree, mMinute, mSecond + mFracSecond);
}

void QArcCoord::setSectionCalc(QSectionCalc* sectionCalc)
{
    if (sectionCalc)
    {
        ArcDegree rawArcDegree = arcDegree();
        mSectionCalc.reset(sectionCalc);
        emit sectionCalcChanged();
        setArcDegree(rawArcDegree);
    }
}

void QArcCoord::setSection(int section)
{
    if (mSection != section)
    {
        mSection = section;
        emit sectionChanged();
        emit arcDegreeChanged();
    }
}

void QArcCoord::setDegree(Degree degree)
{
    if (mDegree != degree)
    {
        mDegree = degree;
        emit degreeChanged();
        emit arcDegreeChanged();
    }
}

void QArcCoord::setMinute(Minute minute)
{
    while (minute >= 60)
    {
        minute -= 60;
    }
    if (mMinute != minute)
    {
        mMinute = minute;
        emit minuteChanged();
        emit arcDegreeChanged();
    }
}

void QArcCoord::setSecond(Second second)
{
    while (second >= 60)
    {
        second -= 60;
    }
    if (mSecond != second)
    {
        mSecond = second;
        emit secondChanged();
        emit arcDegreeChanged();
    }
}

void QArcCoord::setArcDegree(ArcDegree arcDegree)
{
    if (!eph::arc_degree_equals(QArcCoord::arcDegree(),arcDegree))
    {
        eph::arc_coord arcCoord(arcDegree);
        eph::arc_coord::degree signedDegree = arcCoord._M_signum * arcCoord._M_degree;
        if (mSectionCalc->index(signedDegree) != mSection)
        {
            mSection = mSectionCalc->index(signedDegree);
            emit sectionChanged();
        }
        if (mSectionCalc->degree(arcCoord._M_degree) != mDegree)
        {
            mDegree = mSectionCalc->degree(signedDegree);
            emit degreeChanged();
        }
        if (arcCoord._M_minute != mMinute)
        {
            mMinute = arcCoord._M_minute;
            emit minuteChanged();
        }
        int intSecond = int(std::floor(arcCoord._M_second));
        if (intSecond != mSecond)
        {
            mSecond = intSecond;
            emit secondChanged();
        }
        mFracSecond = arcCoord._M_second - intSecond;
        emit arcDegreeChanged();
    }
}

eph::arc_coord::degree QNoneSectionCalc::raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    Q_UNUSED(sectionIndex);
    return sectionDegree;
}

eph::arc_coord::degree QNoneSectionCalc::degree(eph::arc_coord::degree plainDegree) const
{
    return plainDegree;
}

int QNoneSectionCalc::index(eph::arc_coord::degree plainDegree) const
{
    Q_UNUSED(plainDegree);
    return 0;
}

eph::arc_coord::degree QGeoLattSectionCalc::raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    return (sectionIndex == north_index) ? sectionDegree : -sectionDegree;
}

eph::arc_coord::degree QGeoLattSectionCalc::degree(eph::arc_coord::degree rawDegree) const
{
    if (rawDegree < -sectionMax() || rawDegree > sectionMax())
    {
        return sectionMax();
    }
    else
    {
        return qAbs(rawDegree);
    }
}

int QGeoLattSectionCalc::index(eph::arc_coord::degree rawDegree) const
{
    return rawDegree >= 0 ? north_index : south_index;
}

eph::arc_coord::degree QGeoLontSectionCalc::raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    return (sectionIndex == east_index) ? sectionDegree : -sectionDegree;
}

eph::arc_coord::degree QGeoLontSectionCalc::normalizeDegree(eph::arc_coord::degree rawDegree)
{
    while (rawDegree > 180) rawDegree -= 360;
    while (rawDegree <= -180) rawDegree += 360;
    return rawDegree;
}

eph::arc_coord::degree QGeoLontSectionCalc::degree(eph::arc_coord::degree rawDegree) const
{
    return qAbs(normalizeDegree(rawDegree));
}

int QGeoLontSectionCalc::index(eph::arc_coord::degree rawDegree) const
{
    return normalizeDegree(rawDegree) >= 0 ? east_index : west_index;
}

eph::arc_coord::degree QZodiacSectionCalc::raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    return sectionIndex * 30 + sectionDegree;
}

eph::arc_coord::degree QZodiacSectionCalc::degree(eph::arc_coord::degree rawDegree) const
{
    while (rawDegree < 0.0)
    {
        rawDegree += 360;
    }
    return rawDegree % 30;
}

int QZodiacSectionCalc::index(eph::arc_coord::degree rawDegree) const
{
    while (rawDegree < 0.0)
    {
        rawDegree += 360;
    }
    return rawDegree / 30;
}
