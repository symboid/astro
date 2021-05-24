
#ifndef __SYMBOID_ASTRO_HORA_QHORAITEMSMODEL_H__
#define __SYMBOID_ASTRO_HORA_QHORAITEMSMODEL_H__

#include "astro/hora/defs.h"
#include "astro/hora/qhora.h"
#include <QAbstractTableModel>
#include "astro/controls/qastrofont.h"

class ASTRO_HORA_API QHoraTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    QHoraTableModel(QHora* hora, QObject* parent = Q_NULLPTR);

public:
    Q_PROPERTY(QHora* hora READ hora WRITE setHora NOTIFY horaChanged)
private:
    QHora* mHora;
public:
    virtual QHora* hora() const;
    virtual void setHora(QHora* hora);
signals:
    void horaChanged();

protected:
    QSharedPointer<QAstroFont> mAstroFont;
public:
    using QAbstractItemModel::beginResetModel;
    using QAbstractItemModel::endResetModel;

public:
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

public:
    Q_PROPERTY(QStringList headerModel READ headerModel NOTIFY headerModelChanged)
private:
    virtual QStringList headerModel() const = 0;
signals:
    void headerModelChanged();

public:
    Q_INVOKABLE void update();
};

#endif // __SYMBOID_ASTRO_HORA_QHORAITEMSMODEL_H__
