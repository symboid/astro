
#include "astro/hora/setup.h"
#include "astro/hora/qrevolution.h"

QRevolutionListModel::QRevolutionListModel(QObject* parent)
    : QAbstractListModel(parent)
{
}

QHash<int, QByteArray> QRevolutionListModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles[DateTimeRole] = "datetime";
    roles[RetrogradRole] = "retrograd";
    return roles;
}

int QRevolutionListModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return mRevolutions.size();
}

QVariant QRevolutionListModel::data(const QModelIndex& index, int role) const
{
    int rIndex = index.row();
    QVariant revData;
    if (0 <= rIndex && rIndex < mRevolutions.size())
    {
        const RevolutionData& data = mRevolutions[rIndex];
        switch (role)
        {
        case DateTimeRole: revData = QVariant::fromValue(data.mCoords.get()); break;
        case RetrogradRole: revData = data.mIsRetrograd; break;
        default: revData = data.mCoords->dateTime().toString(QLocale().dateFormat(QLocale::ShortFormat)); break;
        }
    }
    return revData;
}

void QRevolutionListModel::clear()
{
    mRevolutions.clear();
}

void QRevolutionListModel::append(const QSharedPointer<QHoraCoords>& coords, bool isRetrograd)
{
    RevolutionData revData;
    revData.mCoords = coords;
    revData.mIsRetrograd = isRetrograd;
    mRevolutions.push_back(revData);
}

QRevolution::QRevolution(QObject* parent)
    : QCalcable(parent)
    , mHora(nullptr)
    , mPlanetModel({ tr("Sun"), tr("Moon"), tr("Mercury"), tr("Venus"), tr("Mars"), tr("Jupiter"), tr("Saturn") })
    , mPlanetIndex(0)
    , mList(new QRevolutionListModel(this))
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
    if ((mHora = hora))
    {
        connect(mHora, SIGNAL(changed()), this, SIGNAL(planetLontChanged()));
    }
    emit horaChanged();
}

void QRevolution::calc()
{
    mList->clear();
}

const QStringList& QRevolution::planetModel() const
{
    return mPlanetModel;
}

int QRevolution::planetIndex() const
{
    return mPlanetIndex;
}

QString QRevolution::planetName() const
{
    return 0 <= mPlanetIndex && mPlanetIndex < mPlanetModel.length() ? mPlanetModel.at(mPlanetIndex) : "";
}

void QRevolution::setPlanetIndex(int planetIndex)
{
    if (mPlanetIndex != planetIndex)
    {
        mPlanetIndex = planetIndex;
        emit planetIndexChanged();
    }
}

qreal QRevolution::planetLont() const
{
    return mHora && 0 <= mPlanetIndex && mPlanetIndex < mPlanetModel.length() ?
            mHora->planet(mPlanetIndex)->eclPos()._M_lont.to_arc_degree() : 0.0;
}

int QRevolution::defaultRevCount() const
{
    return mPlanetIndex == 0 ? 1 :
           mPlanetIndex == 1 ? 13 :
                               3;
}

QAbstractListModel* QRevolution::list() const
{
    return mList;
}
