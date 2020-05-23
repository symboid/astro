
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
    eph::arc_coord::signum signum = mSectionCalc->signum(mSection, mDegree);
    return eph::arc_coord::calc_arc_pos(signum, rawDegree, mMinute, mSecond + mFracSecond);
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
        if (mSectionCalc->index(arcCoord._M_signum, arcCoord._M_degree) != mSection)
        {
            mSection = mSectionCalc->index(arcCoord._M_signum, arcCoord._M_degree);
            emit sectionChanged();
        }
        if (mSectionCalc->degree(arcCoord._M_signum, arcCoord._M_degree) != mDegree)
        {
            mDegree = mSectionCalc->degree(arcCoord._M_signum, arcCoord._M_degree);
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

eph::arc_coord::signum QNoneSectionCalc::signum(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    Q_UNUSED(sectionIndex);
    Q_UNUSED(sectionDegree);
    return 1;
}

eph::arc_coord::degree QNoneSectionCalc::degree(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const
{
    Q_UNUSED(signum);
    return rawDegree;
}

int QNoneSectionCalc::index(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const
{
    Q_UNUSED(signum);
    Q_UNUSED(rawDegree);
    return 0;
}

eph::arc_coord::degree QSignumSectionCalc::raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    return sectionIndex == negative_index ? -sectionDegree : sectionDegree;
}

eph::arc_coord::signum QSignumSectionCalc::signum(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    Q_UNUSED(sectionDegree);
    return sectionIndex == 1 ? -1 : 1;
}

eph::arc_coord::degree QSignumSectionCalc::degree(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const
{
    rawDegree *= signum;
    while (rawDegree >= 180)
    {
        rawDegree -= 360;
    }
    while (rawDegree < -180)
    {
        rawDegree += 360;
    }
    return rawDegree < 0 ? -rawDegree : rawDegree;
}

int QSignumSectionCalc::index(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const
{
    Q_UNUSED(rawDegree);
    return signum == -1 ? negative_index : positive_index;
}

eph::arc_coord::degree QGeoLattSectionCalc::raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    return (sectionIndex == north_index) ? sectionDegree : -sectionDegree;
}

eph::arc_coord::signum QGeoLattSectionCalc::signum(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    Q_UNUSED(sectionDegree);
    return sectionIndex == 1 ? -1 : 1;
}

eph::arc_coord::degree QGeoLattSectionCalc::degree(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const
{
    rawDegree *= signum;
    if (rawDegree < -sectionMax() || rawDegree > sectionMax())
    {
        return sectionMax();
    }
    else
    {
        return qAbs(rawDegree);
    }
}

int QGeoLattSectionCalc::index(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const
{
    Q_UNUSED(rawDegree);
    return signum != -1 ? north_index : south_index;
}

eph::arc_coord::degree QGeoLontSectionCalc::raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    return (sectionIndex == east_index) ? sectionDegree : -sectionDegree;
}

eph::arc_coord::signum QGeoLontSectionCalc::signum(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    Q_UNUSED(sectionDegree);
    return sectionIndex == 1 ? -1 : 1;
}

eph::arc_coord::degree QGeoLontSectionCalc::normalizeDegree(eph::arc_coord::degree rawDegree)
{
    while (rawDegree >= 180) rawDegree -= 360;
    while (rawDegree < -180) rawDegree += 360;
    return rawDegree;
}

eph::arc_coord::degree QGeoLontSectionCalc::degree(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const
{
    return qAbs(normalizeDegree(signum * rawDegree));
}

int QGeoLontSectionCalc::index(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const
{
    Q_UNUSED(rawDegree);
    return signum != -1 ? east_index : west_index;
}

eph::arc_coord::degree QZodiacSectionCalc::raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    return sectionIndex * 30 + sectionDegree;
}

eph::arc_coord::signum QZodiacSectionCalc::signum(int sectionIndex, eph::arc_coord::degree sectionDegree) const
{
    Q_UNUSED(sectionIndex);
    Q_UNUSED(sectionDegree);
    return 1;
}

eph::arc_coord::degree QZodiacSectionCalc::degree(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const
{
    rawDegree *= signum;
    while (rawDegree < 0.0)
    {
        rawDegree += 360;
    }
    return rawDegree % 30;
}

int QZodiacSectionCalc::index(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const
{
    rawDegree *= signum;
    while (rawDegree < 0.0)
    {
        rawDegree += 360;
    }
    return rawDegree / 30;
}
