
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/qhoraitemsmodel.h"

QHoraItemsModel::QHoraItemsModel(const hor::hora* hora, QObject* parent)
    : QAbstractTableModel(parent)
    , mHora(hora)
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
{
}

QHash<int, QByteArray> QHoraItemsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractTableModel::roleNames();
    roles[SymbolRole] = "symbol";
    roles[EclLontRole] = "ecl_lont";
    roles[EclLattRole] = "ecl_latt";
    roles[EclSpeedRole] = "ecl_speed";
    return roles;
}