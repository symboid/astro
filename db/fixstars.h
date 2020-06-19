
#ifndef __SYMBOID_ASTRO_DB_FIXSTARS_H__
#define __SYMBOID_ASTRO_DB_FIXSTARS_H__

#include "astro/db/defs.h"
#include "sdk/arch/mainobject.h"

class ASTRO_DB_API Fixstars
{
    MAIN_OBJECT(Fixstars,Fixstars)

public:
    Fixstars();

public:
    bool load();
};

#endif // __SYMBOID_ASTRO_DB_FIXSTARS_H__
