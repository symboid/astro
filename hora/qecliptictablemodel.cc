
#include "astro/hora/setup.h"
#include "astro/hora/qecliptictablemodel.h"

QEclipticTableModel::QEclipticTableModel(QObject* parent)
    : QHoraTableModel(parent)
{
}

QHash<int, QByteArray> QEclipticTableModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractTableModel::roleNames();
    roles[SymbolRole] = "symbol";
    roles[EclLontRole] = "ecl_lont";
    roles[EclLattRole] = "ecl_latt";
    roles[EclSpeedRole] = "ecl_speed";
    return roles;
}

QHoraPlanetsModel::QHoraPlanetsModel(QObject* parent)
    : QEclipticTableModel(parent)
{
}

int QHoraPlanetsModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return hora() ? int(hora()->planetCount()) : 0;
}

QVariant QHoraPlanetsModel::data(const QModelIndex& index, int role) const
{
    QVariant planetData;
    if (hora() != Q_NULLPTR)
    {
        if (role == Qt::DisplayRole)
        {
            switch (index.column())
            {
            case 0: role = EclLontRole; break;
            case 1: role = EclLattRole; break;
            case 2: role = EclSpeedRole; break;
            }
        }
        const QPlanet* planet = hora()->planet(index.row());
        switch (role) {
        case SymbolRole: planetData = planet->symbol(mAstroFont.get()); break;
        case EclLontRole: planetData = planet->eclPos()._M_lont.to_arc_degree(); break;
        case EclLattRole: planetData = planet->eclPos()._M_latt; break;
        case EclSpeedRole: planetData = planet->eclSpeed()._M_lont; break;
        }
    }
    return planetData;
}

QStringList QHoraPlanetsModel::horzHeaderModel() const
{
    return { tr("Ecl. lng."), tr("Ecl. lat."), tr("°/day") };
}

QString QHoraPlanetsModel::vertHeaderTitle(int rowIndex) const
{
    const QPlanet* planet = hora()->planet(rowIndex);
    return planet ? planet->symbol(mAstroFont.get()) : "";
}

QHoraHousesModel::QHoraHousesModel(QObject* parent)
    : QEclipticTableModel(parent)
{
}

int QHoraHousesModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return hora() ? QHouseSystem::HOUSE_COUNT : 0;
}

QVariant QHoraHousesModel::data(const QModelIndex& index, int role) const
{
    QVariant houseData;
    if (hora() != Q_NULLPTR)
    {
        if (role == Qt::DisplayRole)
        {
            switch (index.column())
            {
            case 0: role = EclLontRole; break;
            case 1: role = EclSpeedRole; break;
            }
        }
        int houseIndex = index.row() + 1;
        const QHouseCusp* house = hora()->house(houseIndex);
        switch (role) {
        case SymbolRole: houseData = house->symbol(mAstroFont.get()); break;
        case EclLontRole: houseData = house->eclPos()._M_lont.to_arc_degree(); break;
        case EclLattRole: houseData = 0; break;
        case EclSpeedRole: houseData = house->eclSpeed().lont_coord().arc_pos(); break;
        }
    }
    return houseData;
}

QStringList QHoraHousesModel::horzHeaderModel() const
{
    return { tr("Ecl. lng."), tr("°/hour") };
}

QString QHoraHousesModel::vertHeaderTitle(int rowIndex) const
{
    const QHouseCusp* house = hora()->house(rowIndex + 1);
    return house ? house->symbol(mAstroFont.get()) : "";
}
