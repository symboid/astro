
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
        mEclSpeed = eclSpeed;
    }
    return isSuccess;
}

bool QPlanet::isRetrograd() const
{
    return eclSpeed()._M_lont < 0.0;
}

QEclSpeed::lont QPlanet::maxLontSpeed() const
{
    if (0 <= mIndex && mIndex <= PLUTO)
    {
        static const qreal MAX_SPEEDS[] = { 1.1, 16.0, 2.5, 2.5, 1.0, 1.0/4.0,
                                            1.0/8.0, 1.0/16.0, 1.0/20.0, 1.0/20.0 };
        return MAX_SPEEDS[mIndex];
    }
    else if (mIndex == MEAN_NODE)
    {
        return 1.0 / 18.0;
    }
    else if (mIndex == LILITH)
    {
        return 1.0 / 9.0;
    }
    else
    {
        return 0.0;
    }
}

QString QPlanet::abbrName() const
{
    switch (mIndex)
    {
    case SUN:       return "Sun";
    case MOON:      return "Mon";
    case MERCURY:   return "Mer";
    case VENUS:     return "Ven";
    case MARS:      return "Mar";
    case JUPITER:   return "Jup";
    case SATURN:    return "Sat";
    case URANUS:    return "Ura";
    case NEPTUNE:   return "Nep";
    case PLUTO:     return "Plu";
    case MEAN_NODE: return "Nod";
    case LILITH:    return "Lil";
    case UNDEF:     return "???";
    }
    return "???";
}

QString QPlanet::symbol(const QAstroFont* font) const
{
    return font->planetLetter(mIndex);
}

QColor QPlanet::drawColor() const
{
    return Qt::black;
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
        mEclSpeed = eclSpeed;
    }
    return isSuccess;
}

QString QLunarNode::abbrName() const
{
    switch (mIndex)
    {
    case DRAGON_HEAD: return "Hed";
    case DRAGON_TAIL: return "Tal";
    default: return "???";
    }
}

QString QLunarNode::symbol(const QAstroFont* font) const
{
    return mIndex == DRAGON_TAIL ? font->dragonTailLetter() : font->dragonHeadLetter();
}
