
#include "astro/controls/setup.h"
#include "astro/controls/init.h"
#include <qglobal.h>

mod_astro_controls::mod_astro_controls()
{
    qRegisterMetaType<QArcCoord::Degree>("Degree");
    qRegisterMetaType<QArcCoord::Minute>("Minute");
    qRegisterMetaType<QArcCoord::Second>("Second");
    qRegisterMetaType<QArcCoord::FracSecond>("FracSecond");
    qRegisterMetaType<QArcCoord::ArcDegree>("ArcDegree");

    Q_INIT_RESOURCE(astro_controls);
    load_translator();
}

mod_astro_controls::~mod_astro_controls()
{
    Q_CLEANUP_RESOURCE(astro_controls);
}
