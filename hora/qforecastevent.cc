
#include "astro/hora/setup.h"
#include "astro/hora/qforecastevent.h"

QSigtor::QSigtor(const QMagObject* origin, const QString& id)
    : QMagObject(nullptr, id)
    , mOrigin(origin)
{
}

QEclPos QSigtor::eclPos() const
{
    return mEclPos;
}

QEclSpeed QSigtor::eclSpeed() const
{
    return mEclSpeed;
}

void QSigtor::setEclPos(const QEclPos& eclPos)
{
    mEclPos = eclPos;
}

void QSigtor::setEclSpeed(const QEclSpeed& eclSpeed)
{
    mEclSpeed = eclSpeed;
}

QString QSigtor::abbrName() const
{
    return mOrigin->abbrName();
}

QString QSigtor::symbol(const QAstroFont* font) const
{
    return mOrigin->symbol(font);
}

QColor QSigtor::drawColor() const
{
    return mOrigin->drawColor();
}

QOrbisValue QSigtor::orbis() const
{
    return mOrigin->orbis();
}

QPlanetSigtor::QPlanetSigtor(const QPlanet* planetOrigin)
    : QSigtor(planetOrigin, planetOrigin->id())
    , mPlanetOrigin(planetOrigin)
{
}

QSigtor* QPlanetSigtor::clone() const
{
    QSigtor* planetSigtor = new QPlanetSigtor(mPlanetOrigin);
    planetSigtor->setEclPos(mEclPos);
    planetSigtor->setEclSpeed(mEclSpeed);
    return planetSigtor;
}

bool QPlanetSigtor::calcEclPos(const QEphTime& ephTime, eph::arc_degree, eph::arc_degree)
{
    QEclPos eclPos;
    QEclSpeed eclSpeed;
    QPlanet::Index objectIndex = mPlanetOrigin->mIndex == QPlanet::Index(1000) || mPlanetOrigin->mIndex == QPlanet::Index(1001) ?
                QPlanet::MEAN_NODE : mPlanetOrigin->mIndex;
    bool isSuccess = (eph_proxy::object::calc_pos(objectIndex, ephTime, eclPos, eclSpeed) == eph::calc_result::SUCCESS);
    if (isSuccess)
    {
        if (mPlanetOrigin->mIndex == QLunarNode::DRAGON_TAIL)
        {
            eclPos = QEclPos(eclPos._M_lont + 180.0, eclPos._M_latt, eclPos._M_dist);
        }
        mEclPos = eclPos;
        mEclSpeed = eclSpeed;
    }
    return isSuccess;
}

QHouseCuspSigtor::QHouseCuspSigtor(const QHouseCusp* houseCuspOrigin)
    : QSigtor(houseCuspOrigin, houseCuspOrigin->id())
    , mHouseCuspOrigin(houseCuspOrigin)
{
}

bool QHouseCuspSigtor::calcEclPos(const QEphTime& ephTime, eph::arc_degree geoLatt, eph::arc_degree geoLont)
{
    bool calcResult(false);
    static constexpr typename eph::basic_calendar<eph_proxy>::days TIME_DIFF(10.0 / 1440.0);
    QEclLont houseCuspLonts[QHouseSystem::HOUSE_COUNT + 1], houseCuspLontsNext[QHouseSystem::HOUSE_COUNT + 1];
    QHouseSystem::Type hsType = mHouseCuspOrigin->mHouseSystem->mType;

    if (eph_proxy::houses::calc(ephTime, eph_proxy::houses::type(hsType), geoLont, geoLatt, houseCuspLonts) == eph::calc_result::SUCCESS &&
        eph_proxy::houses::calc(ephTime + TIME_DIFF, eph_proxy::houses::type(hsType), geoLont, geoLatt, houseCuspLontsNext) == eph::calc_result::SUCCESS)
    {
        int h = mHouseCuspOrigin->mHouseIndex;
        QEclLont houseLontDiff = houseCuspLontsNext[h] - houseCuspLonts[h];
        QEclLont houseSpeed(houseLontDiff / TIME_DIFF.count(), true);
        eph::arc_degree degPerHour = (houseSpeed.to_arc_degree() + 360) / 24;
        mEclPos = houseCuspLonts[h];
        mEclSpeed = QEclSpeed(degPerHour, 0.0);
        calcResult = true;
    }
    return calcResult;
}

QSigtor* QHouseCuspSigtor::clone() const
{
    QSigtor* houseCuspSigtor = new QHouseCuspSigtor(mHouseCuspOrigin);
    houseCuspSigtor->setEclPos(mEclPos);
    houseCuspSigtor->setEclSpeed(mEclSpeed);
    return houseCuspSigtor;
}

QForecastEvent::QForecastEvent(const QSigtor* sigtor)
    : QObject(nullptr)
    , mSigtor(sigtor)
{
}

QHoraCoords* QForecastEvent::eventBegin() const
{
    return mEventExact;
}

QHoraCoords* QForecastEvent::eventExact() const
{
    return mEventExact;
}

QHoraCoords* QForecastEvent::eventEnd() const
{
    return mEventExact;
}

void QForecastEvent::setEventExact(QHoraCoords* eventExact)
{
    if (mEventExact != eventExact)
    {
        mEventExact = eventExact;
        emit eventBeginChanged();
        emit eventExactChanged();
        emit eventEndChanged();
    }
}

const QPrmsor* QForecastEvent::prmsor() const
{
    return mPrmsor;
}

const QSigtor* QForecastEvent::sigtor() const
{
    return mSigtor;
}

void QForecastEvent::setPrmsor(const QPrmsor* prmsor)
{
    mPrmsor = prmsor;
}

void QForecastEventBuffer::clear()
{
    for (QForecastEvent* event : *this)
    {
        event->deleteLater();
    }
    QList<QForecastEvent*>::clear();
}

void QForecastEventBuffer::insert(QForecastEvent* newEvent)
{
    // insert-sorting based on event's exact time
    iterator insertPos = begin(), eventEnd = end();
    while (insertPos != eventEnd && (*insertPos)->eventExact()->ephTime() < newEvent->eventExact()->ephTime())
    {
        insertPos++;
    }
    QList<QForecastEvent*>::insert(insertPos, newEvent);
}

QForecastEvent* QForecastEventBuffer::pop()
{
    return isEmpty() ? nullptr : takeFirst();
}
