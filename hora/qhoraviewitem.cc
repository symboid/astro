
#include "astro/hora/setup.h"
#include "astro/hora/qhoraviewitem.h"
#include <QPainter>
#include "astro/hora/qhorastellium.h"
#include <QFontMetrics>

QHoraPlanetsModel::QHoraPlanetsModel(QHora* hora, QObject* parent)
    : QEclipticTableModel(hora, parent)
{
}

int QHoraPlanetsModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return hora() ? int(hora()->planetCount()) : 0;
}

QVariant QHoraPlanetsModel::data(const QModelIndex& index, int role) const
{
    QVariant planetData;
    if (hora() != Q_NULLPTR)
    {
        if (role == Qt::DisplayRole)
        {
            switch (index.column())
            {
            case 0: role = SymbolRole; break;
            case 1: role = EclLontRole; break;
            case 2: role = EclLattRole; break;
            case 3: role = EclSpeedRole; break;
            }
        }
        const QPlanet* planet = hora()->planet(index.row());
        switch (role) {
        case SymbolRole: planetData = planet->symbol(mAstroFont.get()); break;
        case EclLontRole: planetData = planet->eclPos()._M_lont.to_arc_degree(); break;
        case EclLattRole: planetData = planet->eclPos()._M_latt; break;
        case EclSpeedRole: planetData = planet->eclSpeed()._M_lont; break;
        }
    }
    return planetData;
}

QStringList QHoraPlanetsModel::headerModel() const
{
    if (mWithSpeed)
    {
        return { "", tr("Ecl. lng."), tr("Ecl. lat."), tr("°/day") };
    }
    else
    {
        return { "", tr("Ecl. lng."), tr("Ecl. lat.") };
    }
}

QHoraHousesModel::QHoraHousesModel(QHora* hora, QObject* parent)
    : QEclipticTableModel(hora, parent)
{
}

int QHoraHousesModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return hora() ? QHouseSystem::HOUSE_COUNT : 0;
}

QVariant QHoraHousesModel::data(const QModelIndex& index, int role) const
{
    QVariant houseData;
    if (hora() != Q_NULLPTR)
    {
        if (role == Qt::DisplayRole)
        {
            switch (index.column())
            {
            case 0: role = SymbolRole; break;
            case 1: role = EclLontRole; break;
            case 2: role = EclSpeedRole; break;
            }
        }
        int houseIndex = index.row() + 1;
        const QHouseCusp* house = hora()->house(houseIndex);
        switch (role) {
        case SymbolRole: houseData = house->symbol(mAstroFont.get()); break;
        case EclLontRole: houseData = house->eclPos()._M_lont.to_arc_degree(); break;
        case EclLattRole: houseData = 0; break;
        case EclSpeedRole: houseData = house->eclSpeed().lont_coord().arc_pos(); break;
        }
    }
    return houseData;
}

QStringList QHoraHousesModel::headerModel() const
{
    if (mWithSpeed)
    {
        return { "", tr("Ecl. lng."), tr("°/hour") };
    }
    else
    {
        return { "", tr("Ecl. lng.") };
    }
}

QHoraViewItem::QHoraViewItem(QQuickItem* parent)
    : QQuickPaintedItem(parent)
    , mHora(new QHora(this))
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
    , mIsInteractive(false)
    , mPlanetsModel(new QHoraPlanetsModel(mHora, this))
    , mHousesModel(new QHoraHousesModel(mHora, this))
{
    setFlag(QQuickItem::ItemHasContents);
    setRenderTarget(QQuickPaintedItem::Image);
    connect(this, SIGNAL(widthChanged()), this, SLOT(calcMandalaGeometry()));
    connect(this, SIGNAL(heightChanged()), this, SLOT(calcMandalaGeometry()));

    connect(this, SIGNAL(interactiveChanged()), this, SLOT(onInteractiveChanged()));
    connect(mHora, SIGNAL(planetsUpdated()), this, SLOT(recalc()));
    connect(mHora, SIGNAL(coordsChanged()), this, SIGNAL(coordsChanged()));
    connect(mHora, SIGNAL(houseSystemTypeChanged()), this, SIGNAL(housesTypeChanged()));

    qRegisterMetaType<QHoraCoords*>();
    qRegisterMetaType<QHora*>();
}

void QHoraViewItem::calcMandalaGeometry()
{
    // basic attributes of horoscope mandala
    QRectF mandalaRect = boundingRect();
    mMandalaRadius = qMin(mandalaRect.width(), mandalaRect.height()) / 2.0;
    mMandalaCenter = mandalaRect.center();
}

