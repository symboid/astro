
#include "astro/hora/setup.h"
#include "astro/hora/qmagobject.h"

QMagObject::QMagObject(QObject* parent, const QString& id)
    : QEphObject(parent, id)
{
}

void QMagObjectList::insertMagObject(QMagObject* magObject)
{
    push_back(magObject);
}
