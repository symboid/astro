
#include "astro/demo-qml/setup.h"
#include "astro/demo-qml/init.h"

int main(int argc, char **argv)
{
    arh::main_object_init<app_astro_demo_qt, int*, char***> app(&argc, &argv);
    return app->run();
}
