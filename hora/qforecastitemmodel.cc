
#include "astro/hora/setup.h"
#include "astro/hora/qforecastitemmodel.h"

QForecastItemModel::QForecastItemModel(QObject* parent)
    : QHoraTableModel(Q_NULLPTR, parent)
{
    connect(this, SIGNAL(periodBeginChanged()), this, SLOT(recalc()));
    connect(this, SIGNAL(periodEndChanged()), this, SLOT(recalc()));
}

int QForecastItemModel::rowCount(const QModelIndex& parent) const
{
    return 1;
}

QVariant QForecastItemModel::data(const QModelIndex& index, int role) const
{
    QVariant value("23");
    return value;
}

QHash<int, QByteArray> QForecastItemModel::roleNames() const
{
    QHash<int, QByteArray> roles(QAbstractTableModel::roleNames());
    return roles;
}

QStringList QForecastItemModel::headerModel() const
{
    return { "", "Sgtor house" };
}

QDateTime QForecastItemModel::periodBegin() const
{
    return mPeriodBegin;
}

QDateTime QForecastItemModel::periodEnd() const
{
    return mPeriodBegin;
}

void QForecastItemModel::setPeriodBegin(const QDateTime& periodBegin)
{
    if (mPeriodBegin != periodBegin)
    {
        mPeriodBegin = periodBegin;
        emit periodBeginChanged();
    }
}

void QForecastItemModel::setPeriodEnd(const QDateTime& periodEnd)
{
    if (mPeriodEnd != periodEnd)
    {
        mPeriodEnd = periodEnd;
        emit periodEndChanged();
    }
}

void QForecastItemModel::recalc()
{
    const QMagObjectList& magObjects = mHora->magObjects();
    const QSigtor* sigtor = mHora->planet(0);
    QEclLont sigtorLont = sigtor->eclPos()._M_lont;
}
