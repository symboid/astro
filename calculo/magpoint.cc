
#include "astro/calculo/setup.h"
#include "astro/calculo/magpoint.h"

namespace As {

MagPoint::MagPoint(Orbis orbis)
    : mOrbis(orbis)
{
}

MagPoint::~MagPoint()
{
}

ElePoint::ElePoint(Orbis orbis)
    : MagPoint(orbis)
{
}

} // namespace As
