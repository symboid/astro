
#ifndef __SYMBOID_ASTRO_DB_QEPHOBJECT_H__
#define __SYMBOID_ASTRO_DB_QEPHOBJECT_H__

#include "astro/db/defs.h"
#include "astro/eph/calendar.h"
#include "astro/db/swephcpp.h"
#include "astro/eph/ecliptic.h"
#include <QObject>

typedef eph::basic_time_point<eph_proxy> QEphTime;

typedef eph::ecl_pos QEclPos;
typedef eph::ecl_speed QEclSpeed;

class QEphObject : public QObject
{
    Q_OBJECT

public:
    QEphObject(QObject* parent, const QString& id);
    ~QEphObject();

    virtual bool calc(const QEphTime& ephTime) = 0;

public:
    virtual QString id() const;
private:
    const QString mId;

    QEclPos eclPos() const;
    bool isEclPosValid() const;
protected:
    void setEclPos(const QEclPos& eclPos);
private:
    bool mIsEclPosValid = false;
    QEclPos mEclPos;
signals:
    void eclPosChanged();

public:
    QEclSpeed eclSpeed() const;
    bool isEclSpeedValid() const;
protected:
    void setEclSpeed(const QEclSpeed& eclSpeed);
private:
    bool mIsEclSpeedValid = false;
    QEclSpeed mEclSpeed;
signals:
    void eclSpeedChanged();
};

#endif // __SYMBOID_ASTRO_DB_QEPHOBJECT_H__
