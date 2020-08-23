
#include "astro/controls/setup.h"
#include "astro/controls/init.h"
#include <qglobal.h>
#include "sdk/arch/log.h"

#include <QTranslator>
#include <QCoreApplication>
#include <QLocale>

#include "sdk/hosting/qsoftwareupdate.h"
#include "astro/component.h"

mod_astro_controls::mod_astro_controls()
{
    qRegisterMetaType<QArcCoord::Degree>("Degree");
    qRegisterMetaType<QArcCoord::Minute>("Minute");
    qRegisterMetaType<QArcCoord::Second>("Second");
    qRegisterMetaType<QArcCoord::FracSecond>("FracSecond");
    qRegisterMetaType<QArcCoord::ArcDegree>("ArcDegree");

    Q_INIT_RESOURCE(astro_controls);
    load_translator();

    arh::main_object<QSoftwareUpdate> softwareUpdate;
    softwareUpdate->addComponentVersion(COMPONENT_NAME, COMPONENT_VER_MAJOR, COMPONENT_VER_MINOR,
                        COMPONENT_VER_PATCH, COMPONENT_VER_SERIAL, COMPONENT_REV_ID);
}

mod_astro_controls::~mod_astro_controls()
{
    Q_CLEANUP_RESOURCE(astro_controls);
}