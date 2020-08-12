
#include "astro/db/setup.h"
#include "astro/db/init.h"
#include <qglobal.h>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>
#include <QDateTime>
#include "astro/eph/proxy.h"
#include "sdk/arch/log.h"
#include "astro/component.h"

mod_astro_db::mod_astro_db()
{
    Q_INIT_RESOURCE(astro_db);
    load_translator();

#if SY_PLATFORM_IS_ANDROID
    QDir epheDir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    epheDir.mkdir("ephe");
    epheDir.cd("ephe");

    // checking whether ephemeris may have been modified
    QString fingerPrint;
    QFile fingerPrintFile(epheDir.absoluteFilePath("_fp"));
    if (fingerPrintFile.open(QIODevice::ReadOnly))
    {
        fingerPrint = fingerPrintFile.readAll();
        fingerPrintFile.close();
    }
    if (fingerPrint != COMPONENT_REV_ID)
    {
        // purging ephe dir
        for (QFileInfo epheFileInfo : epheDir.entryInfoList(QDir::Files))
        {
            QFile::remove(epheFileInfo.absoluteFilePath());
        }
        // copying ephe files
        QDir epheAssetsDir("assets:/ephe");
        for (QFileInfo epheAssetInfo : epheAssetsDir.entryInfoList())
        {
            QFile::copy(epheAssetInfo.absoluteFilePath(),
                        epheDir.absoluteFilePath(epheAssetInfo.fileName()));
        }
        // printing fingerprint
        if (fingerPrintFile.open(QIODevice::WriteOnly))
        {
            fingerPrintFile.write(COMPONENT_REV_ID);
            fingerPrintFile.close();
        }
        log_info << "Ephemeris files deployed.";
    }
#else
    QDir epheDir(".");
#endif
    eph_proxy::set_eph_dir_path(epheDir.absolutePath().toStdString());
    _M_fixstars->load(epheDir.absoluteFilePath("sefstars.txt"));
}

mod_astro_db::~mod_astro_db()
{
    Q_CLEANUP_RESOURCE(astro_db);
}
