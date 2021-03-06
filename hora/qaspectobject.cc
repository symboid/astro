
#include "astro/hora/setup.h"
#include "astro/hora/qaspectobject.h"

QAspectObject::QAspectObject(QHoraObject* horaObject, QAspectConfigNode* aspect, bool isUpper)
    : QMagObject(horaObject, horaObject->id() + "-" + aspect->mId)
    , mHoraObject(horaObject)
    , mAspect(aspect)
    , mIsUpper(isUpper)
{
}

const QHoraObject* QAspectObject::horaObject() const
{
    return mHoraObject;
}

const QAspectConfigNode* QAspectObject::aspect() const
{
    return mAspect;
}

QEclPos QAspectObject::eclPos() const
{
    return mHoraObject->eclPos()._M_lont + (mIsUpper ? -1 : 1) * mAspect->dist();
}

QEclSpeed QAspectObject::eclSpeed() const
{
    return mHoraObject->eclSpeed();
}

QOrbisValue QAspectObject::orbis() const
{
    return mHoraObject->aspectOrbis(mAspect);
}

QString QAspectObject::abbrName() const
{
    return mAspect->abbrName() + "-" + mHoraObject->abbrName();
}

QString QAspectObject::symbol(const QAstroFont* font) const
{
    return font->aspectLetter(int(mAspect->dist())) + mHoraObject->symbol(font);
}

QColor QAspectObject::drawColor() const
{
    return mAspect->draw()->lineColor();
}
