
#ifndef __SYMBOID_ASTRO_HORA_QFORECASTOBJECT_H__
#define __SYMBOID_ASTRO_HORA_QFORECASTOBJECT_H__

#include "astro/hora/defs.h"
#include <QObject>
#include "astro/hora/qhoracoords.h"
#include "astro/hora/qmagitem.h"
#include "astro/hora/qplanet.h"
#include "astro/hora/qhousecusp.h"

typedef QMagItem QPrmsor;

class ASTRO_HORA_API QSigtor : public QMagObject
{
    Q_OBJECT

public:
    QSigtor(const QMagObject* origin, const QString& id);
    virtual QSigtor* clone() const = 0;
private:
    const QMagObject* mOrigin;

public:
    QEclPos eclPos() const override;
    QEclSpeed eclSpeed() const override;
    void setEclPos(const QEclPos& eclPos);
    void setEclSpeed(const QEclSpeed& eclSpeed);
    virtual bool calcEclPos(const QEphTime& ephTime, eph::arc_degree geoLatt, eph::arc_degree geoLont) = 0;
    inline bool calcEclPos(const QHoraCoords& horaCoords)
    {
        return calcEclPos(horaCoords.ephTime(), horaCoords.geoLatt(), horaCoords.geoLont());
    }
protected:
    QEclPos mEclPos;
    QEclSpeed mEclSpeed;

public:
    QString abbrName() const override;
    QString symbol(const QAstroFont* font) const override;
    QColor drawColor() const override;
    QOrbisValue orbis() const override;
};

class ASTRO_HORA_API QPlanetSigtor : public QSigtor
{
    Q_OBJECT

public:
    QPlanetSigtor(const QPlanet* planetOrigin);
    QSigtor* clone() const override;

private:
    const QPlanet* mPlanetOrigin;
public:
    bool calcEclPos(const QEphTime& ephTime, eph::arc_degree geoLatt, eph::arc_degree geoLont) override;
};

class ASTRO_HORA_API QHouseCuspSigtor : public QSigtor
{
    Q_OBJECT

public:
    QHouseCuspSigtor(const QHouseCusp* houseCuspOrigin);
    QSigtor* clone() const override;

private:
    const QHouseCusp* mHouseCuspOrigin;
public:
    bool calcEclPos(const QEphTime& ephTime, eph::arc_degree geoLatt, eph::arc_degree geoLont) override;
};

class ASTRO_HORA_API QForecastEvent : public QObject
{
    Q_OBJECT

public:
    QForecastEvent(const QSigtor* sigtor = nullptr);

public:
    Q_PROPERTY(QHoraCoords* eventBegin READ eventBegin NOTIFY eventBeginChanged)
    Q_PROPERTY(QHoraCoords* eventExact READ eventExact WRITE setEventExact NOTIFY eventExactChanged)
    Q_PROPERTY(QHoraCoords* eventEnd READ eventEnd NOTIFY eventEndChanged)
    QHoraCoords* eventBegin() const;
    QHoraCoords* eventExact() const;
    QHoraCoords* eventEnd() const;
    void setEventExact(QHoraCoords* eventExact);
private:
    QHoraCoords* mEventExact;
signals:
    void eventBeginChanged();
    void eventExactChanged();
    void eventEndChanged();

public:
    const QPrmsor* prmsor() const;
    const QSigtor* sigtor() const;
    void setPrmsor(const QPrmsor* prmsor);
private:
    const QSigtor* mSigtor;
    const QPrmsor* mPrmsor;
};

class ASTRO_HORA_API QForecastEventBuffer : QList<QForecastEvent*>
{
public:
    void clear();
    void insert(QForecastEvent* event);
    QForecastEvent* pop();
    using QList<QForecastEvent*>::isEmpty;
};

#endif // __SYMBOID_ASTRO_HORA_QFORECASTOBJECT_H__
