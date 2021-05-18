
#include "astro/hora/setup.h"
#include "astro/hora/qmagitem.h"

QMagItem::QMagItem(QObject* parent, const QAspectObject* origin, const QAstroFont* font)
    : QMagObject(parent, origin->id())
    , mEclPos(origin->eclPos())
    , mEclSpeed(origin->eclSpeed())
    , mAbbrName(origin->abbrName())
    , mSymbol(origin->symbol(font))
    , mDrawColor(origin->drawColor())
    , mOrbis(origin->orbis())
    , mAspectDist(origin->aspect()->dist())
    , mHoraObjectSymbol(origin->horaObject()->symbol(font))
{
}

QEclPos QMagItem::eclPos() const
{
    return mEclPos;
}

QEclSpeed QMagItem::eclSpeed() const
{
    return mEclSpeed;
}

QString QMagItem::abbrName() const
{
    return mAbbrName;
}

QString QMagItem::symbol(const QAstroFont*) const
{
    return mSymbol;
}

QColor QMagItem::drawColor() const
{
    return mDrawColor;
}

QOrbisValue QMagItem::orbis() const
{
    return mOrbis;
}

qreal QMagItem::aspectDist() const
{
    return mAspectDist;
}

QString QMagItem::horaObjectSymbol() const
{
    return mHoraObjectSymbol;
}

void QMagItemList::clear()
{
    for (QMagItem* magItem : * this)
    {
        magItem->deleteLater();
    }
}

void QMagItemList::insert(QMagItem* aspectObject)
{
    if (aspectObject)
    {
        const QEclLont objectLont = aspectObject->eclPos()._M_lont;
        iterator insertPos = begin(), oEnd = end();
        while (insertPos != oEnd && objectLont > (*insertPos)->eclPos()._M_lont)
        {
            ++insertPos;
        }
        QList<QMagItem*>::insert(insertPos, aspectObject);
    }
}

QMagItemList::Siblings QMagItemList::find(const QEclPos& eclPos) const
{
    Siblings siblings;
    const_iterator succObject = begin(), endObject = end();
    while (succObject != endObject && (*succObject)->eclPos()._M_lont < eclPos._M_lont)
    {
        succObject++;
    }
    if (succObject == begin() || succObject == endObject)
    {
        siblings.mPrec = *rbegin();
        siblings.mSucc = *begin();
    }
    else
    {
        siblings.mPrec = *(succObject - 1);
        siblings.mSucc = *succObject;
    }
    return siblings;
}
