
#include "astro/hora/setup.h"
#include "astro/hora/qhoraitemsmodel.h"

QHoraTableModel::QHoraTableModel(QHora* hora, QObject* parent)
    : QAbstractTableModel(parent)
    , mHora(hora)
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
{
}

void QHoraTableModel::setHora(QHora* hora)
{
    if (mHora != hora)
    {
        mHora = hora;
        emit horaChanged();
        connect(mHora, SIGNAL(recalculated()), this, SIGNAL(horaRecalculated()));
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

QEclipticTableModel::QEclipticTableModel(QHora* hora, QObject* parent)
    : QHoraTableModel(hora, parent)
    , mWithSpeed(true)
{
}

QHash<int, QByteArray> QEclipticTableModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractTableModel::roleNames();
    roles[SymbolRole] = "symbol";
    roles[EclLontRole] = "ecl_lont";
    roles[EclLattRole] = "ecl_latt";
    if (mWithSpeed)
    {
        roles[EclSpeedRole] = "ecl_speed";
    }
    return roles;
}

void QEclipticTableModel::setWithSpeed(bool withSpeed)
{
    if (mWithSpeed != withSpeed)
    {
        beginResetModel();
        mWithSpeed = withSpeed;
        emit withSpeedChanged();
        emit headerModelChanged();
        endResetModel();
    }
}
