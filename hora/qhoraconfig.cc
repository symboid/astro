
#include "astro/hora/setup.h"
#include "astro/hora/qhoraconfig.h"

QKarmaConfig::QKarmaConfig(QConfigNode* parentNode, const char* parentSignal)
    : QConfigNode(tr("Karmic points"), parentNode, parentSignal)
{
    QSettings settings;
    loadFromSettings(&settings);
}

QKarmaConfig::~QKarmaConfig()
{
    QSettings settings;
    saveToSettings(&settings);
    settings.sync();
}

QHoraConfig::QHoraConfig(QObject* parent)
    : QConfigNode(parent)
{
}

QHoraConfig::~QHoraConfig()
{
}
