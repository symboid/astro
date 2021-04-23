
#ifndef __SYMBOID_ASTRO_HORA_QMAGOBJECT_H__
#define __SYMBOID_ASTRO_HORA_QMAGOBJECT_H__

#include "astro/hora/defs.h"
#include "astro/db/qephobject.h"

typedef eph::ecl_coords::dist QOrbisValue;

class QMagObject : public QEphObject
{
    Q_OBJECT

public:
    QMagObject(QObject* parent, const QString& id);

public:
    virtual QOrbisValue orbis() const = 0;
};

class QMagObjectList : public QList<QMagObject*>
{
public:
    void insertMagObject(QMagObject* magObject);
};

#endif // __SYMBOID_ASTRO_HORA_QMAGOBJECT_H__
