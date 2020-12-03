
#include "astro/hora/setup.h"
#include "astro/hora/qaspectconfig.h"
#include "astro/hora/qhoraobject.h"

QOrbisConfigNode::QOrbisConfigNode(const QString& id, QAbstractConfig* parent, const char* parentSignal,
        const double* orbisDefaults)
    : QConfigNode(id, parent, parentSignal)
    , mOrbisDefaults(orbisDefaults)
{
}

QAspectProperties::QAspectProperties(bool isMain, eph::ecl_pos::dist dist, const QColor& lineColor, double lineWidth)
  : mIsMain(isMain)
  , mDist(dist)
  , mLineColor(lineColor)
  , mLineWidth(lineWidth)
{
}

QAspectDrawConfig::QAspectDrawConfig(const QString& id, QAbstractConfig* parent, const char* parentSignal,
        double lineWidth, const QColor& lineColor)
    : QConfigNode(id, parent, parentSignal)
    , mLineWidthDefault(lineWidth)
    , mLineColorDefault(lineColor)
{
}

QAspectConfigNode::QAspectConfigNode(const QString& id, QAbstractConfig* parent, const char* parentSignal,
        const QAspectProperties& properties, bool enabledDefault, const double* orbisDefaults)
    : QConfigNode(id, parent, parentSignal)
    , mProperties(properties)
    , mEnabledDefault(enabledDefault)
    , mOrbisDefaults(orbisDefaults)
{
}

QAspectConfig::QAspectConfig(const QString& id, QAbstractConfig* parentNode, const char* parentSignal)
    : QConfigContainer<QAspectConfigNode>(id, parentNode, parentSignal)
{
}

const QAspectConfigNode* QAspectConfig::findConnection(const QHoraObject* leftObject, const QHoraObject* rightObject) const
{
    const QAspectConfigNode* aspectConnection = nullptr;
    QOrbisValue dist = leftObject->eclPos().dist_abs(rightObject->eclPos()).to_arc_degree();
    for (int a = 0, aCount = subConfigCount(); aspectConnection == nullptr && a < aCount; ++a)
    {
        if (const QAspectConfigNode* aspectConfig = mSubConfigs[a])
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
