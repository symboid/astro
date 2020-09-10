
#include "astro/hora/setup.h"
#include "astro/hora/forecast.h"

#include <set>
/*
class ForcEvent : public Sa::ForcEvent
{
public:
    ForcEvent(const MagItem* sgtor, const eph::time_point& initTime);

private:
    MagItem* mSgtor;
    eph::time_point mExactTime;
    MagItem* mPmsor;
    eph::time_point mBeginTime;
    eph::time_point mEndTime;

public:
    const MagItem* sgtor() const override;
    const MagItem* pmsor() const override;

public:
    eph::time_point exactTimePos() const override;
    eph::time_point beginTimePos() const override;
    eph::time_point endTimePos() const override;
    bool lt(const Sa::ForcEvent::Ref rhs) const override;

public:
    void calc(Forecast::Model* forcModel, MagItems& pmsors);

public:
    SyString debugStr(bool isShort = false) const override;
    SyString symbolStr(const AstroFont* symbolFont) const override;
};
*/
ForcEvent::ForcEvent(const MagItem* sgtor, const eph::time_point& initTime)
    : mSgtor(sgtor->clone())
    , mExactTime(initTime)
    , mPmsor(nullptr)
{
}

const MagItem* ForcEvent::sgtor() const
{
    return mSgtor;
}

const MagItem* ForcEvent::pmsor() const
{
    return mPmsor;
}

eph::time_point ForcEvent::exactTimePos() const
{
    return mExactTime;
}

eph::time_point ForcEvent::beginTimePos() const
{
    return mBeginTime;
}

eph::time_point ForcEvent::endTimePos() const
{
    return mEndTime;
}

bool ForcEvent::lt(const ForcEvent* rhs) const
{
    return mExactTime < rhs->exactTimePos();
}

void ForcEvent::calc(ForecastModel* forcModel, MagItems& pmsors)
{
    if (forcModel)
    {
        forcModel->initSgtorPos(mSgtor, mExactTime);
        MagItems::Siblings siblings = pmsors.findSiblings(mSgtor);
        mExactTime = forcModel->calcConj(mSgtor, mExactTime,
                siblings.mPrec->eclPos(), siblings.mSucc->eclPos());

        eph::ecl_pos sgtorPos = mSgtor->eclPos();
        bool precIsCloser = sgtorPos.dist_abs(siblings.mPrec->eclPos()) < sgtorPos.dist_abs(siblings.mSucc->eclPos());
        mPmsor = precIsCloser ? siblings.mPrec->clone() : siblings.mSucc->clone();

        // calculation of event orbis:
        mBeginTime = forcModel->calcOrbisTime(mSgtor, mPmsor, mExactTime, true);
        mEndTime = forcModel->calcOrbisTime(mSgtor, mPmsor, mExactTime, false);
    }
}

QString ForcEvent::debugStr(bool isShort) const
{
    QString eventString;
    if (mSgtor != nullptr && mPmsor != nullptr)
    {
        eventString = QString(mSgtor->isRetrograd() ? "r." : "  ") + mSgtor->name() + " ";
        if (isShort)
        {
            Aspect::Auto aspect(aspectType());
            eventString += aspect->debugStr() + " " + mPmsor->name();
        }
        else
        {
            eventString += mPmsor->debugStr() + " " + mExactTime->debugStr();
        }
    }
    return eventString;
}

QString ForcEvent::symbolStr(QAstroFont* symbolFont) const
{
    QString symbols;
    if (mSgtor != nullptr && mPmsor != nullptr)
    {
        QString retrogradStr = symbolFont->retrogradLetter();

        Aspect::Auto aspect(aspectType());
        symbols = mSgtor->symbolStr(symbolFont);
        if (mSgtor->isRetrograd())
        {
            symbols += retrogradStr;
        }
        symbols += aspect->symbolStr(symbolFont);
        symbols += mPmsor->symbolStr(symbolFont);
        if (mPmsor->isRetrograd())
        {
            symbols += retrogradStr;
        }
    }
    return symbols;
}

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

