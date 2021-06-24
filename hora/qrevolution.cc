
#include "astro/hora/setup.h"
#include "astro/hora/qrevolution.h"

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
    mRevolutions.append(Data(new QHoraCoords(mHora->coords()->dateTime().addMonths(1), 2.0), false));
    mRevolutions.append(Data(new QHoraCoords(mHora->coords()->dateTime().addMonths(2), 2.0), false));
    mRevolutions.append(Data(new QHoraCoords(mHora->coords()->dateTime().addMonths(3), 2.0), false));
    mRevolutions.append(Data(new QHoraCoords(mHora->coords()->dateTime().addMonths(4), 2.0), false));
    mRevolutions.append(Data(new QHoraCoords(mHora->coords()->dateTime().addMonths(5), 2.0), false));
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
           planetIndex == 1 ? 13 :
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
