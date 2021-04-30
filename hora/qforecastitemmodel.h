
#ifndef __SYMBOID_ASTRO_HORA_QFORECASTITEMMODEL_H__
#define __SYMBOID_ASTRO_HORA_QFORECASTITEMMODEL_H__

#include "astro/hora/defs.h"
#include "astro/hora/qhoraitemsmodel.h"
#include "astro/hora/qforecastevent.h"
#include "astro/hora/qforecast.h"
#include <QDateTime>
#include "astro/controls/qastrofont.h"

class ASTRO_HORA_API QForecastItemModel : public QHoraTableModel
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "ForecastItemModel";

public:
    QForecastItemModel(QObject* parent = Q_NULLPTR);

private:
    enum {
        ExactTimeRole = Qt::UserRole,
        SigtorRole,
        AspectRole,
        PrmsorRole,
    };
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
signals:
    void periodBeginChanged();
    void periodEndChanged();

public:
    Q_PROPERTY(QForecastModel* forecastModel READ forecastModel WRITE setForecastModel NOTIFY forecastModelChanged)
private:
    QForecastModel* forecastModel() const;
    void setForecastModel(QForecastModel* forecastModel);
signals:
    void forecastModelChanged();

private:
    QForecast mForecast;
    QSharedPointer<QAstroFont> mAstroFont;
private slots:
    void recalc();
};

#endif // __SYMBOID_ASTRO_HORA_QFORECASTITEMMODEL_H__
