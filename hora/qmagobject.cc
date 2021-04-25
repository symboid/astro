
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
