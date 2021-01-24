
#ifndef __SYMBOID_ASTRO_HORA_DEMO_INIT_H__
#define __SYMBOID_ASTRO_HORA_DEMO_INIT_H__

#include "astro/hora-demo/defs.h"
#include "sdk/arch/appqml.h"
#include "sdk/controls/init.h"
#include "sdk/hosting/init.h"
#include "astro/hora/init.h"
#include "astro/db/init.h"

struct app_astro_hora_demo : arh::app_qml<app_astro_hora_demo>
{
    APP_OBJECT(astro_hora_demo)

    static constexpr const char* name = "HoraDemo";
    static constexpr const char* company = "Symboid";
    static constexpr const char* domain = "symboid.com";

    app_astro_hora_demo(int* _argc, char*** _argv);
    ~app_astro_hora_demo();

    arh::mod_init<mod_sdk_controls> _M_mod_sdk_controls;
    arh::mod_init<mod_sdk_hosting> _M_mod_sdk_hosting;
    arh::mod_init<mod_astro_db> _M_mod_astro_db;
    arh::mod_init<mod_astro_hora> _M_mod_astro_hora;
};

#endif // __SYMBOID_ASTRO_HORA_DEMO_INIT_H__
