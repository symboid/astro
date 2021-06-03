
#include "astro/hora/setup.h"
#include "astro/hora/qhoraitemsmodel.h"

QHoraTableModel::QHoraTableModel(QObject* parent)
    : QAbstractTableModel(parent)
    , mHora(nullptr)
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
{
}

QHora* QHoraTableModel::hora() const
{
    return mHora;
}

void QHoraTableModel::setHora(QHora* hora)
{
    if (mHora)
    {
        disconnect(mHora, SIGNAL(changed()), this, SLOT(update()));
    }
    if (mHora != hora)
    {
        if ((mHora = hora))
        {
            connect(mHora, SIGNAL(changed()), this, SLOT(update()));
        }
        emit horaChanged();
    }
}

int QHoraTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent);
    return headerModel().size();
}

void QHoraTableModel::update()
{
    beginResetModel();
    endResetModel();
}