Forecast::Forecast(const hor::hora* radixHora)
    : mRadixHora(radixHora)
{
    mAspectTypes.insert(hor::aspect_type::conjunction);
    mAspectTypes.insert(hor::aspect_type::opposition);
    mAspectTypes.insert(hor::aspect_type::trigon);
    mAspectTypes.insert(hor::aspect_type::quadrat);
    mAspectTypes.insert(hor::aspect_type::sextil);

//    mAspectTypes.insert(A_SEMI_SEXTIL);
//    mAspectTypes.insert(A_QUINCUNX);
    mAspectTypes.insert(hor::aspect_type::quintil);
}

void Forecast::addAspectType(hor::aspect_type aspectType)
{
    mAspectTypes.insert(aspectType);
}

void Forecast::collectPmsors()
{
    MagItemFactory::Auto magItemFactory(mRadixHora);

    mPmsors.deleteElements();

    int planetCount = mRadixHora->planetCount();
    for (int p = 0; p < planetCount; ++p)
    {
        for (AspectType aspectType : mAspectTypes)
        {
            mPmsors.insert(magItemFactory->createPlanetItem(p, aspectType));
            if (aspectType != A_CONJUNCTION && aspectType != A_OPPOSITION)
            {
                mPmsors.insert(magItemFactory->createPlanetItem(p, aspectType, true));
            }
        }
    }
    for (int h = 1; h <= mRadixHora->houseCount(); ++h)
    {
        mPmsors.insert(magItemFactory->createHouseCuspItem(h));
        mPmsors.insert(magItemFactory->createSignBorderItem(Se::ZodSign(h)));
    }

    for (MagItem* forcItem = mPmsors.first(); forcItem != nullptr; forcItem = forcItem->mNext)
    {
        sy_debug("pmsor item: %s", forcItem->debugStr().cStr());
    }
}

Forecast::EventList Forecast::initEventList(ForecastModel* forcModel, const eph::time_point& periodBegin)
{
    EventList events;

    forcModel->init(mRadixHora);
    ForecastModel::Sgtors sgtors = forcModel->sgtors(mRadixHora);
    for (MagItem* sgtor : sgtors)
    {
        ForcEvent* event = new ForcEvent(sgtor, periodBegin);
        event->calc(forcModel, mPmsors);
        events.insert(event);
    }

//    sgtors.deleteElements();

    return events;
}

