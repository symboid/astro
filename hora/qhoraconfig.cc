
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

    mAspectTypes[hor::conjunction] = aspects()->conjunctionNode();
    mAspectTypes[hor::opposition] = aspects()->oppositionNode();
    mAspectTypes[hor::trigon] = aspects()->trigonNode();
    mAspectTypes[hor::quadrat] = aspects()->quadratNode();
    mAspectTypes[hor::quintile] = aspects()->quintileNode();
    mAspectTypes[hor::sextile] = aspects()->sextileNode();

    mAspectTypes[hor::semi_sextile] = aspects()->semi_sextileNode();
    mAspectTypes[hor::quincunx] = aspects()->quincunxNode();
    mAspectTypes[hor::semi_quadrat] = aspects()->semi_quadratNode();
    mAspectTypes[hor::sesqui_quadrat] = aspects()->sesqui_quadratNode();
    mAspectTypes[hor::biquintile] = aspects()->biquintileNode();
}

QHoraConfig::~QHoraConfig()
{
    QSettings settings;
    aspects()->saveToSettings(&settings);
    orbis()->saveToSettings(&settings);
    fixstars()->saveToSettings(&settings);
    karma()->saveToSettings(&settings);
}
