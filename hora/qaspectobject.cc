
#include "astro/hora/setup.h"
#include "astro/hora/qaspectobject.h"

QAspectObject::QAspectObject(QPlanet* planet, QAspectConfigNode* aspect, bool isUpper)
    : QMagObject(planet, planet->id() + "-" + aspect->mId)
    , mPlanet(planet)
    , mAspect(aspect)
    , mIsUpper(isUpper)
{
}

QEclPos QAspectObject::eclPos() const
{
    return mPlanet->eclPos()._M_lont + (mIsUpper ? -1 : 1) * mAspect->dist();
}

QEclSpeed QAspectObject::eclSpeed() const
{
    return mPlanet->eclSpeed();
}

QOrbisValue QAspectObject::orbis() const
{
    return mPlanet->aspectOrbis(mAspect);
}

QString QAspectObject::symbol(const QAstroFont* font) const
{
    return font->aspectLetter(int(mAspect->dist())) + mPlanet->symbol(font);
}

QColor QAspectObject::drawColor() const
{
    return mAspect->draw()->lineColor();
}
