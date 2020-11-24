
#include "setup.h"
#include "qaspectconfig.h"

QAspectConfig::QAspectConfig(QConfigNode* parentNode, const char* parentSignal)
    : QConfigNode(tr("Aspect connections"), parentNode, parentSignal)
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
