
#ifndef __SYMBOID_ASTRO_DB_FIXSTARS_H__
#define __SYMBOID_ASTRO_DB_FIXSTARS_H__

#include "astro/db/defs.h"
#include "sdk/arch/mainobject.h"
#include "astro/eph/proxy.h"

class ASTRO_DB_API Fixstars : public eph_proxy::fixstar::AddFunctor
{
    MAIN_OBJECT(Fixstars,Fixstars)

public:
    Fixstars();

public:
    bool load();
private:
    void addFixstar(const std::string& _name, const std::string& _nomenclature, double _magnitude) override;
private:
    std::list<eph::fixstar_data<eph_proxy>> _M_fixstars;
};

#endif // __SYMBOID_ASTRO_DB_FIXSTARS_H__
