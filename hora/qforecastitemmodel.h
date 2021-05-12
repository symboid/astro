
#ifndef __SYMBOID_ASTRO_HORA_QFORECASTITEMMODEL_H__
#define __SYMBOID_ASTRO_HORA_QFORECASTITEMMODEL_H__

#include "astro/hora/defs.h"
#include "astro/hora/qhoraitemsmodel.h"
#include "astro/hora/qforecastevent.h"
#include "astro/hora/qforecast.h"
#include "astro/controls/qastrofont.h"
#include "sdk/controls/qcalcthread.h"

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

public:
    virtual QHora* hora() const override;
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
    Q_PROPERTY(QForecastModel* forecastModel READ forecastModel WRITE setForecastModel NOTIFY forecastModelChanged)
private:
    QForecastModel* forecastModel() const;
    void setForecastModel(QForecastModel* forecastModel);
signals:
    void forecastModelChanged();

private:
    QScopedPointer<QForecast> mForecast;
    QSharedPointer<QAstroFont> mAstroFont;

    Q_PROPERTY(bool autoRecalc READ autoRecalc WRITE setAutoRecalc NOTIFY autoRecalcChanged)
    Q_PROPERTY(bool valid READ valid NOTIFY validChanged)
    Q_PROPERTY(bool calculating READ calculating NOTIFY calculatingChanged)
private:
    bool autoRecalc() const;
    bool valid() const;
    void setAutoRecalc(bool autoRecalc);
    bool calculating() const;
signals:
    void autoRecalcChanged();
    void validChanged();
    void calculatingChanged();
public slots:
    Q_INVOKABLE void startRecalc();
    Q_INVOKABLE void abortRecalc();
private slots:
    void invokeRecalc();
    void onRecalcFinished();
    void onRecalcAborted();

public:
    Q_PROPERTY(double progress READ progress NOTIFY progressChanged)
public:
    double progress() const;
signals:
    void progressChanged();
};

#endif // __SYMBOID_ASTRO_HORA_QFORECASTITEMMODEL_H__
