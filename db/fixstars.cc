
#include "astro/db/setup.h"
#include "astro/db/fixstars.h"
#include "astro/sweph/cxxproxy.h"

Fixstars::Fixstars()
{
}

bool Fixstars::load()
{
    bool loadResult(false);
    std::list<eph::basic_fixstar<swe::proxy>> fixstars;
    loadResult = swe::proxy::fixstar::load_from_disk(fixstars);
    return loadResult;
}
