
#include "astro/hora/setup.h"
#include "astro/hora/qhoraitemsmodel.h"

QHoraTableModel::QHoraTableModel(QHora* hora, QObject* parent)
    : QAbstractTableModel(parent)
    , mHora(hora)
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
{
}

QHora* QHoraTableModel::hora() const
{
    return mHora;
}

void QHoraTableModel::setHora(QHora* hora)
{
    if (mHora != hora)
    {
        mHora = hora;
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
