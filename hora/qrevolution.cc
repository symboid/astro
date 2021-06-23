
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

void QRevolutionListModel::startUpdate()
{
    beginResetModel();
    mRevolutions.clear();
}

void QRevolutionListModel::append(const QSharedPointer<QHoraCoords>& coords, bool isRetrograd)
{
    RevolutionData revData;
    revData.mCoords = coords;
    revData.mIsRetrograd = isRetrograd;
    mRevolutions.push_back(revData);
}

void QRevolutionListModel::finishUpdate()
{
    endResetModel();
}

QRevolution::QRevolution(QObject* parent)
    : QCalcable(parent)
    , mHora(nullptr)
    , mPlanetModel({ tr("Sun"), tr("Moon"), tr("Mercury"), tr("Venus"), tr("Mars"), tr("Jupiter"), tr("Saturn") })
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
    mList->startUpdate();
    mList->append(QSharedPointer<QHoraCoords>(new QHoraCoords(QDateTime::currentDateTime().addMonths(1), 2.0)), true);
    mList->append(QSharedPointer<QHoraCoords>(new QHoraCoords(QDateTime::currentDateTime().addMonths(2), 2.0)), true);
    mList->append(QSharedPointer<QHoraCoords>(new QHoraCoords(QDateTime::currentDateTime().addMonths(3), 2.0)), true);
    mList->append(QSharedPointer<QHoraCoords>(new QHoraCoords(QDateTime::currentDateTime().addMonths(4), 2.0)), true);
    mList->append(QSharedPointer<QHoraCoords>(new QHoraCoords(QDateTime::currentDateTime().addMonths(5), 2.0)), true);
    mList->finishUpdate();
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

QAbstractListModel* QRevolution::list() const
{
    return mList;
}
