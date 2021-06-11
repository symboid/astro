
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
    QHoraTableModel(QObject* parent = Q_NULLPTR);

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
    Q_PROPERTY(QStringList horzHeaderModel READ horzHeaderModel CONSTANT)
private:
    virtual QStringList horzHeaderModel() const = 0;

private slots:
    void update();

private:
    virtual QString vertHeaderTitle(int rowIndex) const = 0;
public:
    class VertHeaderModel : public QAbstractListModel
    {
    public:
        VertHeaderModel(QHoraTableModel* parent);
    private:
        QHoraTableModel* mParentModel;

    public:
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role) const override;
    };

    Q_PROPERTY(QAbstractListModel* vertHeaderModel MEMBER mVertHeaderModel NOTIFY vertHeaderModelChanged)
signals:
    void vertHeaderModelChanged();
private:
    QAbstractListModel* mVertHeaderModel;
};

#endif // __SYMBOID_ASTRO_HORA_QHORAITEMSMODEL_H__
