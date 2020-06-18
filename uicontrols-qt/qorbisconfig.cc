
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/qorbisconfig.h"

QOrbisConfig::QOrbisConfig(QConfigNode* parentNode)
    : QConfigNode(tr("Orbis"), parentNode)
{
}

OrbisConfig::OrbisConfig(int index)
{
    mAspectConfigs[hor::none_aspect] = nullptr;
    mAspectConfigs[hor::conjunction] = mOrbisConfig->conjunction()->subConfig(index);
    mAspectConfigs[hor::opposition] = mOrbisConfig->opposition()->subConfig(index);
    mAspectConfigs[hor::trigon] = mOrbisConfig->trigon()->subConfig(index);
    mAspectConfigs[hor::quadrat] = mOrbisConfig->quadrat()->subConfig(index);
    mAspectConfigs[hor::quintil] = mOrbisConfig->quintil()->subConfig(index);
    mAspectConfigs[hor::sextil] = mOrbisConfig->sextil()->subConfig(index);
}

hor::orbis OrbisConfig::aspect_orbis(hor::aspect_type _aspect_type) const
{
    const QConfigNode* orbisConfigNode = mAspectConfigs[_aspect_type];
    return orbisConfigNode != nullptr ? orbisConfigNode->value().toDouble() : 0.0;
}
