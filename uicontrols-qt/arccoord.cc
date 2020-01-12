
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
    return eph::arc_coord::calc_arc_pos(rawDegree, mMinute, mSecond + mFracSecond);
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

void QArcCoord::setDegree(Degree rawDegree)
{
    if (mSectionCalc->raw(mSection, mDegree) != rawDegree)
    {
        if (mSection != mSectionCalc->index(rawDegree))
        {
            mSection = mSectionCalc->index(rawDegree);
            emit sectionChanged();
        }
        if (mDegree != mSectionCalc->degree(rawDegree))
        {
            mDegree = mSectionCalc->degree(rawDegree);
            emit degreeChanged();
        }
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
        if (mSectionCalc->index(arcCoord._M_degree) != mSection)
        {
            mSection = mSectionCalc->index(arcCoord._M_degree);
            emit sectionChanged();
        }
        if (mSectionCalc->degree(arcCoord._M_degree) != mDegree)
        {
            mDegree = mSectionCalc->degree(arcCoord._M_degree);
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
