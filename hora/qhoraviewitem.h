
#ifndef __SYMBOID_ASTRO_HORA_QHORAVIEWITEM_H__
#define __SYMBOID_ASTRO_HORA_QHORAVIEWITEM_H__

#include "astro/hora/defs.h"
#include <QQuickPaintedItem>
#include "astro/eph/ecliptic.h"
#include "astro/hora/qhora.h"
#include "astro/controls/qastrofont.h"
#include "astro/hora/qhoraitemsmodel.h"
#include "astro/eph/constellation.h"
#include "astro/db/fixstars.h"
#include "astro/hora/qhoraconfig.h"

class ASTRO_HORA_API QHoraPlanetsModel : public QEclipticTableModel
{
    Q_OBJECT

public:
    QHoraPlanetsModel(QHora* hora, QObject* parent = Q_NULLPTR);

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QStringList headerModel() const override;
};

class ASTRO_HORA_API QHoraHousesModel : public QEclipticTableModel
{
    Q_OBJECT

public:
    QHoraHousesModel(QHora* hora, QObject* parent = Q_NULLPTR);

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    QStringList headerModel() const override;
};

class ASTRO_HORA_API QHoraViewItem : public QQuickPaintedItem
{
    Q_OBJECT

public:
    static constexpr const char* qml_name = "HoraView";

public:
    QHoraViewItem(QQuickItem* parent = Q_NULLPTR);

private:
    static constexpr qreal PI = 3.14159265;
    static constexpr qreal PLANET_DIST = 0.875;
    static constexpr qreal ASPECT_DIST = 1.0 - 2.0 * (1.0 - PLANET_DIST);

    QBrush planetBrush(QPlanet::Index planetIndex, qreal alpha);
    enum Rank
    {
        PERG = 0x00, // peregrin

        HOME = 0x03, // otthon
        EXIL = 0x01, // szamuzetes

        ELEV = 0x0C, // eroben
        FALL = 0x04, // esesben
    };
    Rank planetRank(const QPlanet* planet) const;
    void drawPlanetSymbol(QPainter* painter, const QPlanet* planet, const eph::ecl_pos& displayPos);
//    void drawConstellation(QPainter* painter, const eph::constellation* constellation);
    void drawAspectConnection(QPainter* painter, const QPlanet* planet, const QHoraObject* object);
    void drawRadialText(QPainter* painter, const QString& text, const QEclLont& lont, qreal dist);
    void paint(QPainter* painter) override;
private:
    QPointF horaPoint(eph::ecl_lont horaLont, qreal dist) const;

private:
    QPointF mMandalaCenter;
    qreal mMandalaRadius;
    QHora* mHora;

    Q_PROPERTY(QHora* hora READ hora CONSTANT)
private:
    QHora* hora() const { return mHora; }

private:
    eph::ecl_lont mandalaLeft() const;
    qreal eclipticRatio() const;
    qreal eclipticRadius() const;
    qreal oneDegree() const;
private slots:
    void calcMandalaGeometry();
public:
    Q_PROPERTY(qreal defaultZoom READ defaultZoom NOTIFY defaultZoomChanged)
    qreal defaultZoom() const;
signals:
    void defaultZoomChanged();

private:
    QSharedPointer<QAstroFont> mAstroFont;

public:
    Q_PROPERTY(QHoraCoords* coords READ coords WRITE setCoords NOTIFY coordsChanged)
    Q_PROPERTY(QString housesType READ housesType WRITE setHousesType NOTIFY housesTypeChanged)
public:
    QHoraCoords* coords() const;
    void setCoords(QHoraCoords* coords);
    QString housesType() const;
    void setHousesType(const QString& housesType);
signals:
    void coordsChanged();
    void housesTypeChanged();
    void fontPointSizeChanged();

private slots:
    void onRecalcStarted();
    void onRecalcFinished();
    void connectHoraSignals();

public:
    Q_PROPERTY(QEclipticTableModel* planetsModel READ planetsModel NOTIFY planetsModelChanged)
    Q_PROPERTY(QEclipticTableModel* housesModel READ housesModel NOTIFY housesModelChanged)
private:
    QHoraPlanetsModel* mPlanetsModel;
    QHoraHousesModel* mHousesModel;
    QEclipticTableModel* planetsModel() const { return mPlanetsModel; }
    QEclipticTableModel* housesModel() const { return mHousesModel; }
signals:
    void planetsModelChanged();
    void housesModelChanged();

private:
    arh::main_object<Fixstars> mFixstars;
    arh::main_object<QHoraConfig> mHoraConfig;
};

Q_DECLARE_METATYPE(QHoraCoords*)
Q_DECLARE_METATYPE(QHora*)

#endif // __SYMBOID_ASTRO_HORA_QHORAVIEWITEM_H__
