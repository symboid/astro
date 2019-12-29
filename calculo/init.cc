
#include "astro/calculo/setup.h"
#include "astro/calculo/init.h"
#include "sdk/basics/frontend/tr.h"
#include <qglobal.h>
#include <QQmlEngine>
#include "astro/calculo/solarcalcobject.h"
#include "astro/calculo/lunarcalcobject.h"
#include "astro/calculo/horaview.h"

int initAstroCalculo()
{
    qmlRegisterType<As::SolarCalcObject>("Symboid.Astro.Calculo", 1, 0, "SolarCalcObject");
    qmlRegisterType<As::LunarCalcObject>("Symboid.Astro.Calculo", 1, 0, "LunarCalcObject");
    qmlRegisterType<As::HoraView>("Symboid.Astro.Calculo", 1, 0, "HoraView");

    // still just for having dependency on frontend library
    // without qt resources have not initiated here
    Q_INIT_RESOURCE(astro_calculo);
// TODO: temporary fix !!!
#if !SY_PLATFORM_IS_ANDROID
    Sf::loadTranslations("astro-calculo");
#endif

    return 0;
}
