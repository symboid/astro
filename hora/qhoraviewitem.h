
#ifndef __SYMBOID_ASTRO_HORA_QHORAVIEWITEM_H__
#define __SYMBOID_ASTRO_HORA_QHORAVIEWITEM_H__

#include "astro/hora/defs.h"
#include <QQuickPaintedItem>
#include "astro/eph/ecliptic.h"
#include "astro/calculo/hora.h"
#include "astro/controls/qastrofont.h"
#include "astro/hora/qhoraitemsmodel.h"
#include "astro/eph/constellation.h"
#include "astro/db/fixstars.h"
#include "astro/hora/qhoraconfig.h"

class ASTRO_HORA_API QHoraPlanetsModel : public QEclipticTableModel
{
    Q_OBJECT

public:
    QHoraPlanetsModel(const hor::hora* hora, QObject* parent = Q_NULLPTR);

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QStringList headerModel() const override;
};

class ASTRO_HORA_API QHoraHousesModel : public QEclipticTableModel
{
    Q_OBJECT

public:
    QHoraHousesModel(const hor::hora* hora, QObject* parent = Q_NULLPTR);

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QStringList headerModel() const override;
};

class ASTRO_HORA_API QHoraViewItem : public QQuickPaintedItem
{
    Q_OBJECT

public:
    static constexpr const char* qml_name = "HoraView";

public:
    QHoraViewItem(QQuickItem* parent = Q_NULLPTR);

private:
    static constexpr qreal PI = 3.14159265;
    static constexpr qreal PLANET_DIST = 0.875;
    static constexpr qreal ASPECT_DIST = 1.0 - 2.0 * (1.0 - PLANET_DIST);

    QBrush planetBrush(hor::planet::index planetIndex, qreal alpha);
    enum Rank
    {
        PERG = 0x00, // peregrin

        HOME = 0x03, // otthon
        EXIL = 0x01, // szamuzetes

        ELEV = 0x0C, // eroben
        FALL = 0x04, // esesben
    };
    Rank planetRank(const hor::planet& planet) const;
    void drawPlanetSymbol(QPainter* painter, const hor::planet& planet, const eph::ecl_pos& displayPos);
//    void drawConstellation(QPainter* painter, const eph::constellation* constellation);
    void paint(QPainter* painter) override;
private:
    QPointF horaPoint(eph::ecl_lont horaLont, qreal dist) const;

private:
    QPointF mMandalaCenter;
    qreal mMandalaRadius;
    hor::hora mHora;

    Q_PROPERTY(const hor::hora* hora READ hora CONSTANT)
private:
    const hor::hora* hora() const { return & mHora; }
//    QList<eph::constellation*> mConstellations;

private:
    eph::ecl_lont mandalaLeft() const;
    qreal eclipticRatio() const;
    qreal eclipticRadius() const;
    qreal oneDegree() const;
private slots:
    void calcMandalaGeometry();
public:
    Q_PROPERTY(qreal defaultZoom READ defaultZoom NOTIFY defaultZoomChanged)
    qreal defaultZoom() const;
signals:
    void defaultZoomChanged();

private:
    QSharedPointer<QAstroFont> mAstroFont;

public:
    Q_PROPERTY(bool interactive MEMBER mIsInteractive WRITE setInteractive NOTIFY interactiveChanged)
private:
    bool mIsInteractive = false;
    void setInteractive(bool isInteractive);
signals:
    void interactiveChanged();
private slots:
    void onInteractiveChanged();

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
    void fontPointSizeChanged();

private slots:
    void recalc();
signals:
    void startCalc();
    void stopCalc();

public:
    Q_PROPERTY(QEclipticTableModel* planetsModel READ planetsModel NOTIFY planetsModelChanged)
    Q_PROPERTY(QEclipticTableModel* housesModel READ housesModel NOTIFY housesModelChanged)
private:
    QHoraPlanetsModel* mPlanetsModel;
    QHoraHousesModel* mHousesModel;
    QEclipticTableModel* planetsModel() const { return mPlanetsModel; }
    QEclipticTableModel* housesModel() const { return mHousesModel; }
signals:
    void planetsModelChanged();
    void housesModelChanged();

public:
    enum DisplayFlags
    {
        NONE = 0x00,
        SHOW_FIXSTARS = 0x01,
    };
    Q_ENUM(DisplayFlags)
public:
    Q_PROPERTY(DisplayFlags displayFlags MEMBER mDisplayFlags WRITE setDisplayFlags NOTIFY displayFlagsChanged)
private:
    DisplayFlags mDisplayFlags;
    void setDisplayFlags(DisplayFlags displayFlags);
public:
    Q_INVOKABLE void setDisplayFlag(DisplayFlags displayFlag, bool isSet = true);
    Q_INVOKABLE bool isDisplayFlagSet(DisplayFlags displayFlag) const;
signals:
    void displayFlagsChanged();

private:
    arh::main_object<Fixstars> mFixstars;
    arh::main_object<QHoraConfig> mHoraConfig;
};

#endif // __SYMBOID_ASTRO_HORA_QHORAVIEWITEM_H__
