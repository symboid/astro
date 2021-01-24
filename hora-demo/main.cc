
#include "astro/hora-demo/defs.h"
#include "sdk/arch/mainobject.h"
#include "astro/hora-demo/init.h"
#include "astro/component.h"
#include "sdk/hosting/qsoftwareupdate.h"
#include "sdk/hosting/qappconfig.h"
#include <QQuickStyle>

int main(int _argc, char* _argv[])
{
    arh::main_object_init<app_astro_hora_demo, int*, char***> app(&_argc, &_argv);
    arh::main_object<QSoftwareUpdate> softwareUpdate;
    softwareUpdate->mAppSwid = COMPONENT_SWID;
    softwareUpdate->setAppVersion(COMPONENT_NAME, COMPONENT_VER_MAJOR, COMPONENT_VER_MINOR,
                        COMPONENT_VER_PATCH, COMPONENT_VER_SERIAL, COMPONENT_REV_ID);

    arh::main_object<QAppConfig> appConfig;
    QString st = appConfig->ui()->style();
    QQuickStyle::setStyle(appConfig->ui()->style());

    return app->run();
}
