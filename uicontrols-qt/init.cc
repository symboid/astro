
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/init.h"
#include <qglobal.h>

mod_astro_uicontrols_qt::mod_astro_uicontrols_qt()
{
    qRegisterMetaType<QArcCoord::Degree>("Degree");
    qRegisterMetaType<QArcCoord::Minute>("Minute");
    qRegisterMetaType<QArcCoord::Second>("Second");
    qRegisterMetaType<QArcCoord::FracSecond>("FracSecond");
    qRegisterMetaType<QArcCoord::ArcDegree>("ArcDegree");

    Q_INIT_RESOURCE(astro_uicontrols_qt);
}

mod_astro_uicontrols_qt::~mod_astro_uicontrols_qt()
{
    Q_CLEANUP_RESOURCE(astro_uicontrols_qt);
}
