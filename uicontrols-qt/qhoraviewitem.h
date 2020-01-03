
#ifndef __SYMBOID_ASTRO_UICONTROLS_QT_QHORAVIEWITEM_H__
#define __SYMBOID_ASTRO_UICONTROLS_QT_QHORAVIEWITEM_H__

#include "astro/uicontrols-qt/defs.h"
#include <QQuickPaintedItem>
#include "astro/eph/ecliptic.h"
#include "astro/calculo/hora.h"
#include "astro/uicontrols-qt/qastrofont.h"

hor_ns_begin
typedef basic_hora<eph_proxy> hora;
typedef basic_planet<eph_proxy> planet;
hor_ns_end

class ASTRO_UICONTROLS_QT_API QHoraViewItem : public QQuickPaintedItem
{
    Q_OBJECT

public:
    static constexpr const char* qml_name = "HoraView";

public:
    QHoraViewItem(QQuickItem* parent = Q_NULLPTR);

protected:
    QBrush planetBrush(hor::planet::index planetIndex, qreal alpha);
    void paint(QPainter* painter) override;
private:
    QPointF horaPoint(eph::ecl_lont horaLont, qreal dist) const;

private:
    QPointF mMandalaCenter;
    qreal mMandalaRadius;
    hor::hora mHora;
private:
    eph::ecl_lont mandalaLeft() const;
    qreal eclipticRadius() const;
private slots:
    void calcMandalaGeometry();

private:
    QSharedPointer<QAstroFont> mAstroFont;

public:
    Q_PROPERTY(int year MEMBER mYear WRITE setYear NOTIFY yearChanged)
    Q_PROPERTY(int month MEMBER mMonth WRITE setMonth NOTIFY monthChanged)
    Q_PROPERTY(int day MEMBER mDay WRITE setDay NOTIFY dayChanged)
    Q_PROPERTY(int hour MEMBER mHour WRITE setHour NOTIFY hourChanged)
    Q_PROPERTY(int minute MEMBER mMinute WRITE setMinute NOTIFY minuteChanged)
    Q_PROPERTY(int second MEMBER mSecond WRITE setSecond NOTIFY secondChanged)
    Q_PROPERTY(qreal geoLont MEMBER mGeoLont WRITE setGeoLont NOTIFY geoLontChanged)
    Q_PROPERTY(qreal geoLatt MEMBER mGeoLatt WRITE setGeoLatt NOTIFY geoLattChanged)
    Q_PROPERTY(qreal tzDiff MEMBER mTzDiff WRITE setTzDiff NOTIFY tzDiffChanged)
    Q_PROPERTY(QString housesType MEMBER mHousesType WRITE setHousesType NOTIFY housesTypeChanged)
    Q_PROPERTY(bool withJulianCalendar MEMBER mCalendarIsJulian WRITE setCalendarIsJulian NOTIFY withJulianCalendarChanged)
public:
    void setYear(int year);
    void setMonth(int month);
    void setDay(int day);
    void setHour(int hour);
    void setMinute(int minute);
    void setSecond(int second);
    void setGeoLont(qreal geoLont);
    void setGeoLatt(qreal geoLatt);
    void setTzDiff(qreal tzDiff);
    void setHousesType(const QString& housesType);
    void setCalendarIsJulian(bool calendarIsJulian);
private:
    int mYear;
    int mMonth;
    int mDay;
    int mHour;
    int mMinute;
    int mSecond;
    eph::arc_degree mGeoLont;
    eph::arc_degree mGeoLatt;
    qreal mTzDiff;
    QString mHousesType;
    bool mCalendarIsJulian;
signals:
    void yearChanged();
    void monthChanged();
    void dayChanged();
    void hourChanged();
    void minuteChanged();
    void secondChanged();
    void geoLontChanged();
    void geoLattChanged();
    void tzDiffChanged();
    void housesTypeChanged();
    void withJulianCalendarChanged();

private slots:
    void recalc();
};

#endif // __SYMBOID_ASTRO_UICONTROLS_QT_QHORAVIEWITEM_H__
