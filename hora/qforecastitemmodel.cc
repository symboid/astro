
#include "astro/hora/setup.h"
#include "astro/hora/qforecastitemmodel.h"
#include <QtConcurrent>

QForecastItemModel::QForecastItemModel(QObject* parent)
    : QHoraTableModel(Q_NULLPTR, parent)
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
    , mAutoRecalc(false)
    , mIsValid(false)
{
    connect(this, SIGNAL(periodBeginChanged()), this, SLOT(invokeRecalc()));
    connect(this, SIGNAL(periodEndChanged()), this, SLOT(invokeRecalc()));
    connect(this, SIGNAL(recalculated()), this, SLOT(onRecalculated()));
}

int QForecastItemModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return mForecast.forecastEventCount();
}

QVariant QForecastItemModel::data(const QModelIndex& index, int role) const
{
    Q_UNUSED(role)
    QVariant value;
    const QForecastEvent* forecastEvent = mForecast.forecastEvent(index.row());
    switch (index.column())
    {
    case 0: return forecastEvent->eventExact();
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
    return mForecast.model()->hora();
}

void QForecastItemModel::setHora(QHora* hora)
{
    mForecast.model()->setHora(hora);
    connect(hora, SIGNAL(recalculated()), this, SLOT(invokeRecalc()));
}

QStringList QForecastItemModel::headerModel() const
{
    return { "", "Sigtor", "Aspect", "Prmsor" };
}

QDateTime QForecastItemModel::periodBegin() const
{
    return mForecast.periodBegin();
}

QDateTime QForecastItemModel::periodEnd() const
{
    return mForecast.periodEnd();
}

void QForecastItemModel::setPeriodBegin(const QDateTime& periodBegin)
{
    if (mForecast.periodBegin() != periodBegin)
    {
        mForecast.setPeriodBegin(periodBegin);
        emit periodBeginChanged();
    }
}

void QForecastItemModel::setPeriodEnd(const QDateTime& periodEnd)
{
    if (mForecast.periodEnd() != periodEnd)
    {
        mForecast.setPeriodEnd(periodEnd);
        emit periodEndChanged();
    }
}

void QForecastItemModel::setAutoRecalc(bool autoRecalc)
{
    if (mAutoRecalc != autoRecalc)
    {
        mAutoRecalc = autoRecalc;
        emit autoRecalcChanged();
        if (mAutoRecalc && !mIsValid)
        {
            recalc();
        }
    }
}

void QForecastItemModel::setValid(bool isValid)
{
    if (mIsValid != isValid)
    {
        mIsValid = isValid;
        emit validChanged();
    }
}

void QForecastItemModel::invokeRecalc()
{
    if (mAutoRecalc)
    {
        recalc();
    }
    else
    {
        setValid(false);
    }
}

void QForecastItemModel::recalc()
{
    beginResetModel();
    QtConcurrent::run([this]{
        mForecast.calc();
        setValid(true);
        emit recalculated();
    });
}

void QForecastItemModel::onRecalculated()
{
    endResetModel();
}

QForecastModel* QForecastItemModel::forecastModel() const
{
    return mForecast.model();
}

void QForecastItemModel::setForecastModel(QForecastModel* forecastModel)
{
    if (mForecast.model() != forecastModel)
    {
        mForecast.setModel(forecastModel);
        emit forecastModelChanged();
    }
}
