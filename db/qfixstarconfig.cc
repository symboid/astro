
#include "astro/db/setup.h"
#include "astro/db/qfixstarconfig.h"

QFixstarConfig::QFixstarConfig(QConfigNode* parentNode, const char* parentSignal)
    : QConfigNode(tr("Fixed stars"), parentNode, parentSignal)
{
    QSettings settings;
    loadFromSettings(&settings);
}

QFixstarConfig::~QFixstarConfig()
{
    QSettings settings;
    saveToSettings(&settings);
}

