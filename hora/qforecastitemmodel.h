
#ifndef __SYMBOID_ASTRO_HORA_QFORECASTITEMMODEL_H__
#define __SYMBOID_ASTRO_HORA_QFORECASTITEMMODEL_H__

#include "astro/hora/defs.h"
#include "astro/hora/qhoraitemsmodel.h"
#include "astro/hora/qforecastevent.h"
#include <QDateTime>

class ASTRO_HORA_API QForecastItemModel : public QHoraTableModel
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "ForecastModel";

public:
    QForecastItemModel(QObject* parent = Q_NULLPTR);

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

public:
    QStringList headerModel() const override;

    Q_PROPERTY(QDateTime periodBegin READ periodBegin WRITE setPeriodBegin NOTIFY periodBeginChanged)
    Q_PROPERTY(QDateTime periodEnd READ periodEnd WRITE setPeriodEnd NOTIFY periodEndChanged)
public:
    QDateTime periodBegin() const;
    QDateTime periodEnd() const;
    void setPeriodBegin(const QDateTime& periodBegin);
    void setPeriodEnd(const QDateTime& periodEnd);
private:
    QDateTime mPeriodBegin;
    QDateTime mPeriodEnd;
signals:
    void periodBeginChanged();
    void periodEndChanged();

private slots:
    void recalc();
};

#endif // __SYMBOID_ASTRO_HORA_QFORECASTITEMMODEL_H__
