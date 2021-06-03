
#include "astro/hora/setup.h"
#include "astro/hora/qhoraview.h"
#include <QPainter>
#include "astro/hora/qhorastellium.h"

QHoraView::QHoraView(QQuickItem* parent)
    : QQuickPaintedItem(parent)
    , mHouseSystemType(QHouseSystem::PLACIDUS)
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
    , mAstroFontMetrics(*mAstroFont)
{
    setFlag(QQuickItem::ItemHasContents);
    setRenderTarget(QQuickPaintedItem::Image);
    connect(this, SIGNAL(widthChanged()), this, SLOT(calcMandalaGeometry()));
    connect(this, SIGNAL(heightChanged()), this, SLOT(calcMandalaGeometry()));

    qRegisterMetaType<QHora*>();
    qRegisterMetaType<QHoraCoords*>();
}

void QHoraView::calcMandalaGeometry()
{
    // basic attributes of horoscope mandala
    QRectF mandalaRect = boundingRect();
    mMandalaRadius = qMin(mandalaRect.width(), mandalaRect.height()) / 2.0;
    mMandalaCenter = mandalaRect.center();
}

qreal QHoraView::defaultZoom() const
{
    return eclipticRatio() + 0.2;
}

qreal QHoraView::eclipticRatio() const
{
    return mHoraConfig->fixstars()->included() ? 0.55 : 0.8;
}

qreal QHoraView::eclipticRadius() const
{
    return mMandalaRadius * eclipticRatio();
}

qreal QHoraView::oneDegree() const
{
    return eclipticRadius() * PLANET_DIST * 2.0 * PI / 360;
}

QBrush QHoraView::planetBrush(QPlanet::Index planetIndex, qreal alpha)
{
    QBrush brush;
    // style = solid
    brush.setStyle(Qt::SolidPattern);
    // color
    QColor objectColor(0x80,0x80,0x80);
    switch (planetIndex)
    {
    case QPlanet::SUN: objectColor = QColor(0xF0,0xE0,0x00); break;
    case QPlanet::MOON: objectColor = QColor(0xC4,0xCA,0xCE); break;
    case QPlanet::MERCURY: objectColor = QColor(0xBF,0x93,0x39); break;
    case QPlanet::VENUS: objectColor = QColor(0x00,0x80,0x40); break;
    case QPlanet::MARS: objectColor = QColor(0xB0,0x00,0x00); break;
    case QPlanet::JUPITER: objectColor = QColor(0xC0,0x20,0xC0); break;
    case QPlanet::SATURN: objectColor = QColor(0x20,0x20,0x20); break;

    case QPlanet::URANUS: objectColor = QColor(0x84,0x87,0x89); break;
    case QPlanet::NEPTUNE: objectColor = QColor(0x20,0x20,0xC0); break;
    case QPlanet::PLUTO: objectColor = QColor(0x80,0x20,0x20); break;

    case QLunarNode::DRAGON_HEAD: objectColor = Qt::black; break;
    case QLunarNode::DRAGON_TAIL: objectColor = Qt::black; break;
    case QPlanet::LILITH: objectColor = Qt::black; break;
    case QPlanet::CHIRON: objectColor = Qt::black; break;

    case QPlanet::UNDEF: objectColor = Qt::black; break;
    }
    objectColor.setAlphaF(alpha);
    brush.setColor(objectColor);
    return brush;
}

