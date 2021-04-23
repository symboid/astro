
#ifndef __SYMBOID_ASTRO_HORA_QHORAOBJECT_H__
#define __SYMBOID_ASTRO_HORA_QHORAOBJECT_H__

#include "astro/hora/defs.h"
#include "astro/hora/qmagobject.h"
#include "astro/hora/qaspectconfig.h"

typedef eph::ecl_coords::dist QOrbisValue;

class ASTRO_HORA_API QHoraObject : public QMagObject
{
    Q_OBJECT

public:
    QHoraObject(QObject* parent, const QString& name, QOrbisConfigNodeGetter orbisGetter);

public:
    QOrbisValue orbis() const override;
private:
    QAspectConfigNode* mConjunctionConfigNode;

public:
    QOrbisValue aspectOrbis(const QAspectConfigNode* aspectConfig) const;
private:
    QOrbisConfigNodeGetter mObjectOrbisGetter;
};

#endif // __SYMBOID_ASTRO_HORA_QHORAOBJECT_H__
