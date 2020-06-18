
#ifndef __SYMBOID_ASTRO_DB_FIXSTARS_H__
#define __SYMBOID_ASTRO_DB_FIXSTARS_H__

#include "astro/db/defs.h"
#include <QObject>
#include "sdk/arch/mainobject.h"

class ASTRO_DB_API QFixstars : public QObject
{
    Q_OBJECT
    QML_SINGLETON(Fixstars)

public:
    QFixstars(QObject* parent = Q_NULLPTR);

public:
    bool load();
};

#endif // __SYMBOID_ASTRO_DB_FIXSTARS_H__
