
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

QString QAspectObject::symbol(const QAstroFont* font) const
{
    return font->aspectLetter(int(mAspect->dist())) + mHoraObject->symbol(font);
}

QColor QAspectObject::drawColor() const
{
    return mAspect->draw()->lineColor();
}

void QAspectObjectList::insert(QAspectObject* aspectObject)
{
    if (aspectObject)
    {
        const QEclPos objectPos = aspectObject->eclPos();
        iterator insertPos = begin(), oEnd = end();
        while (insertPos != oEnd && objectPos < (*insertPos)->eclPos())
        {
            ++insertPos;
        }
        QList<QAspectObject*>::insert(insertPos, aspectObject);
    }
}

QAspectObjectList::Siblings QAspectObjectList::find(const QAspectObject* aspectObject)
{
    Siblings siblings;
    if (aspectObject)
    {
        const QEclPos objectPos = aspectObject->eclPos();
        iterator precObject = begin(), endObject = end();
        while (precObject != endObject && (*precObject)->eclPos()._M_lont < objectPos._M_lont)
        {
            precObject++;
        }
        if (precObject != endObject && (precObject+1) != endObject)
        {
            siblings.mPrec = *precObject;
            siblings.mSucc = *(precObject+1);
        }
        else
        {
            siblings.mPrec = *rbegin();
            siblings.mSucc = *begin();
        }
    }
    return siblings;
}
