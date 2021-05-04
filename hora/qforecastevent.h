
#ifndef __SYMBOID_ASTRO_HORA_QFORECASTOBJECT_H__
#define __SYMBOID_ASTRO_HORA_QFORECASTOBJECT_H__

#include "astro/hora/defs.h"
#include <QObject>
#include <QDateTime>
#include "astro/hora/qhoracoords.h"
#include "astro/hora/qaspectobject.h"
#include "astro/hora/qmagobject.h"
#include "astro/hora/qhousecusp.h"

typedef QAspectObject QPrmsor;

class ASTRO_HORA_API QSigtor : public QMagObject
{
    Q_OBJECT

public:
    QSigtor(QObject* parent, const QMagObject* origin, const QString& id);
    virtual QSigtor* clone() const = 0;
private:
    const QMagObject* mOrigin;

public:
    QEclPos eclPos() const override;
    QEclSpeed eclSpeed() const override;
    void setEclPos(const QEclPos& eclPos);
    virtual bool calcEclPos(const QHoraCoords& horaCoords) = 0;
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
    QPlanetSigtor(QObject* parent, const QPlanet* planetOrigin);
    QSigtor* clone() const override;

private:
    const QPlanet* mPlanetOrigin;
public:
    bool calcEclPos(const QHoraCoords& horaCoords) override;
};

class ASTRO_HORA_API QHouseCuspSigtor : public QSigtor
{
    Q_OBJECT

public:
    QHouseCuspSigtor(QObject* parent, const QHouseCusp* houseCuspOrigin);
    QSigtor* clone() const override;

private:
    const QHouseCusp* mHouseCuspOrigin;
public:
    bool calcEclPos(const QHoraCoords& horaCoords) override;
};

class ASTRO_HORA_API QForecastEvent : public QObject
{
    Q_OBJECT

public:
    QForecastEvent(QObject* parent, QSigtor* sigtor = nullptr);

public:
    Q_PROPERTY(QDateTime eventBegin READ eventBegin NOTIFY eventBeginChanged)
    Q_PROPERTY(QDateTime eventExact READ eventExact WRITE setEventExact NOTIFY eventExactChanged)
    Q_PROPERTY(QDateTime eventEnd READ eventEnd NOTIFY eventEndChanged)
    QDateTime eventBegin() const;
    QDateTime eventExact() const;
    QDateTime eventEnd() const;
    void setEventExact(const QDateTime& eventExact);
private:
    QDateTime mEventExact;
signals:
    void eventBeginChanged();
    void eventExactChanged();
    void eventEndChanged();

public:
    const QPrmsor* prmsor() const;
    const QSigtor* sigtor() const;
    QPrmsor* prmsor();
    QSigtor* sigtor();
    void setPrmsor(QPrmsor* prmsor);
    void setSigtor(QSigtor* sigtor);
private:
    QSigtor* mSigtor;
    QPrmsor* mPrmsor;
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
