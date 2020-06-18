
#include "astro/db/setup.h"
#include "astro/db/init.h"
#include <qglobal.h>

mod_astro_db::mod_astro_db()
{
    Q_INIT_RESOURCE(astro_db);
    load_translator();
}

mod_astro_db::~mod_astro_db()
{
    Q_CLEANUP_RESOURCE(astro_db);
}
