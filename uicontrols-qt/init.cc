
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/init.h"
#include <qglobal.h>
#include "sdk/arch/log.h"

#include <QTranslator>
#include <QCoreApplication>
#include <QLocale>

mod_astro_uicontrols_qt::mod_astro_uicontrols_qt()
{
    qRegisterMetaType<QArcCoord::Degree>("Degree");
    qRegisterMetaType<QArcCoord::Minute>("Minute");
    qRegisterMetaType<QArcCoord::Second>("Second");
    qRegisterMetaType<QArcCoord::FracSecond>("FracSecond");
    qRegisterMetaType<QArcCoord::ArcDegree>("ArcDegree");

    Q_INIT_RESOURCE(astro_uicontrols_qt);
    load_translator();
}

mod_astro_uicontrols_qt::~mod_astro_uicontrols_qt()
{
    Q_CLEANUP_RESOURCE(astro_uicontrols_qt);
}
