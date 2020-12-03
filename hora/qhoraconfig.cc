
#include "astro/hora/setup.h"
#include "astro/hora/qhoraconfig.h"

QKarmaConfig::QKarmaConfig(const QString& id, QAbstractConfig* parentNode, const char* parentSignal)
    : QConfigNode(id, parentNode, parentSignal)
{
}

QHouseCuspOrbisConfigNode::QHouseCuspOrbisConfigNode(const QString& id, QAbstractConfig* parentNode, const char* parentSignal)
    : QConfigNode(id, parentNode, parentSignal)
{
}

QHoraConfig::QHoraConfig(QObject* parent)
    : QConfigNode(parent)
{
    QSettings settings;
    for (int c = 0; c < subConfigCount(); ++c)
    {
        subConfig(c)->loadFromSettings(&settings);
    }
}

QHoraConfig::~QHoraConfig()
{
    QSettings settings;
    for (int c = 0; c < subConfigCount(); ++c)
    {
        subConfig(c)->saveToSettings(&settings);
    }
}
