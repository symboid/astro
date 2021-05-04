
#ifndef __SYMBOID_ASTRO_HORA_QHORACOORDS_H__
#define __SYMBOID_ASTRO_HORA_QHORACOORDS_H__

#include "astro/hora/defs.h"
#include <QObject>
#include "astro/eph/ecliptic.h"
#include "astro/eph/calendar.h"
#include "astro/eph/proxy.h"
#include <QDateTime>

typedef eph::basic_time_point<eph_proxy> QEphTime;

class QHoraCoords : public QObject
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "HoraCoords";

public:
    explicit QHoraCoords(QObject *parent = nullptr);
    explicit QHoraCoords(const QDateTime& dateTime, double tzDiffHours);
    QHoraCoords& operator=(const QHoraCoords& src);

public:
    Q_PROPERTY(int year READ year WRITE setYear NOTIFY yearChanged)
    Q_PROPERTY(int month READ month WRITE setMonth NOTIFY monthChanged)
    Q_PROPERTY(int day READ day WRITE setDay NOTIFY dayChanged)
    Q_PROPERTY(int hour READ hour WRITE setHour NOTIFY hourChanged)
    Q_PROPERTY(int minute READ minute WRITE setMinute NOTIFY minuteChanged)
    Q_PROPERTY(int second READ second WRITE setSecond NOTIFY secondChanged)
    Q_PROPERTY(qreal tzDiff READ tzDiff WRITE setTzDiff NOTIFY tzDiffChanged)
    Q_PROPERTY(qreal geoLont MEMBER mGeoLont WRITE setGeoLont NOTIFY geoLontChanged)
    Q_PROPERTY(qreal geoLatt MEMBER mGeoLatt WRITE setGeoLatt NOTIFY geoLattChanged)
public:
    int year() const;
    int month() const;
    int day() const;
    int hour() const;
    int minute() const;
    int second() const;
    qreal tzDiff() const;
    qreal geoLont() const;
    qreal geoLatt() const;
    void setYear(int year);
    void setMonth(int month);
    void setDay(int day);
    void setHour(int hour);
    void setMinute(int minute);
    void setSecond(int second);
    void setTzDiff(qreal tzDiff);
    void setGeoLont(qreal geoLont);
    void setGeoLatt(qreal geoLatt);
private:
    eph::calendar_coords mCalendarCoords;
    std::chrono::minutes mTzDiff;
    eph::arc_degree mGeoLont;
    eph::arc_degree mGeoLatt;
signals:
    void yearChanged();
    void monthChanged();
    void dayChanged();
    void hourChanged();
    void minuteChanged();
    void secondChanged();
    void tzDiffChanged();
    void geoLontChanged();
    void geoLattChanged();
    void changed();

public:
    QEphTime ephTime() const;
    QDateTime dateTime() const;
    void setDateTime(const QDateTime& dateTime);
};

#endif // __SYMBOID_ASTRO_HORA_QHORACOORDS_H__
