
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

QEclPos QEphObject::eclPos() const
{
    return mEclPos;
}

bool QEphObject::isEclPosValid() const
{
    return mIsEclPosValid;
}

void QEphObject::setEclPos(const QEclPos& eclPos)
{
    mEclPos = eclPos;
    mIsEclPosValid = true;
    emit eclPosChanged();
}

QEclSpeed QEphObject::eclSpeed() const
{
    return mEclSpeed;
}

bool QEphObject::isEclSpeedValid() const
{
    return mIsEclSpeedValid;
}

void QEphObject::setEclSpeed(const QEclSpeed& eclSpeed)
{
    mEclSpeed = eclSpeed;
    mIsEclSpeedValid = true;
    emit eclSpeedChanged();
}
