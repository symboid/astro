
#ifndef __SYMBOID_ASTRO_DEMO_QML_INIT_H__
#define __SYMBOID_ASTRO_DEMO_QML_INIT_H__

#include "astro/demo-qml/defs.h"
#include "sdk/arch/appqml.h"
#include "sdk/network/init.h"
#include "astro/controls/init.h"
#include "sdk/dox/init.h"

struct app_astro_demo_qt : arh::app_qml<app_astro_demo_qt>
{
    APP_OBJECT(astro_demo_qt)

    app_astro_demo_qt(int* _argc, char*** _argv)
        : arh::app_qml<app_astro_demo_qt>(_argc, _argv)
    {
    }
    arh::mod_init<mod_sdk_network> _M_mod_sdk_network;
    arh::mod_init<mod_astro_controls> _M_mod_astro_controls;
    arh::mod_init<mod_sdk_dox> _M_mod_sdk_dox;
};

#endif // __SYMBOID_ASTRO_DEMO_QML_INIT_H__
