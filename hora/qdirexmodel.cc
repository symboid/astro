
#include "astro/hora/setup.h"
#include "astro/hora/qdirexmodel.h"

QDirexModel::QDirexModel(QObject* parent)
    : QForecastModel(parent)
{
}

QVector<QSigtor*> QDirexModel::sigtorList() const
{
    QVector<QSigtor*> sigtors;
    for (QHora::Planets::ConstIterator planet = mHora->planetsBegin(); planet != mHora->planetsEnd(); ++planet)
    {
        sigtors.push_back(*planet);
    }
    return sigtors;
}
