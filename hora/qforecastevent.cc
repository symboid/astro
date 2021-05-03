
#include "astro/hora/setup.h"
#include "astro/hora/qforecastevent.h"

QSigtor::QSigtor(QMagObject* origin, const QString& id)
    : QMagObject(origin, id)
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

QPlanetSigtor::QPlanetSigtor(QPlanet* planetOrigin)
    : QSigtor(planetOrigin, planetOrigin->id())
    , mPlanetOrigin(planetOrigin)
{
}

QSigtor* QPlanetSigtor::clone() const
{
    return new QPlanetSigtor(mPlanetOrigin);
}

bool QPlanetSigtor::calcEclPos(const QHoraCoords& horaCoords)
{
    QEclPos eclPos;
    QEclSpeed eclSpeed;
    QPlanet::Index objectIndex = mPlanetOrigin->mIndex == QLunarNode::DRAGON_HEAD || mPlanetOrigin->mIndex == QLunarNode::DRAGON_TAIL ?
                QPlanet::MEAN_NODE : mPlanetOrigin->mIndex;
    bool isSuccess = (eph_proxy::object::calc_pos(objectIndex, horaCoords.ephTime(), eclPos, eclSpeed) == eph::calc_result::SUCCESS);
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

QHouseCuspSigtor::QHouseCuspSigtor(QHouseCusp* houseCuspOrigin)
    : QSigtor(houseCuspOrigin, houseCuspOrigin->id())
    , mHouseCuspOrigin(houseCuspOrigin)
{
}

bool QHouseCuspSigtor::calcEclPos(const QHoraCoords& horaCoords)
{
    bool calcResult(false);
    static constexpr typename eph::basic_calendar<eph_proxy>::days TIME_DIFF(10.0 / 1440.0);
    QEclLont houseCuspLonts[QHouseSystem::HOUSE_COUNT + 1], houseCuspLontsNext[QHouseSystem::HOUSE_COUNT + 1];
    QHouseSystem::Type hsType = mHouseCuspOrigin->mHouseSystem->mType;

    if (eph_proxy::houses::calc(horaCoords.ephTime(), eph_proxy::houses::type(hsType), horaCoords.geoLont(), horaCoords.geoLatt(), houseCuspLonts) == eph::calc_result::SUCCESS &&
        eph_proxy::houses::calc(horaCoords.ephTime() + TIME_DIFF, eph_proxy::houses::type(hsType), horaCoords.geoLont(), horaCoords.geoLatt(), houseCuspLontsNext) == eph::calc_result::SUCCESS)
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
    return new QHouseCuspSigtor(mHouseCuspOrigin);
}

QForecastEvent::QForecastEvent(QObject* parent, QSigtor* sigtor)
    : QObject(parent)
    , mSigtor(sigtor)
{
}

QDateTime QForecastEvent::eventBegin() const
{
    return mEventExact;
}

QDateTime QForecastEvent::eventExact() const
{
    return mEventExact;
}

QDateTime QForecastEvent::eventEnd() const
{
    return mEventExact;
}

void QForecastEvent::setEventExact(const QDateTime& eventExact)
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

QPrmsor* QForecastEvent::prmsor()
{
    return mPrmsor;
}

QSigtor* QForecastEvent::sigtor()
{
    return mSigtor;
}

void QForecastEvent::setPrmsor(QPrmsor* prmsor)
{
    mPrmsor = prmsor;
}

void QForecastEvent::setSigtor(QSigtor* sigtor)
{
    mSigtor = sigtor;
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
    while (insertPos != eventEnd && (*insertPos)->eventExact() < newEvent->eventExact())
    {
        insertPos++;
    }
    QList<QForecastEvent*>::insert(insertPos, newEvent);
}

QForecastEvent* QForecastEventBuffer::pop()
{
    return isEmpty() ? nullptr : takeFirst();
}
