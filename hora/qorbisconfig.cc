
#include "astro/hora/setup.h"
#include "astro/hora/qorbisconfig.h"

OrbisConfig::OrbisConfig(QOrbisConfig* orbisConfig, int index)
{
    mAspectConfigs[hor::none_aspect] = nullptr;
    mAspectConfigs[hor::conjunction] = orbisConfig->conjunction()->subConfig(index);
    mAspectConfigs[hor::opposition] = orbisConfig->opposition()->subConfig(index);
    mAspectConfigs[hor::trigon] = orbisConfig->trigon()->subConfig(index);
    mAspectConfigs[hor::quadrat] = orbisConfig->quadrat()->subConfig(index);
    mAspectConfigs[hor::quintile] = orbisConfig->quintile()->subConfig(index);
    mAspectConfigs[hor::sextile] = orbisConfig->sextile()->subConfig(index);

    mAspectConfigs[hor::semi_sextile] = orbisConfig->semi_sextile()->subConfig(index);
    mAspectConfigs[hor::quincunx] = orbisConfig->quincunx()->subConfig(index);
    mAspectConfigs[hor::semi_quadrat] = orbisConfig->semi_quadrat()->subConfig(index);
    mAspectConfigs[hor::sesqui_quadrat] = orbisConfig->sesqui_quadrat()->subConfig(index);
    mAspectConfigs[hor::biquintile] = orbisConfig->biquintile()->subConfig(index);
}

hor::orbis OrbisConfig::aspect_orbis(hor::aspect_type _aspect_type) const
{
    const QConfigNode* orbisConfigNode = mAspectConfigs[_aspect_type];
    return orbisConfigNode != nullptr ? orbisConfigNode->value().toDouble() : 0.0;
}
