
#include "astro/hora/setup.h"
#include "astro/hora/qforecastitemmodel.h"

QForecastItemModel::QForecastItemModel(QObject* parent)
    : QHoraTableModel(Q_NULLPTR, parent)
    , mForecast(new QForecast(this))
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
{
    connect(mForecast.get(), SIGNAL(calcTaskChanged()), this, SLOT(connectForecastSignals()));
    connect(mForecast.get(), SIGNAL(periodBeginChanged()), this, SIGNAL(periodBeginChanged()));
    connect(mForecast.get(), SIGNAL(periodEndChanged()), this, SIGNAL(periodEndChanged()));
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
    mForecast->setPeriodBegin(periodBegin);
}

void QForecastItemModel::setPeriodEnd(QHoraCoords* periodEnd)
{
    mForecast->setPeriodEnd(periodEnd);
}

QCalcable* QForecastItemModel::calcable() const
{
    return mForecast.get();
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

void QForecastItemModel::connectForecastSignals()
{
    if (QCalcTask* forecastCalcTask = mForecast->calcTask())
    {
        connect(forecastCalcTask, SIGNAL(started()), this, SLOT(onRecalcStarted()));
        connect(forecastCalcTask, SIGNAL(finished()), this, SLOT(onRecalcFinished()));
        connect(forecastCalcTask, SIGNAL(aborted()), this, SLOT(onRecalcAborted()));
        connect(mForecast->model()->hora(), SIGNAL(recalculated()), forecastCalcTask, SLOT(invoke()));
    }
}
