
#include "astro/db/setup.h"
#include "astro/db/fixstars.h"

Fixstars::Fixstars()
{
}

bool Fixstars::load()
{
    bool loadResult(false);
    loadResult = eph_proxy::fixstar::load_from_disk(this);
    return loadResult;
}

void Fixstars::addFixstar(const std::string& _name, const std::string& _nomenclature, double _magnitude)
{
    _M_fixstar_list.push_back(eph::fixstar_data<eph_proxy>(_name, _nomenclature, _magnitude));
}
