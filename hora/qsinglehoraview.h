
#ifndef __SYMBOID_ASTRO_HORA_QSINGLEHORAVIEW_H__
#define __SYMBOID_ASTRO_HORA_QSINGLEHORAVIEW_H__

#include "astro/hora/defs.h"
#include "astro/hora/qhoraview.h"
#include "astro/hora/qhoracoords.h"
#include "astro/hora/qhora.h"
#include "astro/hora/qecliptictablemodel.h"
#include "astro/eph/constellation.h"
#include "astro/db/fixstars.h"

class QSingleHoraView : public QHoraView
{
    Q_OBJECT
public:
    static constexpr const char* qml_name = "SingleHoraView";

public:
    QSingleHoraView(QQuickItem* parent = Q_NULLPTR);

public:
    Q_PROPERTY(QHora* hora READ hora WRITE setHora NOTIFY horaChanged)
private:
    QHora* mHora;
public:
    QHora* hora() const;
    void setHora(QHora* hora);
signals:
    void horaChanged();

private:
    eph::ecl_lont mandalaLeft() const override;

private slots:
    void onRecalcStarted();
    void onRecalcFinished();
    void connectHoraSignals();

private:
    void paintFixstars(QPainter* painter);
    void paint(QPainter* painter) override;

private:
    arh::main_object<Fixstars> mFixstars;
};

#endif // __SYMBOID_ASTRO_HORA_QSINGLEHORAVIEW_H__
