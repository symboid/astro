
#include "astro/db/setup.h"
#include "astro/db/fixstars.h"

Fixstars::Fixstars()
{
}

bool Fixstars::load()
{
    bool loadResult(false);
    loadResult = eph_proxy::fixstar::load_from_disk(this);
    eph::basic_fixstar<eph_proxy> fixstar(_M_fixstars.front());
    return loadResult;
}

void Fixstars::addFixstar(const std::string& _name, const std::string& _nomenclature, double _magnitude)
{
    _M_fixstars.push_back(eph::fixstar_data<eph_proxy>(_name, _nomenclature, _magnitude));
}
