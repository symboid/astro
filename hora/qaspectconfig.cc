
#include "astro/hora/setup.h"
#include "astro/hora/qaspectconfig.h"

OrbisConfig::OrbisConfig(QAspectConfig* aspectConfig, int index)
{
    mAspectConfigs[hor::none_aspect] = nullptr;
    mAspectConfigs[hor::conjunction] = aspectConfig->conjunction()->orbis()->subConfig(index);
    mAspectConfigs[hor::opposition] = aspectConfig->opposition()->orbis()->subConfig(index);
    mAspectConfigs[hor::trigon] = aspectConfig->trigon()->orbis()->subConfig(index);
    mAspectConfigs[hor::quadrat] = aspectConfig->quadrat()->orbis()->subConfig(index);
    mAspectConfigs[hor::quintile] = aspectConfig->quintile()->orbis()->subConfig(index);
    mAspectConfigs[hor::sextile] = aspectConfig->sextile()->orbis()->subConfig(index);

    mAspectConfigs[hor::semi_sextile] = aspectConfig->semi_sextile()->orbis()->subConfig(index);
    mAspectConfigs[hor::quincunx] = aspectConfig->quincunx()->orbis()->subConfig(index);
    mAspectConfigs[hor::semi_quadrat] = aspectConfig->semi_quadrat()->orbis()->subConfig(index);
    mAspectConfigs[hor::sesqui_quadrat] = aspectConfig->sesqui_quadrat()->orbis()->subConfig(index);
    mAspectConfigs[hor::biquintile] = aspectConfig->biquintile()->orbis()->subConfig(index);
}

hor::orbis OrbisConfig::aspect_orbis(hor::aspect_type _aspect_type) const
{
    const QConfigNode* orbisConfigNode = mAspectConfigs[_aspect_type];
    return orbisConfigNode != nullptr ? orbisConfigNode->value().toDouble() : 0.0;
}
