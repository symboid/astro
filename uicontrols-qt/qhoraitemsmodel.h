
#ifndef __SYMBOID_ASTRO_UICONTROLS_QT_QHORAITEMSMODEL_H__
#define __SYMBOID_ASTRO_UICONTROLS_QT_QHORAITEMSMODEL_H__

#include "astro/uicontrols-qt/defs.h"
#include "astro/calculo/hora.h"
#include <QAbstractTableModel>
#include "astro/uicontrols-qt/qastrofont.h"

hor_ns_begin
typedef basic_hora<eph_proxy> hora;
hor_ns_end

class QHoraItemsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    QHoraItemsModel(const hor::hora* hora, QObject* parent = Q_NULLPTR);

protected:
    enum {
        SymbolRole = Qt::UserRole,
        EclLontRole,
        EclLattRole,
        EclSpeedRole,
    };

public:
    QHash<int, QByteArray> roleNames() const override;
protected:
    const hor::hora* mHora;
    QSharedPointer<QAstroFont> mAstroFont;
public:
    using QAbstractItemModel::beginResetModel;
    using QAbstractItemModel::endResetModel;

public:
    Q_PROPERTY(QStringList headerModel READ headerModel CONSTANT)
private:
    virtual QStringList headerModel() const = 0;
};

#endif // __SYMBOID_ASTRO_UICONTROLS_QT_QHORAITEMSMODEL_H__
