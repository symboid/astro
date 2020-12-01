
#ifndef __SYMBOID_ASTRO_HORA_QHORAOBJECT_H__
#define __SYMBOID_ASTRO_HORA_QHORAOBJECT_H__

#include "astro/hora/defs.h"
#include "astro/db/qephobject.h"
#include "astro/hora/qaspectconfig.h"

typedef eph::ecl_coords::dist QOrbisValue;

class QHoraObject : public QEphObject
{
    Q_OBJECT

public:
    QHoraObject(QObject* parent, const QString& name, QOrbisConfigNodeGetter orbisGetter);

public:
    QOrbisValue aspectOrbis(const QAspectConfigNode* aspectConfig) const;
private:
    QOrbisConfigNodeGetter mObjectOrbisGetter;
};

#endif // __SYMBOID_ASTRO_HORA_QHORAOBJECT_H__
