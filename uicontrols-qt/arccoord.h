
#ifndef __SYMBOID_ASTRO_UICONTROLS_QT_ARCCOORD_H__
#define __SYMBOID_ASTRO_UICONTROLS_QT_ARCCOORD_H__

#include "astro/uicontrols-qt/defs.h"
#include <QObject>
#include "astro/eph/ecliptic.h"

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
    Q_PROPERTY(Degree degree READ degree WRITE setDegree NOTIFY degreeChanged)
    Q_PROPERTY(Minute minute READ minute WRITE setMinute NOTIFY minuteChanged)
    Q_PROPERTY(Second second READ second WRITE setSecond NOTIFY secondChanged)
    Q_PROPERTY(FracSecond fracSecond READ fracSecond)
    Q_PROPERTY(ArcDegree arcDegree READ arcDegree WRITE setArcDegree NOTIFY arcDegreeChanged)

private:
    Degree mDegree;
    Minute mMinute;
    Second mSecond;
    ArcDegree mFracSecond;

public:
    Degree degree() const;
    Minute minute() const;
    Second second() const;
    FracSecond fracSecond() const;
    ArcDegree arcDegree() const;
    void setDegree(Degree degree);
    void setMinute(Minute minute);
    void setSecond(Second second);
    void setArcDegree(ArcDegree arcDegree);

signals:
    void degreeChanged();
    void minuteChanged();
    void secondChanged();
    void arcDegreeChanged();
};

#endif // __SYMBOID_ASTRO_UICONTROLS_QT_ARCCOORD_H__
