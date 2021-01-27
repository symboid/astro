
#include "astro/hora-demo/setup.h"
#include "astro/hora-demo/init.h"
#include <qglobal.h>
#include "sdk/hosting/qappconfig.h"
#include "astro/eph/proxy.h"

app_astro_hora_demo::app_astro_hora_demo(int* _argc, char*** _argv)
    : arh::app_qml<app_astro_hora_demo>(_argc, _argv)
{
    Q_INIT_RESOURCE(astro_hora_demo);
    load_translator();
    arh::main_object<QAppConfig> appConfig;
    appConfig->software()->update_methodSet(2);
    eph_proxy::set_calc_type(swe::proxy::calc_type::MOSHIER);
}

app_astro_hora_demo::~app_astro_hora_demo()
{
    Q_CLEANUP_RESOURCE(astro_hora_demo);
}
