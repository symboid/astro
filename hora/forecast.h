
#ifndef __SYMBOID_ASTRO_HORA_FORECAST_H__
#define __SYMBOID_ASTRO_HORA_FORECAST_H__

#include "astro/hora/defs.h"
#include "astro/calculo/hora.h"
#include "astro/controls/qastrofont.h"
#include "astro/hora/magitem.h"

hor_ns_begin
typedef basic_hora<eph_proxy> hora;
hor_ns_end

eph_ns_begin
typedef basic_time_point<eph_proxy> time_point;
eph_ns_end

class ForecastModel;

class ForcEvent
{
public:
    ForcEvent(const MagItem* sgtor, const eph::time_point& initTime);

public:
    const MagItem* sgtor() const;
    const MagItem* pmsor() const;
    inline hor::aspect_type aspectType() const
    {
        const MagItem* pmsorItem = pmsor();
        return pmsorItem != nullptr ? pmsorItem->aspectType() : hor::aspect_type::none_aspect;
    }

public:
    eph::time_point exactTimePos() const;
    eph::time_point beginTimePos() const;
    eph::time_point endTimePos() const;
    virtual bool lt(const ForcEvent* rhs) const = 0;

public:
    QString debugStr(bool isShort = false) const;
    QString symbolStr(QAstroFont* symbolFont) const;
    void calc(ForecastModel* forcModel, MagItems& pmsors);

private:
    MagItem* mSgtor;
    eph::time_point mExactTime;
    MagItem* mPmsor;
    eph::time_point mBeginTime;
    eph::time_point mEndTime;
};

class ForecastModel
{
public:
    virtual ~ForecastModel() = default;

public:
    virtual eph::ecl_pos initSgtorPos(MagItem* sgtor, const eph::time_point& startTime) = 0;
    virtual eph::time_point calcConj(MagItem* sgtor, const eph::time_point& startTime,
            const eph::ecl_pos& precPos, const eph::ecl_pos& succPos) = 0;
    virtual eph::time_point calcOrbisTime(const MagItem* sgtor, const MagItem* pmsor,
            const eph::time_point& exactTime, bool calcBeginTime) = 0;
    virtual void init(const hor::hora* hora) = 0;
    typedef std::vector<MagItem*> Sgtors;
    virtual Sgtors sgtors(const hor::hora* hora) const = 0;
};

class Forecast
{
public:
    Forecast(const hor::hora* radixHora);

public:
    void addAspectType(hor::aspect_type aspectType);

public:
    typedef QList<ForcEvent*> EventList;
    EventList calc(ForecastModel* forcModel, const eph::time_point& periodBegin,
            const eph::time_point& periodEnd);

private:
    void collectPmsors();
    EventList initEventList(ForecastModel* forcModel,
            const eph::time_point& periodBegin);

private:
    const hor::hora* mRadixHora;
    MagItems mPmsors;
    std::set<hor::aspect_type> mAspectTypes;
};

class ForecastTransit : public ForecastModel
{
public:
    enum Type
    {
        GENERIC_TRANSIT = 0,
        MOON_TRANSIT,
        SLOW_PLANETS_TRANSIT,
    };

public:
    ForecastTransit(Type type);

public:
    eph::ecl_pos initSgtorPos(MagItem* sgtor, const eph::time_point& startTime) override;
    eph::time_point calcConj(MagItem* sgtor, const eph::time_point& startTime,
            const eph::ecl_pos& precPos, const eph::ecl_pos& succPos) override;
    eph::time_point calcOrbisTime(const MagItem* sgtor, const MagItem* pmsor,
            const eph::time_point& exactTime, bool calcBeginTime) override;
    void init(const hor::hora* hora) override;
    Sgtors sgtors(const hor::hora* hora) const override;

private:
    Type mType;

private:
    bool isPlanetSgtor(const hor::planet& planet) const;
};

class ForecastPriDirex : public ForecastModel
{
public:
    ForecastPriDirex();

public:
    eph::ecl_pos initSgtorPos(MagItem* sgtor, const eph::time_point& startTime) override;
    eph::time_point calcConj(MagItem* sgtor, const eph::time_point& startTime,
            const eph::ecl_pos& precPos, const eph::ecl_pos& succPos) override;
    eph::time_point calcOrbisTime(const MagItem* sgtor, const MagItem* pmsor,
            const eph::time_point& exactTime, bool calcBeginTime) override;
    void init(const hor::hora* hora) override;
    Sgtors sgtors(const hor::hora* hora) const override;

private:
    eph::time_point mRadixTime;
};

class ForecastSecDirex : public ForecastModel
{
public:
    eph::ecl_pos initSgtorPos(MagItem* sgtor, const eph::time_point& startTime) override;
    eph::time_point calcConj(MagItem* sgtor, const eph::time_point& startTime,
            const eph::ecl_pos& precPos, const eph::ecl_pos& succPos) override;
    eph::time_point calcOrbisTime(const MagItem* sgtor, const MagItem* pmsor,
            const eph::time_point& exactTime, bool calcBeginTime) override;
    void init(const hor::hora* hora) override;
    Sgtors sgtors(const hor::hora* hora) const override;

private:
    eph::time_point realToImg(const eph::time_point& realTime);
    eph::time_point imgToReal(const eph::time_point& imgTime);

private:
    eph::time_point mRadixTime;
};

#endif // __SYMBOID_ASTRO_HORA_FORECAST_H__