QHoraView::Rank QHoraView::planetRank(const QPlanet* planet) const
{
    int planetIndex = int(planet->mIndex - QPlanet::SUN);
    eph::zod zodSign = planet->eclPos().zod_coords()._M_sign;
    int zodIndex = int(zodSign) - int(eph::zod::ARI);
    static constexpr int PLANET_COUNT = 10;
    static const enum Rank PLANET_RANK[PLANET_COUNT][12] =
    {
        {
            Rank::ELEV, Rank::PERG, Rank::PERG, Rank::PERG, Rank::HOME, Rank::PERG,
            Rank::FALL, Rank::PERG, Rank::PERG, Rank::PERG, Rank::EXIL, Rank::PERG,
        },
        {
            Rank::PERG, Rank::ELEV, Rank::PERG, Rank::HOME, Rank::PERG, Rank::PERG,
            Rank::PERG, Rank::FALL, Rank::PERG, Rank::EXIL, Rank::PERG, Rank::PERG,
        },
        {
            Rank::PERG, Rank::PERG, Rank::HOME, Rank::PERG, Rank::PERG, Rank(HOME + ELEV),
            Rank::PERG, Rank::PERG, Rank::EXIL, Rank::PERG, Rank::PERG, Rank(EXIL + FALL),
        },
        {
            Rank::EXIL, Rank::HOME, Rank::PERG, Rank::PERG, Rank::PERG, Rank::FALL,
            Rank::HOME, Rank::EXIL, Rank::PERG, Rank::PERG, Rank::PERG, Rank::ELEV,
        },
        {
            Rank::HOME, Rank::EXIL, Rank::PERG, Rank::FALL, Rank::PERG, Rank::PERG,
            Rank::EXIL, Rank::HOME, Rank::PERG, Rank::ELEV, Rank::PERG, Rank::PERG,
        },
        {
            Rank::PERG, Rank::PERG, Rank::EXIL, Rank::ELEV, Rank::PERG, Rank::EXIL,
            Rank::PERG, Rank::PERG, Rank::HOME, Rank::FALL, Rank::PERG, Rank::HOME,
        },
        {
            Rank::FALL, Rank::PERG, Rank::PERG, Rank::EXIL, Rank::EXIL, Rank::PERG,
            Rank::ELEV, Rank::PERG, Rank::PERG, Rank::HOME, Rank::HOME, Rank::PERG,
        },
        {
            Rank::PERG, Rank::FALL, Rank::PERG, Rank::PERG, Rank::EXIL, Rank::PERG,
            Rank::PERG, Rank::ELEV, Rank::PERG, Rank::PERG, Rank::HOME, Rank::PERG,
        },
        {
            Rank::PERG, Rank::PERG, Rank::PERG, Rank::PERG, Rank::ELEV, Rank::EXIL,
            Rank::PERG, Rank::PERG, Rank::PERG, Rank::PERG, Rank::FALL, Rank::HOME,
        },
        {
            Rank::PERG, Rank::EXIL, Rank::PERG, Rank::PERG, Rank::PERG, Rank::ELEV,
            Rank::PERG, Rank::HOME, Rank::PERG, Rank::PERG, Rank::PERG, Rank::FALL,
        },
    };
    return 0 <= planetIndex && planetIndex < PLANET_COUNT && 0 <= zodIndex && zodIndex < 12 ? PLANET_RANK[planetIndex][zodIndex] : Rank::PERG;
}

void QHoraView::drawCircle(QPainter* painter, qreal radiusRatio)
{
    const qreal radius = eclipticRadius() * radiusRatio;
    painter->drawEllipse(mMandalaCenter, radius, radius);
}

void QHoraView::drawPlanetSymbol(QPainter* painter, const QPlanet* planet,
        const QEclLont& displayLont, bool outer)
{
    Rank rank = planetRank(planet);

    QFont planetFont = *mAstroFont;
    planetFont.setPixelSize(int(oneDegree() * 5));
    painter->setFont(planetFont);
    QFontMetrics planetFontMetrics(planetFont);

    QPointF planetSignPoint(horaPoint(displayLont, outer ? ZODIAC_DIST + ZODIAC_WIDTH : PLANET_DIST));

    // planet sign pen
    QPen planetPen;
    planetPen.setWidthF(1);
    if ((rank & 0x0C) == Rank::ELEV)
    {
        planetPen.setColor(Qt::red);
    }
    else if ((rank & 0x0C) == Rank::FALL)
    {
        planetPen.setColor("#999900");
    }
    else if ((rank & 0x03) == Rank::EXIL)
    {
        planetPen.setColor(Qt::lightGray);
    }
    painter->setPen(planetPen);

    // bounding circle
    if ((rank & 0x03) == Rank::HOME)
    {
        qreal planetSize = 4 * oneDegree();
        painter->drawEllipse(planetSignPoint, planetSize, planetSize);
    }

    // planet sign text
    QString planetText = planet->symbol(mAstroFont.get());
    QSize textSize = planetFontMetrics.size(0, planetText);
    QRectF planetSignRect(planetSignPoint - QPointF(textSize.width() / 2 - 1, textSize.height() / 2),
                          planetSignPoint + QPointF(textSize.width() / 2,   textSize.height() / 2));
    if (planet->isRetrograd())
    {
        QString retrogradText = mAstroFont->retrogradLetter();
        planetText += retrogradText;
    }
    painter->drawText(planetSignRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextDontClip, planetText);

}

void QHoraView::drawRadialText(QPainter* painter, const QString& text, const QEclLont& lont, qreal dist)
{
    painter->save();
    painter->translate(boundingRect().center());

    const QRectF textBoundingRect(QFontMetrics(painter->font(), painter->device()).boundingRect(text));

    qreal displayLont = mandalaLeft() - lont;
    qreal baseAngle;
    qreal translateX;
    if (displayLont < 90.0 || 270.0 < displayLont)
    {
        baseAngle = displayLont;
        translateX = -eclipticRadius() * dist - textBoundingRect.width();
    }
    else
    {
        baseAngle = displayLont + 180.0;
        translateX = eclipticRadius() * dist;
    }
    painter->rotate(baseAngle);
    painter->drawText(textBoundingRect.translated(translateX, 0.0), Qt::TextSingleLine, text);
    painter->restore();
}

