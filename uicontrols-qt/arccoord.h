
#ifndef __SYMBOID_ASTRO_UICONTROLS_QT_ARCCOORD_H__
#define __SYMBOID_ASTRO_UICONTROLS_QT_ARCCOORD_H__

#include "astro/uicontrols-qt/defs.h"
#include <QObject>
#include "astro/eph/ecliptic.h"
#include <QSharedPointer>
#include "astro/uicontrols-qt/qastrofont.h"

class ASTRO_UICONTROLS_QT_API QSectionCalc : public QObject
{
    Q_OBJECT

public:
    virtual eph::arc_coord::degree raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const = 0;
    virtual eph::arc_coord::signum signum(int sectionIndex, eph::arc_coord::degree sectionDegree) const = 0;
    virtual eph::arc_coord::degree degree(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const = 0;
    virtual int index(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const = 0;

public:
    Q_INVOKABLE virtual int sectionCount() const { return 1; }
    Q_INVOKABLE virtual int sectionMin() const { return -360; }
    Q_INVOKABLE virtual int sectionMax() const { return 360; }
    Q_INVOKABLE virtual QFont sectionFont() const { return QFont(); }
    Q_INVOKABLE virtual QStringList values() const { return QStringList(""); }
};

class ASTRO_UICONTROLS_QT_API QArcCoord : public QObject
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "ArcCoord";

public:
    explicit QArcCoord(QObject* parent = Q_NULLPTR);

public:
    typedef eph::arc_coord::degree Degree;
    typedef eph::arc_coord::minute Minute;
    typedef int Second;
    typedef eph::arc_coord::second FracSecond;
    typedef eph::arc_degree ArcDegree;

public:
    Q_PROPERTY(QSectionCalc* sectionCalc READ sectionCalc WRITE setSectionCalc NOTIFY sectionCalcChanged)
    Q_PROPERTY(int section READ section WRITE setSection NOTIFY sectionChanged)
    Q_PROPERTY(Degree degree READ degree WRITE setDegree NOTIFY degreeChanged)
    Q_PROPERTY(Minute minute READ minute WRITE setMinute NOTIFY minuteChanged)
    Q_PROPERTY(Second second READ second WRITE setSecond NOTIFY secondChanged)
    Q_PROPERTY(FracSecond fracSecond READ fracSecond)
    Q_PROPERTY(ArcDegree arcDegree READ arcDegree WRITE setArcDegree NOTIFY arcDegreeChanged)

private:
    QSharedPointer<QSectionCalc> mSectionCalc;
    int mSection;
    Degree mDegree;
    Minute mMinute;
    Second mSecond;
    ArcDegree mFracSecond;

public:
    QSectionCalc* sectionCalc() const;
    int section() const;
    Degree degree() const;
    Minute minute() const;
    Second second() const;
    FracSecond fracSecond() const;
    ArcDegree arcDegree() const;
    void setSectionCalc(QSectionCalc* sectionCalc);
    void setSection(int section);
    void setDegree(Degree rawDegree);
    void setMinute(Minute minute);
    void setSecond(Second second);
    void setArcDegree(ArcDegree arcDegree);

signals:
    void sectionCalcChanged();
    void sectionChanged();
    void degreeChanged();
    void minuteChanged();
    void secondChanged();
    void arcDegreeChanged();
};

struct ASTRO_UICONTROLS_QT_API QNoneSectionCalc : QSectionCalc
{
    static constexpr const char* qml_name = "NoneSectionCalc";

    eph::arc_coord::degree raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const override;
    eph::arc_coord::signum signum(int sectionIndex, eph::arc_coord::degree sectionDegree) const override;
    eph::arc_coord::degree degree(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const override;
    int index(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const override;
};

struct ASTRO_UICONTROLS_QT_API QSignumSectionCalc : QSectionCalc
{
    static constexpr const char* qml_name = "SignumSectionCalc";

    eph::arc_coord::degree raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const override;
    virtual eph::arc_coord::signum signum(int sectionIndex, eph::arc_coord::degree sectionDegree) const override;
    eph::arc_coord::degree degree(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const override;
    int index(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const override;

    Q_INVOKABLE virtual int sectionCount() const { return 2; }
    Q_INVOKABLE virtual int sectionMin() const { return 0; }
    Q_INVOKABLE virtual int sectionMax() const { return 180; }
    Q_INVOKABLE virtual QStringList values() const { return { "", "-" }; }

    static constexpr int positive_index = 0;
    static constexpr int negative_index = 1;
};

struct ASTRO_UICONTROLS_QT_API QGeoLattSectionCalc : QSectionCalc
{
    static constexpr const char* qml_name = "GeoLattSectionCalc";

    eph::arc_coord::degree raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const override;
    virtual eph::arc_coord::signum signum(int sectionIndex, eph::arc_coord::degree sectionDegree) const override;
    eph::arc_coord::degree degree(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const override;
    int index(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const override;
    Q_INVOKABLE virtual int sectionCount() const override { return 2; }
    Q_INVOKABLE virtual int sectionMin() const override { return 0; }
    Q_INVOKABLE virtual int sectionMax() const override { return 90; }
    Q_INVOKABLE virtual QStringList values() const override { return { tr("N"), tr("S") }; }

    static constexpr int north_index = 0;
    static constexpr int south_index = 1;
};

struct ASTRO_UICONTROLS_QT_API QGeoLontSectionCalc : QSectionCalc
{
    static constexpr const char* qml_name = "GeoLontSectionCalc";

    eph::arc_coord::degree raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const override;
    virtual eph::arc_coord::signum signum(int sectionIndex, eph::arc_coord::degree sectionDegree) const override;
    eph::arc_coord::degree degree(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const override;
    int index(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const override;
    Q_INVOKABLE virtual int sectionCount() const override { return 2; }
    Q_INVOKABLE virtual int sectionMin() const override { return 0; }
    Q_INVOKABLE virtual int sectionMax() const override { return 180; }
    Q_INVOKABLE virtual QStringList values() const override { return { tr("E"), tr("W") }; }

    static constexpr int east_index = 0;
    static constexpr int west_index = 1;

    static eph::arc_coord::degree normalizeDegree(eph::arc_coord::degree rawDegree);
};

struct ASTRO_UICONTROLS_QT_API QZodiacSectionCalc : QSectionCalc
{
    static constexpr const char* qml_name = "ZodiacSectionCalc";

    eph::arc_coord::degree raw(int sectionIndex, eph::arc_coord::degree sectionDegree) const override;
    virtual eph::arc_coord::signum signum(int sectionIndex, eph::arc_coord::degree sectionDegree) const override;
    eph::arc_coord::degree degree(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const override;
    int index(eph::arc_coord::signum signum, eph::arc_coord::degree rawDegree) const override;

    Q_INVOKABLE virtual int sectionCount() const override { return 12; }
    Q_INVOKABLE virtual int sectionMin() const override { return 0; }
    Q_INVOKABLE virtual int sectionMax() const override { return 30; }
    Q_INVOKABLE virtual QFont sectionFont() const override { return QFont("Symboid"); }
    Q_INVOKABLE virtual QStringList values() const override { return {  "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l" }; }
};

#endif // __SYMBOID_ASTRO_UICONTROLS_QT_ARCCOORD_H__