Forecast::EventList Forecast::calc(ForecastModel* forcModel, const eph::time_point& periodBegin,
        const eph::time_point& periodEnd)
{
    collectPmsors();

    EventList events = initEventList(forcModel, periodBegin);

    EventList forcEvents;

    eph::time_point currentTime = periodBegin;
    while (currentTime < periodEnd)
    {
        ForcEvent* nextEvent = events.takeFirst();
        if (nextEvent != nullptr)
        {
            currentTime = nextEvent->exactTimePos()->add(1.0 / 24.0);
            forcEvents.push_back(nextEvent);

            MagItem* copySgtor = nextEvent->sgtor()->clone();
            ForcEvent* newEvent = new ForcEvent(copySgtor, currentTime);
            newEvent->calc(forcModel, mPmsors);
            events.insert(newEvent);
        }
    }

    for (ForcEvent* event : forcEvents)//.first(); event != nullptr; event = event->mNext)
    {
        log_info << "FORECAST EVENT: " << event->debugStr().toStdString();
    }

    return forcEvents;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

eph::time_point calcTransitTime(MagItem* sgtor, const eph::time_point& startTime,
        const eph::ecl_pos& precPos, const eph::ecl_pos& succPos)
{
    eph::time_point conjTime = startTime;

    if (sgtor != nullptr)
    {
        eph::ecl_lont DIST_TOLERANCE = 1.0 / 3600.0;

        eph::ecl_pos conjPos = sgtor->calcEclPos(startTime);
        while (conjPos.dist_abs(precPos) > DIST_TOLERANCE && conjPos.dist_abs(succPos) > DIST_TOLERANCE)
        {
            // speed of planet at current position (measured in degrees/day):
            eph::ecl_speed speed = conjPos->speed();
            double estmDays = 1.0;

            // approx forward to succeeding pmsor:
            if (speed > 0.0)
            {
                // distance to succeeding promissor in ecliptic degree:
                eph::ecl_lont succDist = succPos.dist_to(conjPos);

                // linear estimation of remaining days of progression:
                estmDays = succDist / speed;
            }

            // approx backward to preceeding pmsor:
            else
            {
                // distance to preceding promissor in ecliptic degree:
                eph::ecl_lont precDist = precPos.dist_to(conjPos);

                estmDays = precDist / speed;
            }

            if (estmDays > 1.0)
            {
                estmDays = 1.0;
            }
            else if (estmDays < -1.0)
            {
                estmDays = -1.0;
            }

            // stepping forward in time with estimated amount of days:
            conjTime->addDays(estmDays);
    /*
            sy_debug("prsn time: %0.3lf", conjTime->utTime());
            sy_debug("succ dist: %0.3lf", succPos->distTo(conjPos));
            sy_debug("prec dist: %0.3lf", precPos->distTo(conjPos));
            sy_debug("pln speed: %0.3lf", speed);
            sy_debug("estm days: %0.3lf", estmDays);
    */
            // recalculation of progression position:
            conjPos = sgtor->calcEclPos(conjTime);
        }
    }

    return conjTime;
}

ForecastTransit::ForecastTransit(Type type)
    : mType(type)
{
}

eph::ecl_pos ForecastTransit::initSgtorPos(MagItem* sgtor, const eph::time_point& startTime)
{
    return sgtor != nullptr ? sgtor->calcEclPos(startTime) : eph::ecl_pos();
}

eph::time_point ForecastTransit::calcConj(MagItem* sgtor, const eph::time_point& startTime,
        const eph::ecl_pos& precPos, const eph::ecl_pos& succPos)
{
    return calcTransitTime(sgtor, startTime, precPos, succPos);
}

eph::time_point ForecastTransit::calcOrbisTime(const MagItem* sgtor, const MagItem* pmsor,
        const eph::time_point& exactTime, bool calcBeginTime)
{
    if (sgtor != nullptr)
    {
        eph::ecl_pos sgtorPos = sgtor->eclPos();
        hor::orbis orbis = sgtor->connectingOrbis(pmsor->aspectType()) + pmsor->connectingOrbis();
        orbis = calcBeginTime ? -orbis : orbis;
        return calcConj(sgtor->clone(), exactTime, sgtorPos->stepLont(-orbis), sgtorPos->stepLont(orbis));
    }
    else
    {
        return exactTime;
    }
}

void ForecastTransit::init(const hor::hora* hora)
{
    SY_UNUSED(hora);
}

ForecastModel::Sgtors ForecastTransit::sgtors(const hor::hora* hora) const
{
    Sgtors sgtors;
    MagItemFactory::Auto magItemFactory(hora);

    std::size_t planetCount = hora->planet_count();
    for (std::size_t planetIndex = 0; planetIndex < planetCount; ++planetIndex)
    {
        if (isPlanetSgtor(hora->planet(planetIndex)))
        {
            sgtors.push_back(magItemFactory->createPlanetItem(planetIndex));
        }
    }

    return sgtors;
}

bool ForecastTransit::isPlanetSgtor(const hor::planet& planet) const
{
    switch (mType)
    {
    case MOON_TRANSIT: return planet.get_index() == hor::planet::moon; break;
    case SLOW_PLANETS_TRANSIT: return planet.get_index() > hor::planet::mars; break;
    case GENERIC_TRANSIT: return planet.get_index() != hor::planet::moon; break;
    }
}

ForecastPriDirex::ForecastPriDirex()
    : mRadixTime(eph::basic_calendar<eph_proxy>::time({2000,1,1}))
{
}

eph::ecl_pos ForecastPriDirex::initSgtorPos(MagItem* sgtor, const eph::time_point& startTime)
{
    if (sgtor != nullptr)
    {
        eph::ecl_pos radixPos = sgtor->calcEclPos(mRadixTime);
        eph::ecl_lont yearSum = (startTime->utTime() - mRadixTime->utTime()) / 365.25;
        eph::ecl_pos sgtorPos(radixPos._M_lont + yearSum);
        sgtor->setEclPos(sgtorPos);
        return sgtorPos;
    }
    else
    {
        return eph::ecl_pos();
    }
}

eph::time_point ForecastPriDirex::calcConj(MagItem* sgtor, const eph::time_point& startTime,
        const eph::ecl_pos& precPos, const eph::ecl_pos& succPos)
{
    SY_UNUSED(precPos);

    eph::time_point conjTime;
    if (sgtor != nullptr)
    {
        // ecliptic distance to succeeding item (measured in degrees):
        eph::ecl_lont succDist = succPos.dist_to(sgtor->eclPos());
        // years to step forward is equivalent to ecliptic distance in degrees:
        double yearsToStep = succDist;
        // stepping forward
        conjTime = startTime->add(yearsToStep * 365.25);

        sgtor->setEclPos(succPos);
    }
    return conjTime;
}

eph::time_point ForecastPriDirex::calcOrbisTime(const MagItem* sgtor, const MagItem* pmsor,
        const eph::time_point& exactTime, bool calcBeginTime)
{
    if (sgtor != nullptr)
    {
        hor::orbis orbisMonths = SY_MAX(sgtor->direxOrbis(), pmsor->direxOrbis());
        orbisMonths = calcBeginTime ? -orbisMonths : orbisMonths;
        return exactTime->add(orbisMonths * 30.0);
    }
    else
    {
        return exactTime;
    }
}

void ForecastPriDirex::init(const hor::hora* hora)
{
    mRadixTime = hora->radixPos()->julDay();
}

ForecastModel::Sgtors ForecastPriDirex::sgtors(const hor::hora* hora) const
{
    Sgtors sgtorArray;
    MagItemFactory::Auto magItemFactory(hora);

    std::size_t planetCount = hora->planet_count();
    for (std::size_t planetIndex = 0; planetIndex < planetCount; ++planetIndex)
    {
        sgtorArray.push_back(magItemFactory->createPlanetItem(planetIndex));
    }

    for (int axis = 1; axis < 12; axis += 3)
    {
        sgtorArray.push_back(magItemFactory->createHouseCuspItem(axis));
    }

    return sgtorArray;
}

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


eph::ecl_pos ForecastSecDirex::initSgtorPos(MagItem* sgtor, const eph::time_point& startTime)
{
    return sgtor != nullptr ? sgtor->calcEclPos(realToImg(startTime)) : eph::ecl_pos();
}

eph::time_point ForecastSecDirex::calcConj(MagItem* sgtor, const eph::time_point& startTime,
        const eph::ecl_pos& precPos, const eph::ecl_pos& succPos)
{
    return imgToReal(calcTransitTime(sgtor, realToImg(startTime), precPos, succPos));
}

eph::time_point ForecastSecDirex::calcOrbisTime(const MagItem* sgtor, const MagItem* pmsor,
        const eph::time_point& exactTime, bool calcBeginTime)
{
    if (sgtor != nullptr)
    {
        eph::ecl_pos sgtorPos = sgtor->eclPos();
        hor::orbis orbis = sgtor->connectingOrbis(pmsor->aspectType()) + pmsor->connectingOrbis();
        orbis = calcBeginTime ? -orbis : orbis;
        return calcConj(sgtor->clone(), exactTime, sgtorPos->stepLont(-orbis), sgtorPos->stepLont(orbis));
    }
    else
    {
        return exactTime;
    }
}

void ForecastSecDirex::init(const hor::hora* hora)
{
    mRadixTime = hora->radixPos()->julDay();
}

ForecastModel::Sgtors ForecastSecDirex::sgtors(const hor::hora* hora) const
{
    Sgtors sgtors;
    MagItemFactory::Auto magItemFactory(hora);

    for (int planetIndex = 0; planetIndex < 7; ++planetIndex)
    {
        sgtors.push_back(magItemFactory->createPlanetItem(planetIndex));
    }
/*
    for (int axis = 1; axis < 12; axis += 3)
    {
        sgtors.pushBack(magItemFactory->createHouseCuspItem(axis));
    }
*/
    return sgtors;
}

eph::time_point ForecastSecDirex::realToImg(const eph::time_point& realTime)
{
    double years = (realTime->utTime() - mRadixTime->utTime()) / 365.25;
    double daysAfterRadix = years;
    return mRadixTime->add(daysAfterRadix);
}

eph::time_point ForecastSecDirex::imgToReal(const eph::time_point& imgTime)
{
    double daysAfterRadix = imgTime->utTime() - mRadixTime->utTime();
    double years = daysAfterRadix;
    return mRadixTime->add(years * 365.25);
}
