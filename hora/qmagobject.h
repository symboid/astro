
#ifndef __SYMBOID_ASTRO_HORA_QMAGOBJECT_H__
#define __SYMBOID_ASTRO_HORA_QMAGOBJECT_H__

#include "astro/hora/defs.h"
#include "astro/db/qephobject.h"
#include "astro/controls/qastrofont.h"

typedef eph::ecl_coords::dist QOrbisValue;

class QMagObject : public QEphObject
{
    Q_OBJECT

public:
    QMagObject(QObject* parent, const QString& id);
    virtual QMagObject* clone() const = 0;

public:
    virtual QString symbol(const QAstroFont* font) const = 0;
    virtual QColor drawColor() const = 0;

public:
    virtual QOrbisValue orbis() const = 0;
};

#endif // __SYMBOID_ASTRO_HORA_QMAGOBJECT_H__
