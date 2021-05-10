
#ifndef __SYMBOID_ASTRO_DB_QEPHOBJECT_H__
#define __SYMBOID_ASTRO_DB_QEPHOBJECT_H__

#include "astro/db/defs.h"
#include "astro/eph/calendar.h"
#include "astro/db/swephcpp.h"
#include "astro/eph/ecliptic.h"
#include <QObject>

typedef eph::ecl_pos QEclPos;
typedef eph::ecl_speed QEclSpeed;

class ASTRO_DB_API QEphObject : public QObject
{
    Q_OBJECT

public:
    QEphObject(QObject* parent, const QString& id);
    ~QEphObject();

public:
    virtual QString id() const;
private:
    const QString mId;

public:
    virtual QEclPos eclPos() const = 0;
    virtual QEclSpeed eclSpeed() const = 0;
};

#endif // __SYMBOID_ASTRO_DB_QEPHOBJECT_H__
