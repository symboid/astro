
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
    QForecastEvent(QObject* parent, QSigtor* sigtor = nullptr);

public:
    Q_PROPERTY(QDateTime eventBegin READ eventBegin NOTIFY eventBeginChanged)
    Q_PROPERTY(QDateTime eventExact READ eventExact WRITE setEventExact NOTIFY eventExactChanged)
    Q_PROPERTY(QDateTime eventEnd READ eventEnd NOTIFY eventEndChanged)
    QDateTime eventBegin() const;
    QDateTime eventExact() const;
    QDateTime eventEnd() const;
    void setEventExact(const QDateTime& eventTime);
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
};

#endif // __SYMBOID_ASTRO_HORA_QFORECASTOBJECT_H__
