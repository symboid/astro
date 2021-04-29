
#include "astro/hora/defs.h"
#include "astro/hora/qforecastmodel.h"

QForecastModel::QForecastModel(QObject* parent)
    : QObject(parent)
    , mHora(nullptr)
{
}

const QHora* QForecastModel::hora() const
{
    return mHora;
}

void QForecastModel::setHora(const QHora* hora)
{
    if (mHora != hora)
    {
        mHora = hora;
        emit horaChanged();
    }
}
