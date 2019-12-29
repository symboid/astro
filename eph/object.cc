
#include "astro/ephe/setup.h"
#include "astro/ephe/object.h"
#include <cstring>

namespace As {

struct ObjectTableRow
{
    Object::Index mSwephIndex;
    const char* mName;
    const char* mAbr;
};

static ObjectTableRow sObjectTable[O_COUNT + 1] = {
    { SE_EARTH,     "???",      "???" },

    { SE_SUN,       "Sun",      "Sun" },
    { SE_MOON,      "Moon",     "Mon" },
    { SE_MERCURY,   "Mercury",  "Mer" },
    { SE_VENUS,     "Venus",    "Ven" },
    { SE_MARS,      "Mars",     "Mar" },
    { SE_JUPITER,   "Jupiter",  "Jup" },
    { SE_SATURN,    "Saturn",   "Sat" },

    { SE_URANUS,    "Uranus",   "Urn" },
    { SE_NEPTUNE,   "Neptune",  "Nep" },
    { SE_PLUTO,     "Pluto",    "Plu" },

    { SE_MEAN_APOG, "Lilith",   "Lil" },
    { SE_CHIRON,    "Chiron",   "Chi" }
};

static inline const ObjectTableRow& objectRow(ObjectId objectId)
{
    int rowIndex = int(objectId);
    return sObjectTable[0 < rowIndex && rowIndex <= int(O_COUNT) ? rowIndex : 0];
}

Object::Object(ObjectId objectId)
    : mId(objectId)
{
}

Sy::String Object::name() const
{
    return objectRow(mId).mName;
}

Sy::String Object::abr() const
{
    return objectRow(mId).mAbr;
}

Object::Index Object::index() const
{
    return objectRow(mId).mSwephIndex;
}

void Object::setEpheDirPath(const Sy::FilePath& epheDirPath)
{
    char pathBuffer[AS_MAXCH];
    std::strncpy(pathBuffer, epheDirPath.toString().c_str(), AS_MAXCH);
    swe_set_ephe_path(pathBuffer);
}

CalcResult Object::calcPos(const Time& julTime, CalcType calcType)
{
    char errorStr[AS_MAXCH];
    double epheData[6];

    // setting up ephemeris type
    int32 typeFlag = (calcType == CalcType::JPL ? SEFLG_JPLEPH : calcType == CalcType::MOSHIER ? SEFLG_MOSEPH : SEFLG_SWIEPH);

    // SEFLG_SPEED = asking for speed calculation
    int32 calcFlag = typeFlag | SEFLG_SPEED;

    int retFlag = swe_calc_ut(julTime.toScalar(), objectRow(mId).mSwephIndex, calcFlag, epheData, errorStr);
    if (retFlag < 0)
    {
//        sy_error("%s", errStr);
        mEclPos = EclPos(0,0,0);
        mEclSpeed = EclSpeed(0,0,0);
    }
    else
    {
        if (retFlag != calcFlag)
        {
//            sy_log_once(SY_WARNING, errStr);
        }
        mEclPos = EclPos(epheData[0], epheData[1], epheData[2]);
        mEclSpeed = EclSpeed(epheData[3], epheData[4], epheData[5]);
    }

    return retFlag >= 0 ? CALC_SUCCESS : CALC_ERROR;
}

bool Object::isRetrograd() const
{
    return mEclSpeed.mLont < 0.0;
}

Time Object::approxTransitPos(const Time& startTime,
        const EclPos& precPos, const EclPos& succPos)
{
    Time conjTime = startTime;

    EclPos::Lont DIST_TOLERANCE = 1.0 / 3600.0;

    while (calcPos(conjTime) == CALC_SUCCESS &&
           mEclPos.distAbs(precPos) > DIST_TOLERANCE &&
           mEclPos.distAbs(succPos) > DIST_TOLERANCE)
    {
        // speed of object at current position (measured in degrees/day):
        EclSpeed::Lont speed = mEclSpeed.mLont;
        JulTime::Type estmDays = 1.0;

        // approx forward to succeeding position:
        if (speed > 0.0)
        {
            // distance to succeeding position in ecliptic degree:
            EclCoords::Lont succDist = succPos.distTo(mEclPos);

            // linear estimation of remaining days of transit:
            estmDays = succDist / speed;
        }

        // approx backward to preceeding position:
        else
        {
            // distance to preceding position in ecliptic degree:
            EclCoords::Lont precDist = precPos.distTo(mEclPos);

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
        conjTime.add(estmDays);
    }

    return conjTime;
}

CalcResult Object::approxTransitPos(Time& conjTime, const EclPos& transitPos)
{
    EclPos::Lont DIST_TOLERANCE = 1.0 / 3600.0;

    CalcResult calcResult = CALC_ERROR;
    while ((calcResult = calcPos(conjTime)) == CALC_SUCCESS && mEclPos.distAbs(transitPos) > DIST_TOLERANCE)
    {
        // speed of object at current position (measured in degrees/day):
        EclSpeed::Lont speed = mEclSpeed.mLont;

        // distance to transit position in ecliptic degree:
        EclCoords::Lont transitDist = transitPos.distTo(mEclPos);
        JulTime::Type estmDays = transitDist / speed;

        if (estmDays > 1.0)
        {
            estmDays = 1.0;
        }
        else if (estmDays < -1.0)
        {
            estmDays = -1.0;
        }

        // stepping forward in time with estimated amount of days:
        conjTime.add(estmDays);
    }

    return calcResult;
}

CalcResult Object::approxTransitRaw(Time& rawTime, const EclPos& transitPos)
{
    CalcResult calcResult = CALC_ERROR;
    EclCoords::Dist prevDist = 360.0;
    EclCoords::Dist distFwd = 0;//mEclPos.distFwd(transitPos);
    // TODO: retrograd !!!
    while ((calcResult = calcPos(rawTime)) == CALC_SUCCESS && prevDist > (distFwd = mEclPos.distFwd(transitPos)))
    //while (prevDist > distFwd)
    {
        // speed of object at current position (measured in degrees/day):
        JulTime::Type estmDays = 1.0;

        // stepping forward in time with estimated amount of days:
        rawTime.add(estmDays);

        prevDist = distFwd;

        // recalculation of transit position:
        //successCalc = calcPos(signTime);
        //distFwd = mEclPos.distFwd(transitPos);
    }

    return calcResult;
}

CalcResult Object::calcTransitTime(Time& transitTime, const EclPos& transitPos) const
{
    Object transitObject(mId);
    return transitObject.approxTransitRaw(transitTime, transitPos) == CALC_SUCCESS
        && transitObject.approxTransitPos(transitTime, transitPos) == CALC_SUCCESS
         ? CALC_SUCCESS : CALC_ERROR;
}

} // namespace As