void QHoraView::drawAspectConnection(QPainter* painter, const QPlanet* planet, const QHoraObject* object)
{
    const QAspectConfigNode* aspect = mHoraConfig->aspects()->findConnection(planet, object);
    if (aspect != nullptr && aspect->enabled())
    {
        QPen aspectPen(aspect->draw()->lineColor(), oneDegree() * aspect->draw()->lineWidth(),
                       aspect->isMain() ? Qt::SolidLine : Qt::DashLine);
        painter->setPen(aspectPen);

        QPointF leftPoint(horaPoint(planet->eclPos()._M_lont, ASPECT_DIST));
        QPointF rightPoint(horaPoint(object->eclPos()._M_lont, ASPECT_DIST));
        painter->drawLine(leftPoint, rightPoint);
    }

}

QPointF QHoraView::horaPoint(eph::ecl_lont horaLont, qreal dist) const
{
    horaLont -= mandalaLeft();
    QPointF point;
    point.setX(mMandalaCenter.x() - cos(horaLont / 180.0 * PI) * eclipticRadius() * dist);
    point.setY(mMandalaCenter.y() + sin(horaLont / 180.0 * PI) * eclipticRadius() * dist);
    return point;
}

void QHoraView::paintMandala(QPainter* painter)
{
    if (painter) {

        // eclipitic radius
        painter->setPen(QPen(QColor(0,0,0), 1.5));
        drawCircle(painter, 1.0);
        drawCircle(painter, 1.15);

        // planet aspects radius
        painter->setPen(QPen(QColor(0x80,0x80,0x80), 1.0));
        drawCircle(painter, ASPECT_DIST);

        // earth radius
        painter->setPen(QPen(QColor(0,0,0), 1.5));
        drawCircle(painter, EARTH_DIST);


        // zodiac sign domains
        painter->setPen(QPen(QColor(0,0,0), 1.5));
        QFont zodiacFont(*mAstroFont);
        zodiacFont.setPixelSize(oneDegree() * 4);
        painter->setFont(zodiacFont);
        for (int z = 1; z <= 12; ++z)
        {
            eph::ecl_lont zodSignLont = (z - 1)* 30;
            painter->drawLine(horaPoint(zodSignLont, 1.0),
                              horaPoint(zodSignLont, 1.15));

            QPointF zodSignPoint(horaPoint(zodSignLont + 15.0, 1.07));
            QSize textSize = mAstroFontMetrics.size(0, mAstroFont->zodLetter(eph::zod(z)));
            QRectF zodSignRect(zodSignPoint - QPointF(textSize.width() / 2, textSize.height() / 2),
                               zodSignPoint + QPointF(textSize.width() / 2, textSize.height() / 2));
            painter->drawText(zodSignRect, Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextDontClip,
                              mAstroFont->zodLetter(eph::zod(z)));
        }
    }
}

QString QHoraView::housesType() const
{
    switch(mHouseSystemType)
    {
    case QHouseSystem::KOCH: return "koch";
    case QHouseSystem::REGIOMONTANUS: return "regiomontanus";
    case QHouseSystem::CAMPANUS: return "campanus";
    case QHouseSystem::EQUAL: return "equal";
    default: return "placidus";
    }
}

void QHoraView::setHousesType(const QString& housesType)
{
    QHouseSystem::Type houseSystemType = QHouseSystem::PLACIDUS;
    if (housesType == "koch")
    {
        houseSystemType = QHouseSystem::KOCH;
    }
    else if (housesType == "regiomontanus")
    {
        houseSystemType = QHouseSystem::REGIOMONTANUS;
    }
    else if (housesType == "campanus")
    {
        houseSystemType = QHouseSystem::CAMPANUS;
    }
    else if (housesType == "equal")
    {
        houseSystemType = QHouseSystem::EQUAL;
    }
    if (mHouseSystemType != houseSystemType)
    {
        mHouseSystemType = houseSystemType;
        emit housesTypeChanged();
    }
}

