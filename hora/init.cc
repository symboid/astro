
#include "astro/hora/setup.h"
#include "astro/hora/init.h"
#include <qglobal.h>

#include "sdk/hosting/qsoftwareupdate.h"
#include "astro/component.h"


mod_astro_hora::mod_astro_hora()
{
    Q_INIT_RESOURCE(astro_hora);
    load_translator();

    arh::main_object<QSoftwareUpdate> softwareUpdate;
    softwareUpdate->addComponentVersion(COMPONENT_NAME, COMPONENT_VER_MAJOR, COMPONENT_VER_MINOR,
                        COMPONENT_VER_PATCH, COMPONENT_VER_SERIAL, COMPONENT_REV_ID);
}

mod_astro_hora::~mod_astro_hora()
{
    Q_CLEANUP_RESOURCE(astro_hora);
}
