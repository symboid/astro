
#include "astro/hora/setup.h"
#include "astro/hora/qaspectconfig.h"

OrbisConfig::OrbisConfig(QAspectConfig* aspectConfig, PlanetNodeFn planetNodeFn)
{
    mAspectConfigs[hor::none_aspect] = nullptr;
    mAspectConfigs[hor::conjunction] = (aspectConfig->conjunction()->orbis()->*planetNodeFn)();
    mAspectConfigs[hor::opposition] = (aspectConfig->opposition()->orbis()->*planetNodeFn)();
    mAspectConfigs[hor::trigon] = (aspectConfig->trigon()->orbis()->*planetNodeFn)();
    mAspectConfigs[hor::quadrat] = (aspectConfig->quadrat()->orbis()->*planetNodeFn)();
    mAspectConfigs[hor::quintile] = (aspectConfig->quintile()->orbis()->*planetNodeFn)();
    mAspectConfigs[hor::sextile] = (aspectConfig->sextile()->orbis()->*planetNodeFn)();

    mAspectConfigs[hor::semi_sextile] = (aspectConfig->semi_sextile()->orbis()->*planetNodeFn)();
    mAspectConfigs[hor::quincunx] = (aspectConfig->quincunx()->orbis()->*planetNodeFn)();
    mAspectConfigs[hor::semi_quadrat] = (aspectConfig->semi_quadrat()->orbis()->*planetNodeFn)();
    mAspectConfigs[hor::sesqui_quadrat] = (aspectConfig->sesqui_quadrat()->orbis()->*planetNodeFn)();
    mAspectConfigs[hor::biquintile] = (aspectConfig->biquintile()->orbis()->*planetNodeFn)();
}

hor::orbis OrbisConfig::aspect_orbis(hor::aspect_type _aspect_type) const
{
    const QConfigNode* orbisConfigNode = mAspectConfigs[_aspect_type];
    return orbisConfigNode != nullptr ? orbisConfigNode->value().toDouble() : 0.0;
}
