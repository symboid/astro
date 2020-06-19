
#include "astro/db/setup.h"
#include "astro/db/init.h"
#include <qglobal.h>
#include "astro/sweph/cxxproxy.h"

mod_astro_db::mod_astro_db()
{
    Q_INIT_RESOURCE(astro_db);
    load_translator();
    eph_proxy::set_eph_dir_path(".");
    _M_fixstars->load();
}

mod_astro_db::~mod_astro_db()
{
    Q_CLEANUP_RESOURCE(astro_db);
}
