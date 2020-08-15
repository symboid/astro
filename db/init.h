
#ifndef __SYMBOID_ASTRO_DB_INIT_H__
#define __SYMBOID_ASTRO_DB_INIT_H__

#include "astro/db/defs.h"
#include "sdk/arch/modqt.h"
#include "astro/db/qgeonamesrestclient.h"
#include "astro/db/fixstars.h"

struct ASTRO_DB_API mod_astro_db : arh::mod_qt<mod_astro_db>
{
    MOD_OBJECT(astro_db)

    static constexpr const char* qml_pkg_name = "Symboid.Astro.Db";
    static constexpr int qml_pkg_ver_major = 1;
    static constexpr int qml_pkg_ver_minor = 0;

    mod_astro_db();
    ~mod_astro_db();

    qml_singleton_init<QGeoNamesRestClient> _M_geo_names_rest_client;
    arh::main_object_init<Fixstars> _M_fixstars;
};


#endif // __SYMBOID_ASTRO_DB_INIT_H__
