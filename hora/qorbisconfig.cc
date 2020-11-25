
#include "astro/hora/setup.h"
#include "astro/hora/qorbisconfig.h"

OrbisConfig::OrbisConfig(QOrbisConfig* orbisConfig, int index)
{
    mAspectConfigs[hor::none_aspect] = nullptr;
    mAspectConfigs[hor::conjunction] = orbisConfig->conjunction()->subConfig(index);
    mAspectConfigs[hor::opposition] = orbisConfig->opposition()->subConfig(index);
    mAspectConfigs[hor::trigon] = orbisConfig->trigon()->subConfig(index);
    mAspectConfigs[hor::quadrat] = orbisConfig->quadrat()->subConfig(index);
    mAspectConfigs[hor::quintil] = orbisConfig->quintil()->subConfig(index);
    mAspectConfigs[hor::sextil] = orbisConfig->sextil()->subConfig(index);
}

hor::orbis OrbisConfig::aspect_orbis(hor::aspect_type _aspect_type) const
{
    const QConfigNode* orbisConfigNode = mAspectConfigs[_aspect_type];
    return orbisConfigNode != nullptr ? orbisConfigNode->value().toDouble() : 0.0;
}
