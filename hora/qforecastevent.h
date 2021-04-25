
#ifndef __SYMBOID_ASTRO_HORA_QFORECASTOBJECT_H__
#define __SYMBOID_ASTRO_HORA_QFORECASTOBJECT_H__

#include "astro/hora/defs.h"
#include <QObject>
#include <QDateTime>
#include "astro/hora/qmagobject.h"
#include "astro/hora/qplanet.h"

typedef QMagObject QPrmsor;
typedef QMagObject QSigtor;

class ASTRO_HORA_API QForecastEvent : public QObject
{
    Q_OBJECT

public:
    QForecastEvent(QObject* parent);

public:
    Q_PROPERTY(QDateTime eventBegin READ eventBegin NOTIFY eventBeginChanged)
    Q_PROPERTY(QDateTime eventEnd READ eventEnd NOTIFY eventEndChanged)
    QDateTime eventBegin() const;
    QDateTime eventEnd() const;
    void setEventTime(const QDateTime& eventTime);
private:
    QDateTime mEventTime;
signals:
    void eventBeginChanged();
    void eventEndChanged();

public:
    const QPrmsor* prmsor() const;
    const QSigtor* sigtor() const;
    void setPrmsor(const QPrmsor* prmsor);
    void setSigtor(const QSigtor* sigtor);
};

#endif // __SYMBOID_ASTRO_HORA_QFORECASTOBJECT_H__