qreal QHoraViewItem::defaultZoom() const
{
    return eclipticRatio() + 0.2;
}

qreal QHoraViewItem::eclipticRatio() const
{
    return mHoraConfig->fixstars()->included() ? 0.55 : 0.8;
}

qreal QHoraViewItem::eclipticRadius() const
{
    return mMandalaRadius * eclipticRatio();
}

qreal QHoraViewItem::oneDegree() const
{
    return eclipticRadius() * PLANET_DIST * 2.0 * PI / 360;
}

eph::ecl_lont QHoraViewItem::mandalaLeft() const
{
    const QHouseCusp* firstHouse = *mHora->housesBegin();
    return firstHouse ? firstHouse->eclPos()._M_lont : eph::ecl_lont(0.0);
}

QBrush QHoraViewItem::planetBrush(QPlanet::Index planetIndex, qreal alpha)
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

QHoraViewItem::Rank QHoraViewItem::planetRank(const QPlanet* planet) const
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

void QHoraViewItem::drawPlanetSymbol(QPainter* painter, const QPlanet* planet, const eph::ecl_pos& displayPos)
{
    Rank rank = planetRank(planet);

    QFont planetFont = *mAstroFont;
    planetFont.setPixelSize(int(oneDegree() * 5));
    painter->setFont(planetFont);
    QFontMetrics planetFontMetrics(planetFont);

    eph::ecl_lont planetSignLont = displayPos._M_lont;
    QPointF planetSignPoint(horaPoint(planetSignLont, PLANET_DIST));

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
//    QString planetText = QString(mAstroFont->objectLetter(planet->mIndex));
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

void QHoraViewItem::drawRadialText(QPainter* painter, const QString& text, const QEclLont& lont, qreal dist)
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

void QHoraViewItem::paint(QPainter* painter)
{
    if (painter) {
        painter->setRenderHints(QPainter::Antialiasing, true);

        // eclipitic radius
        painter->setPen(QPen(QColor(0,0,0), 1.5));
        painter->drawEllipse(mMandalaCenter, eclipticRadius(), eclipticRadius());
        painter->drawEllipse(mMandalaCenter, eclipticRadius() * 1.15, eclipticRadius() * 1.15);

        // planet aspects radius
        painter->setPen(QPen(QColor(0x80,0x80,0x80), 1.0));
        qreal aspectRadius = eclipticRadius() * ASPECT_DIST;
        painter->drawEllipse(mMandalaCenter, aspectRadius, aspectRadius);

        // earth radius
        static const qreal EARTH_DIST = 0.25;
        painter->setPen(QPen(QColor(0,0,0), 1.5));
        qreal earthRadius = eclipticRadius() * EARTH_DIST;
        painter->drawEllipse(mMandalaCenter, earthRadius, earthRadius);

        // fixstars
        if (mHoraConfig->fixstars()->included())
        {
            Stellium<eph::basic_fixstar<eph_proxy>>::List fixstarStelliums(4.0);
    //        qreal fixstarRadius = eclipticRadius() * 1.15;
    //        painter->drawEllipse(mMandalaCenter, fixstarRadius, fixstarRadius);

            for (QHora::ConjunctingFixstars::ConstIterator fs = mHora->fixstarsBegin(),
                 fsEnd = mHora->fixstarsEnd(); fs != fsEnd; fs++)
            {
                fixstarStelliums.add(*fs);
            }
            QFont textFont = painter->font();
            textFont.setPixelSize(int(oneDegree()*3));
            painter->setFont(textFont);
            for (Stellium<eph::basic_fixstar<eph_proxy>> fss : fixstarStelliums)
            {
                int p = 0;
                for (eph::basic_fixstar<eph_proxy> fixstar : fss)
                {
                    const bool isEcliptic = fixstar.data()->is_ecliptic();

                    eph::ecl_pos displayPos = fss.displayPos(p++);

                    painter->setPen(QPen(isEcliptic ? QColor(0x00,0x00,0x00) : QColor(0xC0,0xC0,0xC0), 2.0));
                    painter->drawEllipse(horaPoint(fixstar.pos()._M_lont, 1.2), 1.0, 1.0);

                    painter->setPen(QPen(isEcliptic ? QColor(0x00,0x00,0x00) : QColor(0xC0,0xC0,0xC0), 0.5));
                    painter->drawLine(horaPoint(fixstar.pos()._M_lont, 1.2),
                                      horaPoint(fixstar.pos()._M_lont, 1.25));
                    painter->drawLine(horaPoint(fixstar.pos()._M_lont, 1.25),
                                      horaPoint(displayPos._M_lont, 1.35));

                    const QString name = QString("%1 (%2)").arg(fixstar.data()->name().c_str()).arg(fixstar.data()->consltn().c_str());
                    drawRadialText(painter, name, displayPos._M_lont, 1.35);
                }
            }
        }

        // zodiac sign domains
        painter->setPen(QPen(QColor(0,0,0), 1.5));
        QFont zodiacFont(*mAstroFont);
        zodiacFont.setPixelSize(oneDegree() * 4);
        painter->setFont(zodiacFont);
        QFontMetrics fontMetrics(*mAstroFont);
        for (int z = 1; z <= 12; ++z)
        {
            eph::ecl_lont zodSignLont = (z - 1)* 30;
            painter->drawLine(horaPoint(zodSignLont, 1.0),
                              horaPoint(zodSignLont, 1.15));

            QPointF zodSignPoint(horaPoint(zodSignLont + 15.0, 1.07));
            QSize textSize = fontMetrics.size(0, mAstroFont->zodLetter(eph::zod(z)));
            QRectF zodSignRect(zodSignPoint - QPointF(textSize.width() / 2, textSize.height() / 2),
                               zodSignPoint + QPointF(textSize.width() / 2, textSize.height() / 2));
            painter->drawText(zodSignRect, Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextDontClip,
                              mAstroFont->zodLetter(eph::zod(z)));
        }

        // houses
//        painter->setPen(QPen(QColor(0x0,0x0,0x80), 0.5));
//        qreal housesRadius = eclipticRadius() * (EARTH_DIST + 0.08);
//        painter->drawEllipse(mMandalaCenter, housesRadius, housesRadius);
        QFont housesFont;
        housesFont.setPixelSize(oneDegree() * 3);
        painter->setFont(housesFont);

        for (std::size_t h = 1; h <= QHouseSystem::HOUSE_COUNT; ++h)
        {
            const QHouseCusp* houseCusp = mHora->house(h);
            bool isAxis = h % 3 == 1;
            QPen housePen;
            housePen.setWidthF(isAxis ? 2.5 : 1.0);
            housePen.setColor(isAxis ? QColor(0x80,0x0,0x00) : QColor(0x0,0x0,0x80));
            painter->setPen(housePen);
            painter->drawLine(horaPoint(houseCusp->eclPos()._M_lont, isAxis ? 1.20 : 1.0), horaPoint(houseCusp->eclPos()._M_lont, EARTH_DIST));
            if (h == 1 || h == 10)
            {
                painter->drawLine(horaPoint(houseCusp->eclPos()._M_lont, 1.20), horaPoint(houseCusp->eclPos()._M_lont - 0.7, 1.10));
                painter->drawLine(horaPoint(houseCusp->eclPos()._M_lont, 1.20), horaPoint(houseCusp->eclPos()._M_lont + 0.7, 1.10));
            }

            housePen.setColor(QColor(0x0,0x0,0x00));
            painter->setPen(housePen);
            QString houseSign = houseCusp->symbol(mAstroFont.get());
            eph::ecl_lont houseSignLont = houseCusp->eclPos()._M_lont;
            QPointF houseSignPoint(horaPoint(houseSignLont + 8.0, EARTH_DIST + 0.04));
            QSize textSize = fontMetrics.size(0, houseSign);
            QRectF houseSignRect(houseSignPoint - QPointF(textSize.width() / 2, textSize.height() / 2),
                               houseSignPoint + QPointF(textSize.width() / 2, textSize.height() / 2));
            painter->drawText(houseSignRect, Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextDontClip,
                              houseSign);
        }

        QFont planetFont = *mAstroFont;
        planetFont.setPixelSize(oneDegree() * 5);
        painter->setFont(planetFont);
        QFontMetrics planetFontMetrics(planetFont);


        // stelliums
        QHoraStellium::List stelliums(8.0);
        QHora::Planets::ConstIterator planet = mHora->planetsBegin(), pEnd = mHora->planetsEnd();
        while (planet != pEnd)
        {
            stelliums.add(*planet);
            ++planet;
        }

        // planets
        painter->setPen(QPen(QBrush("black"), 2.0));
        for (QHoraStellium stellium : stelliums)
        {
            int p = 0;
            for (QPlanet* planet : stellium)
            {
                eph::ecl_pos planetPos = stellium.displayPos(p++);
                drawPlanetSymbol(painter, planet, planetPos);

                painter->setPen(QPen(QBrush(QColor(0x80,0x80,0x80)), 1.0));
                painter->drawLine(horaPoint(planet->eclPos()._M_lont, ASPECT_DIST), horaPoint(planetPos._M_lont, ASPECT_DIST + 0.05));

                painter->setPen(QPen(QBrush(QColor(0x00,0x00,0x00)), 3.0));
                painter->drawLine(horaPoint(planet->eclPos()._M_lont, ASPECT_DIST - 0.004), horaPoint(planet->eclPos()._M_lont, ASPECT_DIST + 0.004));
            }
        }

        // aspect connections
        planet = mHora->planetsBegin();
        pEnd = mHora->planetsEnd();
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

            drawAspectConnection(painter, *planet, mHora->house(1));
            drawAspectConnection(painter, *planet, mHora->house(4));
            drawAspectConnection(painter, *planet, mHora->house(7));
            drawAspectConnection(painter, *planet, mHora->house(10));

            ++planet;
        }

        if (mHoraConfig->basic_aspects())
        {
            // basic planet aspects
            painter->setFont(*mAstroFont);
            for (QMagObject* aspectObject : *mHora->regularAspectObjects())
            {
                eph::ecl_lont aspectLont = aspectObject->eclPos()._M_lont;
                painter->setPen(aspectObject->drawColor());
                painter->drawLine(horaPoint(aspectLont, 1.15),
                                  horaPoint(aspectLont, 1.20));

                drawRadialText(painter, aspectObject->symbol(mAstroFont.get()), aspectLont, 1.20);
            }
        }
    }
}

