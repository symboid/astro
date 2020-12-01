
#ifndef __SYMBOID_ASTRO_HORA_QPLANET_H__
#define __SYMBOID_ASTRO_HORA_QPLANET_H__

#include "astro/hora/defs.h"
#include "astro/hora/qhoraobject.h"

class QPlanet : public QHoraObject
{
    Q_OBJECT

public:
    static QString resolveId(QOrbisConfigNodeGetter orbisGetter);
    QPlanet(QObject* parent, QOrbisConfigNodeGetter orbisGetter);

public:
    typedef typename eph_proxy::object::index Index;
private:
    static Index resolveIndex(const QString& id);
    const Index mIndex;

public:
    bool calc(const QEphTime& ephTime) override;
};

#endif // __SYMBOID_ASTRO_HORA_QPLANET_H__
