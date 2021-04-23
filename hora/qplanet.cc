
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
        planetIndex = SUN;
    }
    else if (id == "mon")
    {
        planetIndex = MOON;
    }
    else if (id == "mer")
    {
        planetIndex = MERCURY;
    }
    else if (id == "ven")
    {
        planetIndex = VENUS;
    }
    else if (id == "mar")
    {
        planetIndex = MARS;
    }
    else if (id == "jup")
    {
        planetIndex = JUPITER;
    }
    else if (id == "sat")
    {
        planetIndex = SATURN;
    }
    else if (id == "ura")
    {
        planetIndex = URANUS;
    }
    else if (id == "nep")
    {
        planetIndex = NEPTUNE;
    }
    else if (id == "plu")
    {
        planetIndex = PLUTO;
    }
    else if (id == "lil")
    {
        planetIndex = LILITH;
    }
    return planetIndex;
}

QPlanet::QPlanet(QObject* parent, const QString& id, Index index, QOrbisConfigNodeGetter orbisGetter)
    : QHoraObject(parent, id, orbisGetter)
    , mIndex(index)
{
}

QPlanet::QPlanet(QObject* parent, QOrbisConfigNodeGetter orbisGetter)
    : QHoraObject(parent, resolveId(orbisGetter), orbisGetter)
    , mIndex(resolveIndex(id()))
{

}

QEclPos QPlanet::eclPos() const
{
    return mEclPos;
}

QEclSpeed QPlanet::eclSpeed() const
{
    return mEclSpeed;
}

bool QPlanet::calc(const QEphTime& ephTime)
{
    QEclPos eclPos;
    QEclSpeed eclSpeed;
    bool isSuccess = (eph_proxy::object::calc_pos(mIndex, ephTime, eclPos, eclSpeed) == eph::calc_result::SUCCESS);
    if (isSuccess)
    {
        mEclPos = eclPos;
        emit eclPosChanged();
        mEclSpeed = eclSpeed;
        emit eclSpeedChanged();
    }
    return isSuccess;
}

bool QPlanet::isRetrograd() const
{
    return eclSpeed()._M_lont < 0.0;
}

QString QPlanet::symbol(const QAstroFont* font) const
{
    return font->planetLetter(mIndex);
}

QLunarNode::QLunarNode(QObject* parent, Index index)
    : QPlanet(parent, index == DRAGON_HEAD ? "dragon_head" : "dragon_tail", index, &QOrbisConfigNode::nodNode)
{
}

bool QLunarNode::calc(const QEphTime& ephTime)
{
    QEclPos eclPos;
    QEclSpeed eclSpeed;
    bool isSuccess = (eph_proxy::object::calc_pos(MEAN_NODE, ephTime, eclPos, eclSpeed) == eph::calc_result::SUCCESS);
    if (isSuccess)
    {
        if (mIndex == DRAGON_TAIL)
        {
            eclPos = QEclPos(eclPos._M_lont + 180.0, eclPos._M_latt, eclPos._M_dist);
        }
        mEclPos = eclPos;
        emit eclPosChanged();
        mEclSpeed = eclSpeed;
        emit eclSpeedChanged();
    }
    return isSuccess;
}

QString QLunarNode::symbol(const QAstroFont* font) const
{
    return mIndex == DRAGON_TAIL ? font->dragonTailLetter() : font->dragonHeadLetter();
}

