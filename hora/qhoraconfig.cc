
#include "astro/hora/setup.h"
#include "astro/hora/qhoraconfig.h"

QHoraConfig::QHoraConfig(QObject* parent)
    : QConfigNode(parent)
{
    QSettings settings;
    for (int c = 0; c < subConfigCount(); ++c)
    {
        subConfig(c)->loadFromSettings(&settings);
    }

    mAspectTypes[hor::conjunction] = aspects()->conjunction();
    mAspectTypes[hor::opposition] = aspects()->opposition();
    mAspectTypes[hor::trigon] = aspects()->trigon();
    mAspectTypes[hor::quadrat] = aspects()->quadrat();
    mAspectTypes[hor::quintile] = aspects()->quintile();
    mAspectTypes[hor::sextile] = aspects()->sextile();

    mAspectTypes[hor::semi_sextile] = aspects()->semi_sextile();
    mAspectTypes[hor::quincunx] = aspects()->quincunx();
    mAspectTypes[hor::semi_quadrat] = aspects()->semi_quadrat();
    mAspectTypes[hor::sesqui_quadrat] = aspects()->sesqui_quadrat();
    mAspectTypes[hor::biquintile] = aspects()->biquintile();
}

QHoraConfig::~QHoraConfig()
{
    QSettings settings;
    for (int c = 0; c < subConfigCount(); ++c)
    {
        subConfig(c)->saveToSettings(&settings);
    }
}
