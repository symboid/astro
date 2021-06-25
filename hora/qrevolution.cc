
#include "astro/hora/setup.h"
#include "astro/hora/qrevolution.h"
#include "astro/hora/qrevolutioncalcmodel.h"
#include "astro/hora/qhoraconfig.h"

QRevolution::QRevolution(QObject* parent)
    : QCalcable(parent)
    , mHora(nullptr)
    , mPlanetModel({ tr("Sun"), tr("Moon"), tr("Mercury"), tr("Venus"), tr("Mars"), tr("Jupiter"), tr("Saturn") })
{
    connect(this, SIGNAL(planetIndexChanged()), this, SIGNAL(planetLontChanged()));
}

QRevolution::~QRevolution()
{
}

QHora* QRevolution::hora() const
{
    return mHora;
}

void QRevolution::setHora(QHora* hora)
{
    if (mHora)
    {
        disconnect(mHora, SIGNAL(changed()), this, SIGNAL(planetLontChanged()));
        deleteParam(mHora);
    }
    if ((mHora = hora))
    {
        connect(mHora, SIGNAL(changed()), this, SIGNAL(planetLontChanged()));
        addParam(mHora);
    }
    emit horaChanged();
}

void QRevolution::calc()
{
    mRevolutions.clear();

    static const QOrbisConfigNodeGetter planetConfigs[10] =
    {
        &QOrbisConfigNode::sunNode, &QOrbisConfigNode::monNode, &QOrbisConfigNode::merNode,
        &QOrbisConfigNode::venNode, &QOrbisConfigNode::marNode, &QOrbisConfigNode::jupNode,
        &QOrbisConfigNode::satNode, &QOrbisConfigNode::uraNode, &QOrbisConfigNode::nepNode,
        &QOrbisConfigNode::pluNode
    };

    QHoraConfig::mo horaConfig;
    QScopedPointer<QRevolutionCalcModel> calcModel(new QRevolutionCalcModel(planetConfigs[*m_planetIndex], planetLont()));

    QSharedPointer<QHoraCoords> targetCoords(new QHoraCoords(QDateTime(QDate(*m_year, 1, 1), QTime(0,0)), *m_revTzDiff));
    targetCoords = calcModel->approx(targetCoords.get());
    mRevolutions.append(Data(new QHoraCoords(targetCoords->dateTime(), *m_revTzDiff), false));

    for (int revIndex = 1; revIndex < *m_revCount; ++revIndex)
    {
        targetCoords->setDateTime(targetCoords->dateTime().addDays(1));

        targetCoords = calcModel->approx(targetCoords.get());
        mRevolutions.append(Data(new QHoraCoords(targetCoords->dateTime(), *m_revTzDiff), false));
    }
}

const QStringList& QRevolution::planetModel() const
{
    return mPlanetModel;
}

QString QRevolution::planetName() const
{
    int planetIndex = planetIndexGet();
    return 0 <= planetIndex && planetIndex < mPlanetModel.length() ? mPlanetModel.at(planetIndex) : "";
}

qreal QRevolution::planetLont() const
{
    int planetIndex = planetIndexGet();
    return mHora && 0 <= planetIndex && planetIndex < mPlanetModel.length() ?
            mHora->planet(planetIndex)->eclPos()._M_lont.to_arc_degree() : 0.0;
}

int QRevolution::defaultRevCount() const
{
    int planetIndex = planetIndexGet();
    return planetIndex == 0 ? 1 :
           planetIndex == 1 ? 14 :
                              3;
}

int QRevolution::revolutionsCount() const
{
    return mRevolutions.count();
}

const QRevolution::Data& QRevolution::revolutionData(int index) const
{
    return mRevolutions.at(index);
}
