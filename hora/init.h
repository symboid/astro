
#ifndef __SYMBOID_ASTRO_HORA_INIT_H__
#define __SYMBOID_ASTRO_HORA_INIT_H__

#include "astro/hora/defs.h"
#include "sdk/arch/modqt.h"
#include "sdk/controls/init.h"
#include "sdk/network/init.h"
#include "sdk/hosting/init.h"
#include "astro/controls/init.h"
#include "astro/db/init.h"
#include "astro/hora/qhoraviewitem.h"
#include "astro/hora/qorbisconfig.h"

struct ASTRO_HORA_API mod_astro_hora : arh::mod_qt<mod_astro_hora>
{
    MOD_OBJECT(astro_hora)

    static constexpr const char* qml_pkg_name = "Symboid.Astro.Hora";
    static constexpr int qml_pkg_ver_major = 1;
    static constexpr int qml_pkg_ver_minor = 0;

    mod_astro_hora();
    ~mod_astro_hora();

    arh::mod_init<mod_sdk_network> _M_mod_sdk_network;
    arh::mod_init<mod_sdk_controls> _M_mod_sdk_controls;
    arh::mod_init<mod_sdk_hosting> _M_mod_sdk_hosting;
    arh::mod_init<mod_astro_controls> _M_mod_astro_controls;
    arh::mod_init<mod_astro_db> _M_mod_astro_db;

    qml_type_register<QHoraViewItem> _M_reg_hora_view;
    qml_singleton_init<QOrbisConfig> _M_orbis_config;
};


#endif // __SYMBOID_ASTRO_HORA_INIT_H__