
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/init.h"
#include <qglobal.h>
#include "sdk/arch/log.h"

#include <QTranslator>
#include <QCoreApplication>
#include <QLocale>

namespace std {

template<class _CharT, class _Traits>
basic_ostream<_CharT, _Traits>&
operator<<(basic_ostream<_CharT, _Traits>& __os, const QString& __qstring)
{
    return __os << __qstring.toUtf8().data();
}

}

mod_astro_uicontrols_qt::mod_astro_uicontrols_qt()
{
    qRegisterMetaType<QArcCoord::Degree>("Degree");
    qRegisterMetaType<QArcCoord::Minute>("Minute");
    qRegisterMetaType<QArcCoord::Second>("Second");
    qRegisterMetaType<QArcCoord::FracSecond>("FracSecond");
    qRegisterMetaType<QArcCoord::ArcDegree>("ArcDegree");

    Q_INIT_RESOURCE(astro_uicontrols_qt);

    QString langId = QLocale::system().name().left(2);
    log_info << "language ID is '" << langId << "'";

    const QString moduleName = QString(this->id).replace('_','-');
    QTranslator* qmFile = new QTranslator;
    QObject::connect(QCoreApplication::instance(), SIGNAL(aboutToQuit()), qmFile, SLOT(deleteLater()));

    if (!qmFile->load(QLocale::system(), moduleName, "_", ":/tr"))
    {
        log_error << "translation for module '" << moduleName << "' cannot be loaded!";
    }
    else if (!QCoreApplication::installTranslator(qmFile))
    {
        log_error << "translation file '" << moduleName << "' loaded but cannot be installed!";
    }
    else
    {
        log_info << "translation file '" << moduleName << "' loaded and installed.";
    }
}

mod_astro_uicontrols_qt::~mod_astro_uicontrols_qt()
{
    Q_CLEANUP_RESOURCE(astro_uicontrols_qt);
}