void QHoraViewItem::drawAspectConnection(QPainter* painter, const QPlanet* planet, const QHoraObject* object)
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

QPointF QHoraViewItem::horaPoint(eph::ecl_lont horaLont, qreal dist) const
{
    horaLont -= mandalaLeft();
    QPointF point;
    point.setX(mMandalaCenter.x() - cos(horaLont / 180.0 * PI) * eclipticRadius() * dist);
    point.setY(mMandalaCenter.y() + sin(horaLont / 180.0 * PI) * eclipticRadius() * dist);
    return point;
}

QHoraCoords* QHoraViewItem::coords() const
{
    return mHora->coords();
}

void QHoraViewItem::setCoords(QHoraCoords* coords)
{
    if (QHoraCoords* oldCoords = mHora->coords())
    {
        disconnect(oldCoords, SIGNAL(changed()), this, SIGNAL(coordsChanged()));
    }
    mHora->setCoords(coords);
    if (coords)
    {
        connect(coords, SIGNAL(changed()), this, SIGNAL(coordsChanged()));
    }
}

QString QHoraViewItem::housesType() const
{
    switch(mHora->houseSystemType())
    {
    case QHouseSystem::KOCH: return "koch";
    case QHouseSystem::REGIOMONTANUS: return "regiomontanus";
    case QHouseSystem::CAMPANUS: return "campanus";
    case QHouseSystem::EQUAL: return "equal";
    default: return "placidus";
    }
}

