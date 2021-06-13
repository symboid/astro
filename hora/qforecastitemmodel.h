
#ifndef __SYMBOID_ASTRO_HORA_QFORECASTITEMMODEL_H__
#define __SYMBOID_ASTRO_HORA_QFORECASTITEMMODEL_H__

#include "astro/hora/defs.h"
#include "astro/hora/qhoraitemsmodel.h"
#include "astro/hora/qforecastevent.h"
#include "astro/hora/qforecast.h"
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
    QString vertHeaderTitle(int rowIndex) const override;

public:
    virtual void setHora(QHora* hora) override;

    Q_PROPERTY(QHoraCoords* periodBegin READ periodBegin WRITE setPeriodBegin NOTIFY periodBeginChanged)
    Q_PROPERTY(QHoraCoords* periodEnd READ periodEnd WRITE setPeriodEnd NOTIFY periodEndChanged)
public:
    QHoraCoords* periodBegin() const;
    QHoraCoords* periodEnd() const;
    void setPeriodBegin(QHoraCoords* periodBegin);
    void setPeriodEnd(QHoraCoords* periodEnd);
signals:
    void periodBeginChanged();
    void periodEndChanged();

public:
    enum ForecastModel
    {
        NONE,
        PRI_DIREX,
        SEC_DIREX,
        TRANSIT,
    };
    Q_ENUMS(ForecastModel);
public:
    Q_PROPERTY(ForecastModel forecastModel READ forecastModel WRITE setForecastModel NOTIFY forecastModelChanged)
private:
    ForecastModel forecastModel() const;
    void setForecastModel(ForecastModel forecastModel);
    ForecastModel mForecastModel;
signals:
    void forecastModelChanged();

private:
    QScopedPointer<QForecast> mForecast;
    QSharedPointer<QAstroFont> mAstroFont;

    Q_PROPERTY(QCalcable* calcable READ calcable CONSTANT)
private:
    QCalcable* calcable() const;
private slots:
    void connectForecastSignals();
    void onRecalcStarted();
    void onRecalcFinished();
    void onRecalcAborted();
};

#endif // __SYMBOID_ASTRO_HORA_QFORECASTITEMMODEL_H__
