
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/qorbisconfig.h"

QOrbisConfig::QOrbisConfig(QConfigNode* parentNode)
    : QConfigNode(tr("Orbis"), parentNode)
{
}

OrbisConfig::OrbisConfig(int index)
    : mIndex(index)
{
}

hor::orbis OrbisConfig::aspect_orbis(hor::aspect_type _aspect_type) const
{
    const QConfigNode* aspectConfig = nullptr;
    switch (_aspect_type) {
    case hor::conjunction: aspectConfig = mOrbisConfig->conjunction(); break;
    case hor::opposition: aspectConfig = mOrbisConfig->opposition(); break;
    case hor::trigon: aspectConfig = mOrbisConfig->trigon(); break;
    case hor::quadrat: aspectConfig = mOrbisConfig->quadrat(); break;
    case hor::quintil: aspectConfig = mOrbisConfig->quintil(); break;
    case hor::sextil: aspectConfig = mOrbisConfig->sextil(); break;
    default: return 1.0;
    }
    return aspectConfig != nullptr ? aspectConfig->subConfig(mIndex)->configValue().toDouble() : 0.0;
}
