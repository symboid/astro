
#include "setup.h"
#include "qaspectconfig.h"

QAspectConfig::QAspectConfig(QConfigNode* parentNode)
    : QConfigNode(tr("Aspect connections"), parentNode)
{
    QSettings settings;
    loadFromSettings(&settings);
}

QAspectConfig::~QAspectConfig()
{
    QSettings settings;
    saveToSettings(&settings);
    settings.sync();
}
