
#include "astro/hora/setup.h"
#include "astro/hora/qmagobject.h"

QMagObject::QMagObject(QObject* parent, const QString& id)
    : QEphObject(parent, id)
{
}

void QMagObjectList::insertMagObject(QMagObject* magObject)
{
    if (magObject)
    {
        const QEclPos objectPos = magObject->eclPos();
        iterator insertPos = begin(), oEnd = end();
        while (insertPos != oEnd && objectPos < (*insertPos)->eclPos())
        {
            ++insertPos;
        }
        insert(insertPos, magObject);
    }
}

QMagObjectList::Siblings QMagObjectList::find(const QMagObject* magObject)
{
    Siblings siblings;
    if (magObject)
    {
        const QEclPos objectPos = magObject->eclPos();
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
