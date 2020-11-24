
#include "astro/hora/setup.h"
#include "astro/hora/qhoraconfig.h"
#include "sdk/arch/appqt.h"
#include <QDir>
#include <QStandardPaths>

QHoraConfig::QHoraConfig(QObject* parent)
    : QConfigNode(parent)
{
}

QHoraConfig::~QHoraConfig()
{
}
