
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/arccoord.h"

QArcCoord::QArcCoord(QObject* parent)
    : QObject(parent)
    , mDegree(0)
    , mMinute(0)
    , mSecond(0)
    , mFracSecond(0.0)
{
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
    return eph::arc_coord::calc_arc_pos(mDegree, mMinute, mSecond + mFracSecond);
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
        if (arcCoord._M_degree != mDegree)
        {
            mDegree = arcCoord._M_degree;
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
