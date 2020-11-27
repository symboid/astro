
#include "astro/hora/setup.h"
#include "astro/hora/qorbisconfig.h"

OrbisConfig::OrbisConfig(QOrbisConfig* orbisConfig, int index)
{
    mAspectConfigs[hor::none_aspect] = nullptr;
    mAspectConfigs[hor::conjunction] = orbisConfig->conjunction()->orbis()->subConfig(index);
    mAspectConfigs[hor::opposition] = orbisConfig->opposition()->orbis()->subConfig(index);
    mAspectConfigs[hor::trigon] = orbisConfig->trigon()->orbis()->subConfig(index);
    mAspectConfigs[hor::quadrat] = orbisConfig->quadrat()->orbis()->subConfig(index);
    mAspectConfigs[hor::quintile] = orbisConfig->quintile()->orbis()->subConfig(index);
    mAspectConfigs[hor::sextile] = orbisConfig->sextile()->orbis()->subConfig(index);

    mAspectConfigs[hor::semi_sextile] = orbisConfig->semi_sextile()->orbis()->subConfig(index);
    mAspectConfigs[hor::quincunx] = orbisConfig->quincunx()->orbis()->subConfig(index);
    mAspectConfigs[hor::semi_quadrat] = orbisConfig->semi_quadrat()->orbis()->subConfig(index);
    mAspectConfigs[hor::sesqui_quadrat] = orbisConfig->sesqui_quadrat()->orbis()->subConfig(index);
    mAspectConfigs[hor::biquintile] = orbisConfig->biquintile()->orbis()->subConfig(index);
}

hor::orbis OrbisConfig::aspect_orbis(hor::aspect_type _aspect_type) const
{
    const QConfigNode* orbisConfigNode = mAspectConfigs[_aspect_type];
    return orbisConfigNode != nullptr ? orbisConfigNode->value().toDouble() : 0.0;
}
