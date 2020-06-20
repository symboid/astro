
#ifndef __SYMBOID_ASTRO_DB_FIXSTARS_H__
#define __SYMBOID_ASTRO_DB_FIXSTARS_H__

#include "astro/db/defs.h"
#include "sdk/arch/mainobject.h"
#include "astro/eph/proxy.h"
#include <list>

class ASTRO_DB_API Fixstars : public std::list<eph::fixstar_data<eph_proxy>>, public eph_proxy::fixstar::AddFunctor
{
    MAIN_OBJECT(Fixstars,Fixstars)

public:
    Fixstars();

public:
    bool load();
private:
    void addFixstar(const std::string& _name, const std::string& _nomenclature, double _magnitude) override;

public:
    typedef std::list<eph::basic_fixstar<eph_proxy>> Conjunctions;
    eph::calc_result findConjunctions(Conjunctions& conjunctions,
            const eph::basic_time_point<eph_proxy>& time, const eph::ecl_pos& objectPos);
};

#endif // __SYMBOID_ASTRO_DB_FIXSTARS_H__
