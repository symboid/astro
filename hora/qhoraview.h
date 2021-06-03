
#ifndef __SYMBOID_ASTRO_HORA_QHORAVIEW_H__
#define __SYMBOID_ASTRO_HORA_QHORAVIEW_H__

#include "astro/hora/defs.h"
#include <QQuickPaintedItem>
#include "astro/eph/ecliptic.h"
#include "astro/hora/qplanet.h"
#include "astro/controls/qastrofont.h"
#include "astro/hora/qhoraconfig.h"
#include "astro/hora/qhousecusp.h"
#include <QFontMetrics>
#include "astro/hora/qhora.h"

class ASTRO_HORA_API QHoraView : public QQuickPaintedItem
{
    Q_OBJECT

public:
    static constexpr const char* qml_name = "HoraView";

public:
    QHoraView(QQuickItem* parent = Q_NULLPTR);

protected:
    static constexpr qreal PI = 3.14159265;
    static constexpr qreal ZODIAC_WIDTH = 0.15;
    static constexpr qreal ZODIAC_DIST = 1.0 + ZODIAC_WIDTH;
    static constexpr qreal PLANET_DIST = 0.875;
    static constexpr qreal ASPECT_DIST = 1.0 - 2.0 * (1.0 - PLANET_DIST);
    static constexpr qreal EARTH_DIST = 0.25;

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
    void drawCircle(QPainter* painter, qreal radiusRatio);
    void drawPlanetSymbol(QPainter* painter, const QPlanet* planet, const QEclLont& displayLont, bool outer);
    void drawAspectConnection(QPainter* painter, const QPlanet* planet, const QHoraObject* object);
    void drawRadialText(QPainter* painter, const QString& text, const QEclLont& lont, qreal dist);
    QPointF horaPoint(eph::ecl_lont horaLont, qreal dist) const;
    void paintMandala(QPainter* painter);

private:
    QPointF mMandalaCenter;
    qreal mMandalaRadius;
protected:
    virtual eph::ecl_lont mandalaLeft() const = 0;
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

public:
    Q_PROPERTY(QString housesType READ housesType WRITE setHousesType NOTIFY housesTypeChanged)
public:
    QString housesType() const;
    void setHousesType(const QString& housesType);
protected:
    QHouseSystem::Type mHouseSystemType;
signals:
    void housesTypeChanged();

signals:
    void fontPointSizeChanged();

protected:
    QSharedPointer<QAstroFont> mAstroFont;
    QFontMetrics mAstroFontMetrics;
    arh::main_object<QHoraConfig> mHoraConfig;

protected:
    void paintHouses(QPainter* painter, const QHora* hora, bool outer = false);
    void paintPlanets(QPainter* painter, const QHora* hora, bool outer = false);
    void paintAspectConnections(QPainter* painter, const QHora* hora);
};

Q_DECLARE_METATYPE(QHoraCoords*)
Q_DECLARE_METATYPE(QHora*)

#endif // __SYMBOID_ASTRO_HORA_QHORAVIEW_H__
