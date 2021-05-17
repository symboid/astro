
#include "astro/hora/setup.h"
#include "astro/hora/qhora.h"
#include "astro/hora/qaspectobject.h"

QHora::QHora(QObject* parent)
    : QCalcable(parent)
    , mHouseSystem(new QHouseSystem(this))
    , mRegularAspectObjects(new QAspectObjectList)
    , mCoords(nullptr)
{
    connect(this, SIGNAL(calcTaskChanged()), this, SLOT(onCalcTaskChanged()));

    mHouseSystem->mType = QHouseSystem::PLACIDUS;
    for (int h = 0; h < QHouseSystem::HOUSE_COUNT; ++h)
    {
        mHouseCusps[h] = new QHouseCusp(this, mHouseSystem, h + 1);
    }
    mHouseCusps[QHouseSystem::HOUSE_COUNT] = nullptr;

    connect(mHoraConfig->karma(), SIGNAL(changed()), this, SLOT(updatePlanets()));
    updatePlanets();
}

QHouseCusp*const* QHora::housesBegin() const
{
    return & mHouseCusps[0];
}

QHouseCusp*const* QHora::housesEnd() const
{
    return & mHouseCusps[QHouseSystem::HOUSE_COUNT];
}

const QHouseCusp* QHora::house(int index) const
{
    return index > 0 && index <= 12 ? mHouseCusps[index - 1] : nullptr;
}

QHora::Planets::ConstIterator QHora::planetsBegin() const
{
    return mPlanets.begin();
}

QHora::Planets::ConstIterator QHora::planetsEnd() const
{
    return mPlanets.end();
}

int QHora::planetCount() const
{
    return mPlanets.size();
}

const QPlanet* QHora::planet(int index) const
{
    return mPlanets.at(index);
}

void QHora::updatePlanets()
{
    for (QPlanet* planet : mPlanets)
    {
        planet->deleteLater();
    }
    mPlanets.clear();

    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::sunNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::monNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::merNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::venNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::marNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::jupNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::satNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::uraNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::nepNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::pluNode));

    if (mHoraConfig->karma()->dragon_head())
    {
        mPlanets.push_back(new QLunarNode(this, QLunarNode::DRAGON_HEAD));
    }
    if (mHoraConfig->karma()->dragon_tail())
    {
        mPlanets.push_back(new QLunarNode(this, QLunarNode::DRAGON_TAIL));
    }
    if (mHoraConfig->karma()->lilith())
    {
        mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::lilNode));
    }

    QAspectConfig* aspects = mHoraConfig->aspects();
    for (QAspectObject* aspectObject : mAspectObjects)
    {
        aspectObject->deleteLater();
    }
    mAspectObjects.clear();
    for (QPlanet* planet : mPlanets)
    {
        for (int a = 0, aCount = aspects->subConfigCount(); a < aCount; ++a)
        {
            QAspectConfigNode* aspect = aspects->mSubConfigs[a];
            if (aspect->dist() == 0.0 || aspect->dist() == 180.0)
            {
                mAspectObjects.push_back(new QAspectObject(planet, aspect));
            }
            else
            {
                mAspectObjects.push_back(new QLowerAspectObject(planet, aspect));
                mAspectObjects.push_back(new QUpperAspectObject(planet, aspect));
            }
        }
    }
    for (int h = 0; h < QHouseSystem::HOUSE_COUNT; ++h)
    {
        mAspectObjects.push_back(new QAspectObject(mHouseCusps[h], aspects->conjunction()));
    }

    emit planetsUpdated();
}

QHora::ConjunctingFixstars::ConstIterator QHora::fixstarsBegin() const
{
    return mConjunctingFixstars.begin();
}

QHora::ConjunctingFixstars::ConstIterator QHora::fixstarsEnd() const
{
    return mConjunctingFixstars.end();
}

QHoraCoords* QHora::coords() const
{
    return mCoords;
}
void QHora::setCoords(QHoraCoords* coords)
{
    if (mCoords != coords)
    {
        deleteParam(mCoords);
        mCoords = coords;
        addParam(mCoords);
        emit coordsChanged();
    }
}

QHouseSystem::Type QHora::houseSystemType() const
{
    return mHouseSystem->mType;
}

void QHora::setHouseSystemType(const QHouseSystem::Type& houseSystemType)
{
    if (mHouseSystem->mType != houseSystemType)
    {
        mHouseSystem->mType = houseSystemType;
        emit houseSystemTypeChanged();
    }
}

void QHora::onCalcTaskChanged()
{
    if (mCalcTask)
    {
        connect(this, SIGNAL(houseSystemTypeChanged()), mCalcTask, SLOT(invoke()));
    }
}


void QHora::calc()
{
    // hora time corrected with time zone data
    QEphTime horaTime = mCoords->ephTime();

    bool calcResult = true;

    // getting house cusp positions
    calcResult = mHouseSystem->calc(horaTime, mCoords->geoLont(), mCoords->geoLatt());

    // getting planet positions
    for (Planets::iterator planet = mPlanets.begin(), end = mPlanets.end(); calcResult && planet < end; ++planet)
    {
        calcResult = (*planet)->calc(horaTime);
    }

    // getting fixstar data (if enabled)
    mConjunctingFixstars.clear();
    if (mHoraConfig->fixstars()->included())
    {
        typedef typename Fixstars::Container::iterator Iterator;
        for (Iterator fixstarData = mAllFixstars->begin(), fEnd = mAllFixstars->end();
             calcResult && fixstarData != fEnd; ++fixstarData)
        {
            if (mAllFixstars->filter_match(*fixstarData))
            {
                eph::basic_fixstar<eph_proxy> fixstar(fixstarData->get());
                calcResult = (fixstar.calc_pos(horaTime) == eph::calc_result::SUCCESS);
                if (calcResult)
                {
                    bool conjuncting = false;
                    const eph::ecl_pos fixstarPos = fixstar.pos();
                    const QOrbisValue fixstarOrbis = (*fixstarData)->orbis();
                    for (Planets::Iterator planet = mPlanets.begin(), pEnd = mPlanets.end();
                            !conjuncting && planet != pEnd; ++planet)
                    {
                        conjuncting = ((*planet)->eclPos().dist_abs(fixstarPos) < fixstarOrbis);
                    }
    //                    for (std::vector<eph::house_cusp>::const_iterator house = _M_houses.begin(), h_end = _M_houses.end();
    //                            !conjuncting && house != h_end; ++house)
    //                    {
    //                        conjuncting = (house->pos().dist_abs(fixstar_pos) < fixstar_orbis);
    //                    }
                    if (conjuncting)
                    {
                        mConjunctingFixstars.push_back(fixstar);
                    }
                }
            }
        }
    }

    mRegularAspectObjects->clear();
    mRegularAspectObjects = fetchAspectObjects({180, 120, 90, 60});

//    return calcResult;
}

QSharedPointer<QAspectObjectList> QHora::fetchAspectObjects(const QVector<qreal>& dists) const
{
    QAspectObjectList* aspectObjects = new QAspectObjectList;
    for (QAspectObject* aspectObject : mAspectObjects)
    {
        if (aspectObject->aspect()->enabled())
        if (aspectObject->aspect()->enabled() && dists.indexOf(aspectObject->aspect()->dist()) != -1)
        {
            aspectObjects->insert(aspectObject);
        }
    }
    return QSharedPointer<QAspectObjectList>(aspectObjects);
}

const QSharedPointer<QAspectObjectList> QHora::regularAspectObjects() const
{
    return mRegularAspectObjects;
}
