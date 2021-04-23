
#include "astro/hora/setup.h"
#include "astro/hora/qhoraobject.h"
#include "astro/hora/qhoraconfig.h"

QHoraObject::QHoraObject(QObject* parent, const QString& name, QOrbisConfigNodeGetter orbisGetter)
    : QMagObject(parent, name)
    , mConjunctionConfigNode(QHoraConfig::mo()->aspects()->conjunction())
    , mObjectOrbisGetter(orbisGetter)
{
}

QOrbisValue QHoraObject::orbis() const
{
    return aspectOrbis(mConjunctionConfigNode);
}

QOrbisValue QHoraObject::aspectOrbis(const QAspectConfigNode* aspectConfig) const
{
    return aspectConfig != nullptr ? (aspectConfig->orbis()->*mObjectOrbisGetter)()->value().toDouble() : 0.0;
}
