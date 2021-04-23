
#ifndef __SYMBOID_ASTRO_HORA_QASPECTOBJECT_H__
#define __SYMBOID_ASTRO_HORA_QASPECTOBJECT_H__

#include "astro/hora/defs.h"
#include "astro/hora/qmagobject.h"
#include "astro/hora/qplanet.h"

class QAspectObject : public QMagObject
{
    Q_OBJECT

public:
    QAspectObject(QPlanet* planet, QAspectConfigNode* aspect, bool isUpper = false);

public:
    const QPlanet* const mPlanet;
    const QAspectConfigNode* const mAspect;
    const bool mIsUpper;

public:
    QEclPos eclPos() const override;
    QEclSpeed eclSpeed() const override;

public:
    QOrbisValue orbis() const override;
};


class QUpperAspectObject : public QAspectObject
{
    Q_OBJECT

public:
    QUpperAspectObject(QPlanet* planet, QAspectConfigNode* aspect)
        : QAspectObject(planet, aspect, true)
    {
    }
};


class QLowerAspectObject : public QAspectObject
{
    Q_OBJECT

public:
    QLowerAspectObject(QPlanet* planet, QAspectConfigNode* aspect)
        : QAspectObject(planet, aspect, false)
    {
    }
};

#endif // __SYMBOID_ASTRO_HORA_QASPECTOBJECT_H__
