
#ifndef __SYMBOID_ASTRO_DB_QFIXSTARCONFIG_H__
#define __SYMBOID_ASTRO_DB_QFIXSTARCONFIG_H__

#include "astro/db/defs.h"
#include "sdk/hosting/qconfig.h"

class QFixstarConfig : public QConfigNode
{
    Q_OBJECT
public:
    QFixstarConfig(QConfigNode* parentNode, const char* parentSignal);
    ~QFixstarConfig();

    Q_CONFIG_PROPERTY(bool, enabled, false, tr("Include fixed stars"))
};

#endif // __SYMBOID_ASTRO_DB_QFIXSTARCONFIG_H__
