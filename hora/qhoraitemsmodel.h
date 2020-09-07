
#ifndef __SYMBOID_ASTRO_HORA_QHORAITEMSMODEL_H__
#define __SYMBOID_ASTRO_HORA_QHORAITEMSMODEL_H__

#include "astro/hora/defs.h"
#include "astro/calculo/hora.h"
#include <QAbstractTableModel>
#include "astro/controls/qastrofont.h"

hor_ns_begin
typedef basic_hora<eph_proxy> hora;
hor_ns_end

class QHoraTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    QHoraTableModel(const hor::hora* hora, QObject* parent = Q_NULLPTR);

protected:
    const hor::hora* mHora;
    QSharedPointer<QAstroFont> mAstroFont;
public:
    using QAbstractItemModel::beginResetModel;
    using QAbstractItemModel::endResetModel;

public:
    Q_PROPERTY(QStringList headerModel READ headerModel NOTIFY headerModelChanged)
private:
    virtual QStringList headerModel() const = 0;
signals:
    void headerModelChanged();

public:
    Q_INVOKABLE void update();
};

class QEclipticTableModel : public QHoraTableModel
{
    Q_OBJECT

public:
    QEclipticTableModel(const hor::hora* hora, QObject* parent = Q_NULLPTR);

protected:
    enum {
        SymbolRole = Qt::UserRole,
        EclLontRole,
        EclLattRole,
        EclSpeedRole,
    };

public:
    QHash<int, QByteArray> roleNames() const override;

public:
    Q_PROPERTY(bool withSpeed MEMBER mWithSpeed WRITE setWithSpeed NOTIFY withSpeedChanged)
protected:
    bool mWithSpeed;
public:
    void setWithSpeed(bool withSpeed);
signals:
    void withSpeedChanged();
};

#endif // __SYMBOID_ASTRO_HORA_QHORAITEMSMODEL_H__
