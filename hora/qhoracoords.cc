
#include "astro/hora/setup.h"
#include "astro/hora/qhoracoords.h"

QHoraCoords::QHoraCoords(QObject *parent)
    : QCalcObject(parent)
    , mCalendarCoords({1,1,1,0,0,0,eph::calendar_type::GREGORIAN})
    , mTzDiff(0)
    , mGeoLont(0.0)
    , mGeoLatt(0.0)
{
    connect(this, SIGNAL(yearChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(monthChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(dayChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(hourChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(minuteChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(secondChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(tzDiffChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(geoLontChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(geoLattChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(withJulianCalendarChanged()), this, SIGNAL(changed()));
    connect(this, SIGNAL(changed()), this, SIGNAL(dateTimeChanged()));
}

QHoraCoords::QHoraCoords(const QEphTime& ephTime)
    : QHoraCoords(nullptr)
{
    setEphTime(ephTime);
}

QHoraCoords::QHoraCoords(const QDateTime& dateTime, double tzDiffHours)
    : QHoraCoords(nullptr)
{
    setDateTime(dateTime);
    setCalendarIsJulian(false);
    setTzDiff(tzDiffHours);
}

QHoraCoords& QHoraCoords::operator=(const QHoraCoords& src)
{
    setYear(src.year());
    setMonth(src.month());
    setDay(src.day());

    setHour(src.hour());
    setMinute(src.minute());
    setSecond(src.second());

    setTzDiff(src.tzDiff());
    setGeoLont(src.geoLont());
    setGeoLatt(src.geoLatt());

    setCalendarIsJulian(src.calendarIsJulian());

    return *this;
}

int QHoraCoords::year() const
{
    return mCalendarCoords._M_year;
}

int QHoraCoords::month() const
{
    return mCalendarCoords._M_month;
}

int QHoraCoords::day() const
{
    return mCalendarCoords._M_day;
}

int QHoraCoords::hour() const
{
    return mCalendarCoords._M_hour;
}

int QHoraCoords::minute() const
{
    return mCalendarCoords._M_minute;
}

int QHoraCoords::second() const
{
    return mCalendarCoords._M_second;
}

qreal QHoraCoords::tzDiff() const
{
    return qreal(mTzDiff.count()) / 60.0;
}

qreal QHoraCoords::geoLont() const
{
    return mGeoLont;
}

qreal QHoraCoords::geoLatt() const
{
    return mGeoLatt;
}

bool QHoraCoords::calendarIsJulian() const
{
    return  mCalendarCoords._M_calendar_type == eph::calendar_type::JULIAN;
}

void QHoraCoords::setYear(int year)
{
    if (mCalendarCoords._M_year != year)
    {
        mCalendarCoords._M_year = year;
        emit yearChanged();
    }
}

void QHoraCoords::setMonth(int month)
{
    if (mCalendarCoords._M_month != month)
    {
        mCalendarCoords._M_month = month;
        emit monthChanged();
    }
}

void QHoraCoords::setDay(int day)
{
    if (mCalendarCoords._M_day != day)
    {
        mCalendarCoords._M_day = day;
        emit dayChanged();
    }
}

void QHoraCoords::setHour(int hour)
{
    if (mCalendarCoords._M_hour != hour)
    {
        mCalendarCoords._M_hour = hour;
        emit hourChanged();
    }
}

void QHoraCoords::setMinute(int minute)
{
    if (mCalendarCoords._M_minute != minute)
    {
        mCalendarCoords._M_minute = minute;
        emit minuteChanged();
    }
}

void QHoraCoords::setSecond(int second)
{
    if (mCalendarCoords._M_second != second)
    {
        mCalendarCoords._M_second = second;
        emit secondChanged();
    }
}

void QHoraCoords::setGeoLont(qreal geoLont)
{
    if (!eph::arc_degree_equals(mGeoLont,geoLont))
    {
        mGeoLont = geoLont;
        emit geoLontChanged();
    }
}

void QHoraCoords::setGeoLatt(qreal geoLatt)
{
    if (!eph::arc_degree_equals(mGeoLatt,geoLatt))
    {
        mGeoLatt = geoLatt;
        emit geoLattChanged();
    }
}

void QHoraCoords::setTzDiff(qreal tzDiffHours)
{
    std::chrono::minutes tzDiff = std::chrono::minutes(int(60.0 * tzDiffHours));
    if (mTzDiff != tzDiff)
    {
        mTzDiff = tzDiff;
        emit tzDiffChanged();
    }
}

void QHoraCoords::setCalendarIsJulian(bool isJulian)
{
    if (isJulian != calendarIsJulian())
    {
        mCalendarCoords._M_calendar_type = isJulian ? eph::calendar_type::JULIAN : eph::calendar_type::GREGORIAN;
        emit withJulianCalendarChanged();
    }
}

QEphTime QHoraCoords::ephTime() const
{
    return eph::basic_calendar<eph_proxy>::time(mCalendarCoords) - mTzDiff;
}

void QHoraCoords::setEphTime(const QEphTime& ephTime)
{
    mCalendarCoords = eph::basic_calendar<eph_proxy>::coords(ephTime + mTzDiff);
}

QDateTime QHoraCoords::dateTime() const
{
   QDate date(mCalendarCoords._M_year, mCalendarCoords._M_month, mCalendarCoords._M_day);
   QTime time(mCalendarCoords._M_hour, mCalendarCoords._M_minute, mCalendarCoords._M_second);
   return QDateTime(date, time);
}

void QHoraCoords::setDateTime(const QDateTime &dateTime)
{
    const QDate date = dateTime.date();
    const QTime time = dateTime.time();
    setYear(date.year());
    setMonth(date.month());
    setDay(date.day());
    setHour(time.hour());
    setMinute(time.minute());
    setSecond(time.second());

}
