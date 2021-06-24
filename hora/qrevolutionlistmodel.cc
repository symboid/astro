
#include "astro/hora/setup.h"
#include "astro/hora/qrevolutionlistmodel.h"

QCalcListModel::QCalcListModel(QObject* parent)
    : QAbstractListModel(parent)
    , mCalcable(nullptr)
{
}

QCalcable* QCalcListModel::calcable() const
{
    return mCalcable;
}

void QCalcListModel::setCalcable(QCalcable* calcable)
{
    if (mCalcable != calcable)
    {
        if (mCalcable)
        {
            disconnect(mCalcable, SIGNAL(calcTaskChanged()), this, SLOT(connectCalcSignals()));
        }
        if ((mCalcable = calcable))
        {
            connect(mCalcable, SIGNAL(calcTaskChanged()), this, SLOT(connectCalcSignals()));
            connectCalcSignals();
        }
    }
}

void QCalcListModel::connectCalcSignals()
{
    if (QCalcTask* calcTask = mCalcable->calcTask())
    {
        connect(calcTask, SIGNAL(started()), this, SLOT(onRecalcStarted()));
        connect(calcTask, SIGNAL(finished()), this, SLOT(onRecalcFinished()));
        connect(calcTask, SIGNAL(aborted()), this, SLOT(onRecalcAborted()));
    }
}

void QCalcListModel::onRecalcStarted()
{
    beginResetModel();
}

void QCalcListModel::onRecalcFinished()
{
    endResetModel();
}

void QCalcListModel::onRecalcAborted()
{
    endResetModel();
}

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