void QHoraView::paintHouses(QPainter* painter, const QHora* hora, bool outer)
{
    QFont housesFont;
    housesFont.setPixelSize(oneDegree() * 3);
    painter->setFont(housesFont);

    for (std::size_t h = 1; h <= QHouseSystem::HOUSE_COUNT; ++h)
    {
        const QHouseCusp* houseCusp = hora->house(h);
        const QEclLont houseCuspLont = houseCusp->eclPos()._M_lont;
        bool isAxis = h % 3 == 1;
        QPen housePen;
        housePen.setWidthF(isAxis ? 2.5 : 1.0);
        housePen.setColor(isAxis ? QColor(0x80,0x0,0x00) : QColor(0x0,0x0,0x80));
        painter->setPen(housePen);
        const qreal ARROW_SIZE = outer ? -0.05 : 0.05;
        const qreal lineFrom(outer ? ZODIAC_DIST + ZODIAC_WIDTH: EARTH_DIST);
        const qreal lineTo(isAxis ? ZODIAC_DIST + ARROW_SIZE : outer ? ZODIAC_DIST : 1.0);
        painter->drawLine(horaPoint(houseCuspLont, lineFrom), horaPoint(houseCuspLont, lineTo));
        if (h == 1 || h == 10)
        {
            painter->drawLine(horaPoint(houseCuspLont, lineTo),
                              horaPoint(houseCuspLont - 0.7, ZODIAC_DIST - ARROW_SIZE));
            painter->drawLine(horaPoint(houseCuspLont, lineTo),
                              horaPoint(houseCuspLont + 0.7, ZODIAC_DIST - ARROW_SIZE));
        }

        housePen.setColor(QColor(0x0,0x0,0x00));
        painter->setPen(housePen);
        QString houseSign = houseCusp->symbol(mAstroFont.get());
        QPointF houseSignPoint(horaPoint(houseCuspLont + (outer ? 2.0 : 8.0),
                                         outer ? ZODIAC_DIST + 0.04 : EARTH_DIST + 0.04));
        QSize textSize = mAstroFontMetrics.size(0, houseSign);
        QRectF houseSignRect(houseSignPoint - QPointF(textSize.width() / 2, textSize.height() / 2),
                           houseSignPoint + QPointF(textSize.width() / 2, textSize.height() / 2));
        painter->drawText(houseSignRect, Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextDontClip,
                          houseSign);
    }
}

void QHoraView::paintPlanets(QPainter* painter, const QHora* hora, bool outer)
{
    QHoraStellium::List stelliums(outer ? 5.0 : 8.0);
    QHora::Planets::ConstIterator planet = hora->planetsBegin(), pEnd = hora->planetsEnd();
    while (planet != pEnd)
    {
        stelliums.add(*planet);
        ++planet;
    }

    const qreal planetDist(outer ? ZODIAC_DIST : ASPECT_DIST);

    painter->setPen(QPen(QBrush("black"), 2.0));
    for (QHoraStellium stellium : stelliums)
    {
        int p = 0;
        for (QPlanet* planet : stellium)
        {
            const QEclLont displayLont = stellium.displayPos(p++)._M_lont;
            const QEclLont realLont = planet->eclPos()._M_lont;
            drawPlanetSymbol(painter, planet, displayLont, outer);

            painter->setPen(QPen(QBrush(QColor(0x80,0x80,0x80)), 1.0));
            painter->drawLine(horaPoint(realLont, planetDist), horaPoint(displayLont, planetDist + 0.05));
            if (outer)
            {
                painter->setPen(QPen(QBrush(QColor(0xE0,0xE0,0xE0)), 1.0));
                painter->drawLine(horaPoint(realLont, ASPECT_DIST), horaPoint(realLont, planetDist));
            }

            painter->setPen(QPen(QBrush(QColor(0x00,0x00,0x00)), 3.0));
            painter->drawLine(horaPoint(realLont, ASPECT_DIST - 0.004), horaPoint(realLont, ASPECT_DIST + 0.004));
        }
    }
}

void QHoraView::paintAspectConnections(QPainter* painter, const QHora* hora)
{
    QHora::Planets::ConstIterator planet = hora->planetsBegin();
    QHora::Planets::ConstIterator pEnd = hora->planetsEnd();
    while (planet != pEnd)
    {
        QHora::Planets::ConstIterator planet2 = planet;
        while (++planet2 != pEnd)
        {
            if ((*planet)->mIndex != QLunarNode::DRAGON_HEAD || (*planet2)->mIndex != QLunarNode::DRAGON_TAIL)
            {
                drawAspectConnection(painter, *planet, *planet2);
            }
        }

        drawAspectConnection(painter, *planet, hora->house(1));
        drawAspectConnection(painter, *planet, hora->house(4));
        drawAspectConnection(painter, *planet, hora->house(7));
        drawAspectConnection(painter, *planet, hora->house(10));

        ++planet;
    }
}
