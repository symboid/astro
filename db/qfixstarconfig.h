
#ifndef __SYMBOID_ASTRO_DB_QFIXSTARCONFIG_H__
#define __SYMBOID_ASTRO_DB_QFIXSTARCONFIG_H__

#include "astro/db/defs.h"
#include "sdk/hosting/qconfig.h"

class ASTRO_DB_API QFixstarConfig : public QConfigNode
{
    Q_OBJECT
public:
    QFixstarConfig(const QString& id, QAbstractConfig* parentNode, const char* parentSignal)
        : QConfigNode(id, parentNode, parentSignal)
    {
    }

    Q_CONFIG_PROPERTY(bool, enabled, false)
};

#endif // __SYMBOID_ASTRO_DB_QFIXSTARCONFIG_H__
