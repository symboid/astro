
#include "astro/hora/setup.h"
#include "astro/hora/qhoraobject.h"

QHoraObject::QHoraObject(QObject* parent, const QString& name, QOrbisConfigNodeGetter orbisGetter)
    : QEphObject(parent, name)
    , mObjectOrbisGetter(orbisGetter)
{
}

QOrbisValue QHoraObject::aspectOrbis(const QAspectConfigNode* aspectConfig) const
{
    return aspectConfig != nullptr ? (aspectConfig->orbis()->*mObjectOrbisGetter)()->value().toDouble() : 0.0;
}
