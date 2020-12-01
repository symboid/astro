
#include "astro/hora/qplanet.h"
#include "astro/hora/qplanet.h"
#include "astro/hora/qhoraconfig.h"

QString QPlanet::resolveId(QOrbisConfigNodeGetter orbisGetter)
{
    arh::main_object<QHoraConfig> horaConfig;
    return (horaConfig->aspects()->conjunction()->orbis()->*orbisGetter)()->property("id").toString();
}

QPlanet::Index QPlanet::resolveIndex(const QString& id)
{
    QPlanet::Index planetIndex(eph_proxy::object::undef);
    if (id == "sun")
    {
        planetIndex = eph_proxy::object::sun;
    }
    else if (id == "mon")
    {
        planetIndex = eph_proxy::object::moon;
    }
    else if (id == "mer")
    {
        planetIndex = eph_proxy::object::mercury;
    }
    else if (id == "ven")
    {
        planetIndex = eph_proxy::object::venus;
    }
    else if (id == "mar")
    {
        planetIndex = eph_proxy::object::mars;
    }
    else if (id == "jup")
    {
        planetIndex = eph_proxy::object::jupiter;
    }
    else if (id == "sat")
    {
        planetIndex = eph_proxy::object::saturn;
    }
    else if (id == "ura")
    {
        planetIndex = eph_proxy::object::uranus;
    }
    else if (id == "nep")
    {
        planetIndex = eph_proxy::object::neptune;
    }
    else if (id == "plu")
    {
        planetIndex = eph_proxy::object::pluto;
    }
    else if (id == "nod")
    {
        planetIndex = eph_proxy::object::dragon_head;
    }
    else if (id == "lil")
    {
        planetIndex = eph_proxy::object::lilith;
    }
    return planetIndex;
}

QPlanet::QPlanet(QObject* parent, QOrbisConfigNodeGetter orbisGetter)
    : QHoraObject(parent, resolveId(orbisGetter), orbisGetter)
    , mIndex(resolveIndex(id()))
{

}

bool QPlanet::calc(const QEphTime& ephTime)
{
    QEclPos eclPos;
    QEclSpeed eclSpeed;
    bool isSuccess = (eph_proxy::object::calc_pos(mIndex, ephTime, eclPos, eclSpeed) == eph::calc_result::SUCCESS);
    if (isSuccess)
    {
        setEclPos(eclPos);
        setEclSpeed(eclSpeed);
    }
    return isSuccess;
}
