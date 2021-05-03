
#include "astro/hora/setup.h"
#include "astro/hora/qhousecusp.h"

QHouseSystem::QHouseSystem(QObject* parent)
    : QObject(parent)
{
}

bool QHouseSystem::calc(eph::basic_time_point<eph_proxy> horaTime, eph::arc_degree geoLont, eph::arc_degree geoLatt)
{
    bool calcResult(false);
    static constexpr typename eph::basic_calendar<eph_proxy>::days TIME_DIFF(10.0 / 1440.0);
    QEclLont houseCuspLontsNext[HOUSE_COUNT + 1];

    if (eph_proxy::houses::calc(horaTime, eph_proxy::houses::type(mType), geoLont, geoLatt, mEclLonts) == eph::calc_result::SUCCESS &&
        eph_proxy::houses::calc(horaTime + TIME_DIFF, eph_proxy::houses::type(mType), geoLont, geoLatt, houseCuspLontsNext) == eph::calc_result::SUCCESS)
    {
        for (int h = 1; h <= HOUSE_COUNT; ++h)
        {
            QEclLont houseLontDiff = houseCuspLontsNext[h] - mEclLonts[h];
            QEclLont houseSpeed(houseLontDiff / TIME_DIFF.count(), true);
            eph::arc_degree degPerHour = (houseSpeed.to_arc_degree() + 360) / 24;
            mEclSpeed[h] = QEclSpeed(degPerHour, 0.0);
        }
        calcResult = true;
        emit  recalculated();
    }
    return calcResult;
}

QString QHouseCusp::resolveName(int houseIndex)
{
    switch (houseIndex)
    {
    case 1: return "I.";
    case 4: return "IV.";
    case 7: return "VII.";
    case 10: return "X.";
    default: return houseIndex < 0 || houseIndex > 12 ? "" : QString::number(houseIndex) + ".";
    }
}

QOrbisConfigNodeGetter QHouseCusp::resolveConfigGetter(int houseIndex)
{
    switch (houseIndex)
    {
    case 1: case 7: return &QOrbisConfigNode::ascNode;
    case 4: case 10: return &QOrbisConfigNode::mcNode;
    default: return houseIndex < 0 || houseIndex > 12 ? nullptr : &QOrbisConfigNode::cuspNode;
    }
}

QHouseCusp::QHouseCusp(QObject* parent, const QHouseSystem* houseSystem, int houseIndex)
    : QHoraObject(parent, resolveName(houseIndex), resolveConfigGetter(houseIndex))
    , mHouseSystem(houseSystem)
    , mHouseIndex(houseIndex)
{
    connect(houseSystem, SIGNAL(recalculated()), this, SIGNAL(eclPosChanged()));
    connect(houseSystem, SIGNAL(recalculated()), this, SIGNAL(eclSpeedChanged()));
}

QEclPos QHouseCusp::eclPos() const
{
    return mHouseSystem ? mHouseSystem->mEclLonts[mHouseIndex] : QEclPos();
}

QEclSpeed QHouseCusp::eclSpeed() const
{
    return mHouseSystem ? mHouseSystem->mEclSpeed[mHouseIndex] : QEclSpeed();
}

QString QHouseCusp::abbrName() const
{
    return id().left(id().length()-1);
}

QString QHouseCusp::symbol(const QAstroFont *font) const
{
    Q_UNUSED(font)
    return id();
}

QColor QHouseCusp::drawColor() const
{
    return Qt::gray;
}
