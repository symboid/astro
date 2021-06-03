
#ifndef __SYMBOID_ASTRO_HORA_QDOUBLEHORAVIEW_H__
#define __SYMBOID_ASTRO_HORA_QDOUBLEHORAVIEW_H__

#include "astro/hora/defs.h"
#include "astro/hora/qhoraview.h"
#include "astro/hora/qhoracoords.h"
#include "astro/hora/qhora.h"

class QDoubleHoraView : public QHoraView
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "DoubleHoraView";

public:
    QDoubleHoraView(QQuickItem* parent = Q_NULLPTR);

public:
    Q_PROPERTY(QHora* mainHora READ mainHora WRITE setMainHora NOTIFY mainHoraChanged)
private:
    QHora* mMainHora;
public:
    QHora* mainHora() const;
    void setMainHora(QHora* hora);
signals:
    void mainHoraChanged();

public:
    Q_PROPERTY(QHora* auxHora READ auxHora WRITE setAuxHora NOTIFY auxHoraChanged)
private:
    QHora* mAuxHora;
public:
    QHora* auxHora() const;
    void setAuxHora(QHora* hora);
signals:
    void auxHoraChanged();

private:
    eph::ecl_lont mandalaLeft() const override;

private slots:
    void onRecalcStarted();
    void onRecalcFinished();
    void connectHoraSignals();

private:
    void paint(QPainter* painter) override;
};

#endif // __SYMBOID_ASTRO_HORA_QDOUBLEHORAVIEW_H__
