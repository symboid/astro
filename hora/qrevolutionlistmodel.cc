
#include "astro/hora/setup.h"
#include "astro/hora/qrevolutionlistmodel.h"

QRevolutionListModel::QRevolutionListModel(QObject* parent)
    : QCalcListModel(parent)
    , mRevolutionCalc(nullptr)
{
}

QRevolution* QRevolutionListModel::revolutionCalc() const
{
    return mRevolutionCalc;
}

void QRevolutionListModel::setRevolutionCalc(QRevolution* revolutionCalc)
{
    if (mRevolutionCalc != revolutionCalc)
    {
        mRevolutionCalc = revolutionCalc;
        setCalcable(revolutionCalc);
        emit revolutionCalcChanged();
    }
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
    return mRevolutionCalc ? mRevolutionCalc->revolutionsCount() : 0;
}

QVariant QRevolutionListModel::data(const QModelIndex& index, int role) const
{
    int rIndex = index.row();
    QVariant revData;
    if (mRevolutionCalc && 0 <= rIndex && rIndex < mRevolutionCalc->revolutionsCount())
    {
        const QRevolution::Data& data = mRevolutionCalc->revolutionData(rIndex);
        switch (role)
        {
        case DateTimeRole: revData = QVariant::fromValue(data.mCoords.get()); break;
        case RetrogradRole: revData = data.mIsRetrograd; break;
        default: revData = data.mCoords->dateTime().toString(QLocale().dateFormat(QLocale::ShortFormat)); break;
        }
    }
    return revData;
}
