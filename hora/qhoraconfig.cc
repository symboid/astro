
#include "astro/hora/setup.h"
#include "astro/hora/qhoraconfig.h"

QHoraConfig::QHoraConfig(QObject* parent)
    : QConfigNode(parent)
{
    QSettings settings;
    aspects()->loadFromSettings(&settings);
    orbis()->loadFromSettings(&settings);
    fixstars()->loadFromSettings(&settings);
    karma()->loadFromSettings(&settings);
}

QHoraConfig::~QHoraConfig()
{
    QSettings settings;
    aspects()->saveToSettings(&settings);
    orbis()->saveToSettings(&settings);
    fixstars()->saveToSettings(&settings);
    karma()->saveToSettings(&settings);
}
