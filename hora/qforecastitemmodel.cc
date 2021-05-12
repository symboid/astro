
#include "astro/hora/setup.h"
#include "astro/hora/qforecastitemmodel.h"

QForecastItemModel::QForecastItemModel(QObject* parent)
    : QHoraTableModel(Q_NULLPTR, parent)
    , mForecast(new QForecast(this))
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
{
    connect(this, SIGNAL(periodBeginChanged()), this, SLOT(invokeRecalc()));
    connect(this, SIGNAL(periodEndChanged()), this, SLOT(invokeRecalc()));
    connect(mForecast.get(), SIGNAL(started()), this, SLOT(onRecalcStarted()));
    connect(mForecast.get(), SIGNAL(finished()), this, SLOT(onRecalcFinished()));
    connect(mForecast.get(), SIGNAL(aborted()), this, SLOT(onRecalcAborted()));
}

int QForecastItemModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return mForecast->forecastEventCount();
}

QVariant QForecastItemModel::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(role)
    QVariant value;
    const QForecastEvent* forecastEvent = mForecast->forecastEvent(index.row());
    switch (index.column())
    {
    case 0: return forecastEvent->eventExact()->dateTime();
    case 1: return forecastEvent->sigtor()->symbol(mAstroFont.get());
    case 2: return mAstroFont->aspectLetter(int(forecastEvent->prmsor()->aspect()->dist()));
    case 3: return forecastEvent->prmsor()->horaObject()->symbol(mAstroFont.get());
    }

    return value;
}

QHash<int, QByteArray> QForecastItemModel::roleNames() const
{
    QHash<int, QByteArray> roles(QAbstractTableModel::roleNames());
    roles[SigtorRole] = "sigtor";
    roles[AspectRole] = "aspect";
    roles[PrmsorRole] = "prmsor";
    roles[ExactTimeRole] = "exactTime";
    return roles;
}

QHora* QForecastItemModel::hora() const
{
    return mForecast->model()->hora();
}

void QForecastItemModel::setHora(QHora* hora)
{
    mForecast->model()->setHora(hora);
    connect(hora, SIGNAL(recalculated()), this, SLOT(invokeRecalc()));
}

QStringList QForecastItemModel::headerModel() const
{
    return { "", "Sigtor", "Aspect", "Prmsor" };
}

QHoraCoords* QForecastItemModel::periodBegin() const
{
    return mForecast->periodBegin();
}

QHoraCoords* QForecastItemModel::periodEnd() const
{
    return mForecast->periodEnd();
}

void QForecastItemModel::setPeriodBegin(QHoraCoords* periodBegin)
{
    if (mForecast->periodBegin())
    {
        disconnect(mForecast->periodBegin(), SIGNAL(changed()), this, SLOT(invokeRecalc()));
    }
    if (mForecast->periodBegin() != periodBegin)
    {
        mForecast->setPeriodBegin(periodBegin);
        if (periodBegin)
        {
            connect(periodBegin, SIGNAL(changed()), this, SLOT(invokeRecalc()));
        }
        emit periodBeginChanged();
    }
}

void QForecastItemModel::setPeriodEnd(QHoraCoords* periodEnd)
{
    if (mForecast->periodEnd())
    {
        disconnect(mForecast->periodEnd(), SIGNAL(changed()), this, SLOT(invokeRecalc()));
    }
    if (mForecast->periodEnd() != periodEnd)
    {
        mForecast->setPeriodEnd(periodEnd);
        if (periodEnd)
        {
            connect(periodEnd, SIGNAL(changed()), this, SLOT(invokeRecalc()));
        }
        emit periodEndChanged();
    }
}

QCalcTask* QForecastItemModel::calcTask() const
{
    return mForecast.get();
}

void QForecastItemModel::invokeRecalc()
{
    mForecast->invoke();
}

void QForecastItemModel::onRecalcStarted()
{
    beginResetModel();
}

void QForecastItemModel::onRecalcFinished()
{
    endResetModel();
}

void QForecastItemModel::onRecalcAborted()
{
    endResetModel();
}

QForecastModel* QForecastItemModel::forecastModel() const
{
    return mForecast->model();
}

void QForecastItemModel::setForecastModel(QForecastModel* forecastModel)
{
    if (mForecast->model() != forecastModel)
    {
        mForecast->setModel(forecastModel);
        emit forecastModelChanged();
    }
}
