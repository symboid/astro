
#include "astro/hora/setup.h"
#include "astro/hora/qforecastitemmodel.h"
#include "astro/hora/qdirexmodel.h"
#include "astro/hora/qtransitmodel.h"

QForecastItemModel::QForecastItemModel(QObject* parent)
    : QHoraTableModel(parent)
    , mForecastModel(NONE)
    , mForecast(new QForecast(this))
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
{
    setHorzHeaderTitles({ "Sgtr", "Asp", "Pmr" });

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
    case 0: return forecastEvent->sigtor()->symbol(mAstroFont.get());
    case 1: return mAstroFont->aspectLetter(int(forecastEvent->prmsor()->aspectDist()));
    case 2: return forecastEvent->prmsor()->horaObjectSymbol();
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

void QForecastItemModel::setHora(QHora* hora)
{
    mForecast->deleteParam(this->hora());
    QHoraTableModel::setHora(hora);
    if (QForecastModel* forecastModel = mForecast->model())
    {
        forecastModel->setHora(hora);
    }
    mForecast->addParam(hora);
}

QString QForecastItemModel::vertHeaderTitle(int rowIndex) const
{
    const QForecastEvent* forecastEvent = mForecast->forecastEvent(rowIndex);
    return forecastEvent ? QLocale().toString(forecastEvent->eventExact()->dateTime().date(), QLocale::ShortFormat) : "";
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

QForecastItemModel::ForecastModel QForecastItemModel::forecastModel() const
{
    return mForecastModel;
}

void QForecastItemModel::setForecastModel(ForecastModel forecastModel)
{
    if (mForecastModel != forecastModel)
    {
        QForecastModel* model = nullptr;
        switch (forecastModel)
        {
        case PRI_DIREX: model = new QDirexModel; break;
        case TRANSIT: model = new QTransitModel; break;
        default: model = nullptr;
        }
        if (model)
        {
            model->setHora(hora());
        }
        mForecast->setModel(model);

        mForecastModel = forecastModel;
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
    }
}
