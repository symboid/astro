
#include "astro/hora/setup.h"
#include "astro/hora/qaspectconfig.h"
#include "astro/hora/qhoraobject.h"

const QAspectConfigNode* QAspectConfig::findConnection(const QHoraObject* leftObject, const QHoraObject* rightObject) const
{
    const QAspectConfigNode* aspectConnection = nullptr;
    QOrbisValue dist = leftObject->eclPos().dist_abs(rightObject->eclPos()).to_arc_degree();
    for (int a = 0, aCount = subConfigCount(); aspectConnection == nullptr && a < aCount; ++a)
    {
        if (const QAspectConfigNode* aspectConfig = dynamic_cast<const QAspectConfigNode*>(subConfig(a)))
        {
            double aspectDist = aspectConfig->dist();
            QOrbisValue aspectOrbis = leftObject->aspectOrbis(aspectConfig) + rightObject->aspectOrbis(aspectConfig);
            QOrbisValue distMin = dist - aspectOrbis, distMax = dist + aspectOrbis;
            if (distMin < aspectDist && aspectDist < distMax)
            {
                aspectConnection = aspectConfig;
            }
        }
    }
    return aspectConnection;
}
