
#include "astro/hora/setup.h"
#include "astro/hora/init.h"
#include <qglobal.h>

#include <QTranslator>
#include <QCoreApplication>
#include <QLocale>

mod_astro_hora::mod_astro_hora()
{
    Q_INIT_RESOURCE(astro_hora);
    load_translator();
}

mod_astro_hora::~mod_astro_hora()
{
    Q_CLEANUP_RESOURCE(astro_hora);
}
