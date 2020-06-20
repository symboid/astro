
#include "astro/db/setup.h"
#include "astro/db/fixstars.h"

Fixstars::Fixstars()
{
}

bool Fixstars::load()
{
    bool loadResult(false);
    loadResult = eph_proxy::fixstar::load_from_disk(this);
    eph::basic_fixstar<eph_proxy> fixstar(front());
    return loadResult;
}

void Fixstars::addFixstar(const std::string& _name, const std::string& _nomenclature, double _magnitude)
{
    push_back(eph::fixstar_data<eph_proxy>(_name, _nomenclature, _magnitude));
}

eph::calc_result Fixstars::findConjunctions(Conjunctions& conjunctions,
        const eph::basic_time_point<eph_proxy>& time, const eph::ecl_pos& objectPos)
{
    eph::calc_result calcResult = eph::calc_result::SUCCESS;
    return calcResult;
}
