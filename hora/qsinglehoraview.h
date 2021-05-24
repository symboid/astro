
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
    Q_PROPERTY(QHoraCoords* coords READ coords WRITE setCoords NOTIFY coordsChanged)
public:
    QHoraCoords* coords() const;
    void setCoords(QHoraCoords* coords);
signals:
    void coordsChanged();

public:
    Q_PROPERTY(QHora* hora READ hora CONSTANT)
private:
    QScopedPointer<QHora> mHora;
public:
    QHora* hora() const { return mHora.get(); }

public:
    Q_PROPERTY(QEclipticTableModel* planetsModel READ planetsModel NOTIFY planetsModelChanged)
private:
    QHoraPlanetsModel* mPlanetsModel;
    QEclipticTableModel* planetsModel() const { return mPlanetsModel; }
signals:
    void planetsModelChanged();

public:
    Q_PROPERTY(QEclipticTableModel* housesModel READ housesModel NOTIFY housesModelChanged)
private:
    QHoraHousesModel* mHousesModel;
    QEclipticTableModel* housesModel() const { return mHousesModel; }
signals:
    void housesModelChanged();

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

Q_DECLARE_METATYPE(QHoraCoords*)
Q_DECLARE_METATYPE(QHora*)

#endif // __SYMBOID_ASTRO_HORA_QSINGLEHORAVIEW_H__
