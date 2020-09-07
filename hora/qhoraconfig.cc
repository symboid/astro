
#include "astro/hora/setup.h"
#include "astro/hora/qhoraconfig.h"
#include "sdk/arch/appqt.h"
#include <QDir>
#include <QStandardPaths>

QHoraConfig::QHoraConfig(QObject* parent)
    : QJsonSyncFile(parent)
{
    static QString appName(arh::main_object<arh::qt_application>()->applicationName());
    QDir configDir(QStandardPaths::writableLocation(QStandardPaths::ConfigLocation));
    if (configDir.mkpath(appName))
    {
        setFilePath(configDir.absoluteFilePath(appName) + QDir::separator() + "hora.cfg");
    }
    load();
}

QHoraConfig::~QHoraConfig()
{
    save();
}