void QHoraViewItem::setHousesType(const QString& housesType)
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
    mHora->setHouseSystemType(houseSystemType);
}

void QHoraViewItem::setInteractive(bool isInteractive)
{
    if (mIsInteractive != isInteractive)
    {
        mIsInteractive = isInteractive;
        emit interactiveChanged();
    }
}

void QHoraViewItem::onInteractiveChanged()
{
    if (mIsInteractive)
    {
        connect(this, SIGNAL(coordsChanged()), this, SLOT(recalc()));
        connect(this, SIGNAL(housesTypeChanged()), this, SLOT(recalc()));
        connect(mHoraConfig.get(), SIGNAL(changed()), this, SLOT(recalc()));
        recalc();
    }
    else
    {
        disconnect(this, SIGNAL(coordsChanged()), this, SLOT(recalc()));
        disconnect(this, SIGNAL(housesTypeChanged()), this, SLOT(recalc()));
        disconnect(mHoraConfig.get(), SIGNAL(changed()), this, SLOT(recalc()));
    }
}

void QHoraViewItem::recalc()
{
    mPlanetsModel->beginResetModel();
    mHousesModel->beginResetModel();
    emit startCalc();

    mHora->calc();

    mPlanetsModel->endResetModel();
    mHousesModel->endResetModel();
    update();
    emit stopCalc();
}
