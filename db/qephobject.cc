
#include "astro/db/setup.h"
#include "astro/db/qephobject.h"

QEphObject::QEphObject(QObject* parent, const QString& id)
    : QObject(parent)
    , mId(id)
{
}

QEphObject::~QEphObject()
{
}

QString QEphObject::id() const
{
    return mId;
}
