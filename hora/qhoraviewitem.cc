
#include "astro/hora/setup.h"
#include "astro/hora/qhoraviewitem.h"
#include <QPainter>
#include "astro/hora/qhorastellium.h"
#include <QFontMetrics>

QHoraPlanetsModel::QHoraPlanetsModel(const hor::hora* hora, QObject* parent)
    : QEclipticTableModel(hora, parent)
{
}

int QHoraPlanetsModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return mHora ? int(mHora->planet_count()) : 0;
}

QVariant QHoraPlanetsModel::data(const QModelIndex& index, int role) const
{
    QVariant planetData;
    if (mHora != Q_NULLPTR)
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
        const hor::planet& planet = mHora->planet(index.row());
        switch (role) {
        case SymbolRole: planetData = mAstroFont->objectLetter(planet.get_index()); break;
        case EclLontRole: planetData = planet.pos()._M_lont.to_arc_degree(); break;
        case EclLattRole: planetData = planet.pos()._M_latt; break;
        case EclSpeedRole: planetData = planet.speed()._M_lont; break;
        }
    }
    return planetData;
}

QStringList QHoraPlanetsModel::headerModel() const
{
    if (mWithSpeed)
    {
        return { "", tr("Ecliptic longitude"), tr("Ecliptic lattitude"), tr("Speed") };
    }
    else
    {
        return { "", tr("Ecliptic longitude"), tr("Ecliptic lattitude") };
    }
}

QHoraHousesModel::QHoraHousesModel(const hor::hora* hora, QObject* parent)
    : QEclipticTableModel(hora, parent)
{
}

int QHoraHousesModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return mHora ? int(eph::house_system_horizon::house_count) : 0;
}

QVariant QHoraHousesModel::data(const QModelIndex& index, int role) const
{
    QVariant houseData;
    if (mHora != Q_NULLPTR)
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
        std::size_t houseIndex = std::size_t(index.row() + 1);
        const eph::house_cusp house = mHora->houses().size() > houseIndex ? mHora->houses().at(houseIndex) : eph::house_cusp();
        switch (role) {
        case SymbolRole: {
            switch (houseIndex) {
            case 1: houseData = "I."; break;
            case 4: houseData = "IV."; break;
            case 7: houseData = "VII."; break;
            case 10: houseData = "X."; break;
            default: houseData = QString::number(houseIndex) + "."; break;
            }
            break;
        }
        case EclLontRole: houseData = house.pos()._M_lont.to_arc_degree(); break;
        case EclLattRole: houseData = 0; break;
        case EclSpeedRole: houseData = house.speed().lont_coord().arc_pos(); break;
        }
    }
    return houseData;
}

QStringList QHoraHousesModel::headerModel() const
{
    if (mWithSpeed)
    {
        return { "", tr("Ecliptic longitude"), tr("Speed (°/h)") };
    }
    else
    {
        return { "", tr("Ecliptic longitude") };
    }
}

QHoraViewItem::QHoraViewItem(QQuickItem* parent)
    : QQuickPaintedItem(parent)
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
    , mIsInteractive(false)
    , mPlanetsModel(new QHoraPlanetsModel(& mHora, this))
    , mHousesModel(new QHoraHousesModel(& mHora, this))
{
    setFlag(QQuickItem::ItemHasContents);
    setRenderTarget(QQuickPaintedItem::Image);
    connect(this, SIGNAL(widthChanged()), this, SLOT(calcMandalaGeometry()));
    connect(this, SIGNAL(heightChanged()), this, SLOT(calcMandalaGeometry()));

    for (hor::planet planet : mHoraConfig->mPlanets)
    {
        mHora.add_planet(planet);
    }

    connect(this, SIGNAL(interactiveChanged()), this, SLOT(onInteractiveChanged()));
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
    return eclipticRatio() + 0.15;
}

qreal QHoraViewItem::eclipticRatio() const
{
    return mHoraConfig->fixstars()->enabled() ? 0.55 : 0.85;
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
    return mHora.houses().size() > 1 ? mHora.houses()[1].pos()._M_lont : eph::ecl_lont(0.0);
}

QBrush QHoraViewItem::planetBrush(hor::planet::index planetIndex, qreal alpha)
{
    QBrush brush;
    // style = solid
    brush.setStyle(Qt::SolidPattern);
    // color
    QColor objectColor(0x80,0x80,0x80);
    switch (planetIndex)
    {
    case hor::planet::sun: objectColor = QColor(0xF0,0xE0,0x00); break;
    case hor::planet::moon: objectColor = QColor(0xC4,0xCA,0xCE); break;
    case hor::planet::mercury: objectColor = QColor(0xBF,0x93,0x39); break;
    case hor::planet::venus: objectColor = QColor(0x00,0x80,0x40); break;
    case hor::planet::mars: objectColor = QColor(0xB0,0x00,0x00); break;
    case hor::planet::jupiter: objectColor = QColor(0xC0,0x20,0xC0); break;
    case hor::planet::saturn: objectColor = QColor(0x20,0x20,0x20); break;

    case hor::planet::uranus: objectColor = QColor(0x84,0x87,0x89); break;
    case hor::planet::neptune: objectColor = QColor(0x20,0x20,0xC0); break;
    case hor::planet::pluto: objectColor = QColor(0x80,0x20,0x20); break;

    case hor::planet::dragon_head: objectColor = Qt::black; break;
    case hor::planet::lilith: objectColor = Qt::black; break;
    case hor::planet::chiron: objectColor = Qt::black; break;
    }
    objectColor.setAlphaF(alpha);
    brush.setColor(objectColor);
    return brush;
}

QHoraViewItem::Rank QHoraViewItem::planetRank(const hor::planet& planet) const
{
    int planetIndex = int(planet.get_index() - hor::planet::sun);
    eph::zod zodSign = planet.pos().zod_coords()._M_sign;
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

void QHoraViewItem::drawPlanetSymbol(QPainter* painter, const hor::planet& planet, const eph::ecl_pos& displayPos)
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
    QString planetText = QString(mAstroFont->objectLetter(planet.get_index()));
    QSize textSize = planetFontMetrics.size(0, planetText);
    QRectF planetSignRect(planetSignPoint - QPointF(textSize.width() / 2 - 1, textSize.height() / 2),
                          planetSignPoint + QPointF(textSize.width() / 2,   textSize.height() / 2));
    if (planet.is_retrograd())
    {
        QString retrogradText = mAstroFont->retrogradLetter();
        planetText += retrogradText;
    }
    painter->drawText(planetSignRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextDontClip, planetText);

}

void QHoraViewItem::paint(QPainter* painter)
{
    if (painter) {
        painter->setRenderHints(QPainter::Antialiasing, true);

        // eclipitic radius
        painter->setPen(QPen(QColor(0,0,0), 1.5));
        painter->drawEllipse(mMandalaCenter, eclipticRadius(), eclipticRadius());

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
        if (mHoraConfig->fixstars()->enabled())
        {
            Stellium<eph::basic_fixstar<eph_proxy>>::List fixstarStelliums(4.0);
    //        qreal fixstarRadius = eclipticRadius() * 1.15;
    //        painter->drawEllipse(mMandalaCenter, fixstarRadius, fixstarRadius);

            for (hor::hora::fixstar_const_it fs = mHora.fixstars_begin(),
                 fsEnd = mHora.fixstars_end(); fs != fsEnd; fs++)
            {
                fixstarStelliums.add(*fs);
            }
            for (Stellium<eph::basic_fixstar<eph_proxy>> fss : fixstarStelliums)
            {
                int p = 0;
                for (eph::basic_fixstar<eph_proxy> fixstar : fss)
                {
                    const bool isEcliptic = fixstar.data()->is_ecliptic();

                    eph::ecl_pos displayPos = fss.displayPos(p++);

                    painter->setPen(QPen(isEcliptic ? QColor(0x00,0x00,0x00) : QColor(0xC0,0xC0,0xC0), 2.0));
                    painter->drawEllipse(horaPoint(fixstar.pos()._M_lont, 1.15), 1.0, 1.0);

                    painter->setPen(QPen(isEcliptic ? QColor(0x00,0x00,0x00) : QColor(0xC0,0xC0,0xC0), 0.5));
                    painter->drawLine(horaPoint(fixstar.pos()._M_lont, 1.15),
                                      horaPoint(fixstar.pos()._M_lont, 1.25));
                    painter->drawLine(horaPoint(fixstar.pos()._M_lont, 1.25),
                                      horaPoint(displayPos._M_lont, 1.35));

                    painter->save();
                    painter->translate(boundingRect().center());

                    QFont textFont = painter->font();
                    textFont.setPixelSize(int(oneDegree()*3));
                    painter->setFont(textFont);

                    const QString name = QString("%1 (%2)").arg(fixstar.data()->name().c_str()).arg(fixstar.data()->consltn().c_str());
                    const QRectF textBoundingRect(QFontMetrics(painter->font(), painter->device()).boundingRect(name));

                    qreal displayLont = mandalaLeft() - displayPos._M_lont;
                    qreal baseAngle;
                    qreal translateX;
                    if (displayLont < 90.0 || 270.0 < displayLont)
                    {
                        baseAngle = displayLont;
                        translateX = -eclipticRadius() * 1.35 - textBoundingRect.width();
                    }
                    else
                    {
                        baseAngle = displayLont + 180.0;
                        translateX = eclipticRadius() * 1.35;
                    }
                    painter->rotate(baseAngle);
                    painter->drawText(textBoundingRect.translated(translateX, 0.0), Qt::TextSingleLine, name);
                    painter->restore();
                }
            }
        }

        // zodiac sign domains
        painter->setPen(QPen(QColor(0,0,0), 1.5));
        QFont zodiacFont(*mAstroFont);
        zodiacFont.setPixelSize(oneDegree() * 4);
        painter->setFont(zodiacFont);
        QFontMetrics fontMetrics(*mAstroFont);
        for (std::size_t z = 1; z <= eph::house_system_mundan::house_count; ++z)
        {
            const eph::zod_sign_cusp zodSignCusp(z, eph::house_system_mundan::house_names[z]);
            eph::ecl_lont zodSignLont = zodSignCusp.pos()._M_lont;
            painter->drawLine(horaPoint(zodSignLont, 1.0),
                              horaPoint(zodSignLont, mHoraConfig->fixstars()->enabled() ? 1.10 : 1.15));

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

        const std::vector<eph::house_cusp>& houses = mHora.houses();
        for (std::size_t h = 1; h < houses.size(); ++h)
        {
            const eph::house_cusp& houseCusp = houses[h];
            bool isAxis = h % 3 == 1;
            QPen housePen;
            housePen.setWidthF(isAxis ? 2.5 : 1.0);
            housePen.setColor(isAxis ? QColor(0x80,0x0,0x00) : QColor(0x0,0x0,0x80));
            painter->setPen(housePen);
            painter->drawLine(horaPoint(houseCusp.pos()._M_lont, isAxis ? 1.15 : 1.0), horaPoint(houseCusp.pos()._M_lont, EARTH_DIST));
            if (h == 1 || h == 10)
            {
                painter->drawLine(horaPoint(houseCusp.pos()._M_lont, 1.17), horaPoint(houseCusp.pos()._M_lont - 0.7, 1.10));
                painter->drawLine(horaPoint(houseCusp.pos()._M_lont, 1.17), horaPoint(houseCusp.pos()._M_lont + 0.7, 1.10));
            }

            housePen.setColor(QColor(0x0,0x0,0x00));
            painter->setPen(housePen);
            QString houseSign;
            switch (h) {
            case 1: houseSign = "I."; break;
            case 4: houseSign = "IV."; break;
            case 7: houseSign = "VII."; break;
            case 10: houseSign = "X."; break;
            default: houseSign = QString::number(h) + ".";
            }
            eph::ecl_lont houseSignLont = houseCusp.pos()._M_lont;
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
        Stellium<hor::planet>::List stelliums(8.0);
        hor::hora::planet_it_const planet = mHora.planetsBegin(), pEnd = mHora.planetsEnd();
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
            for (hor::planet planet : stellium)
            {
                eph::ecl_pos planetPos = stellium.displayPos(p++);
                drawPlanetSymbol(painter, planet, planetPos);

                painter->setPen(QPen(QBrush(QColor(0x80,0x80,0x80)), 1.0));
                painter->drawLine(horaPoint(planet.pos()._M_lont, ASPECT_DIST), horaPoint(planetPos._M_lont, ASPECT_DIST + 0.05));

                painter->setPen(QPen(QBrush(QColor(0x00,0x00,0x00)), 3.0));
                painter->drawLine(horaPoint(planet.pos()._M_lont, ASPECT_DIST - 0.004), horaPoint(planet.pos()._M_lont, ASPECT_DIST + 0.004));
            }
        }

        // aspect connections
        planet = mHora.planetsBegin();
        pEnd = mHora.planetsEnd();
        while (planet != pEnd)
        {
            const eph::ecl_pos planetPos = planet->pos();

            QPen aspectPen;
            hor::hora::planet_it_const planet2 = planet;
            while (++planet2 != pEnd)
            {
                hor::aspect_type aspect_conn = planet->aspect_conn(*planet2);
                QHoraConfig::AspectTypes::ConstIterator aspect = mHoraConfig->mAspectTypes.find(aspect_conn);
                if (aspect != mHoraConfig->mAspectTypes.end() && aspect.value()->value().toBool())
                {
                    QPointF leftPoint(horaPoint(planetPos._M_lont, ASPECT_DIST));
                    QPointF rightPoint(horaPoint(planet2->pos()._M_lont, ASPECT_DIST));
                    switch (aspect_conn)
                    {
                    case hor::conjunction:
                        aspectPen.setStyle(Qt::SolidLine);
                        aspectPen.setColor(Qt::black);
                        aspectPen.setWidthF(oneDegree());
                        break;

                    case hor::semi_quadrat:
                    case hor::sesqui_quadrat:
                        aspectPen.setStyle(Qt::DashLine);
                        aspectPen.setColor(Qt::red);
                        aspectPen.setWidthF(1.0);
                        break;

                    case hor::opposition:
                    case hor::quadrat:
                        aspectPen.setStyle(Qt::SolidLine);
                        aspectPen.setColor(Qt::red);
                        aspectPen.setWidthF(1.5);
                        break;

                    case hor::biquintile:
                        aspectPen.setStyle(Qt::DashLine);
                        aspectPen.setColor(Qt::blue);
                        aspectPen.setWidthF(1.0);
                        break;

                    case hor::quintile:
                        aspectPen.setStyle(Qt::SolidLine);
                        aspectPen.setColor(Qt::blue);
                        aspectPen.setWidthF(1.0);
                        break;

                    case hor::trigon:
                    case hor::sextile:
                        aspectPen.setStyle(Qt::SolidLine);
                        aspectPen.setColor(Qt::green);
                        aspectPen.setWidthF(1.5);
                        break;

                    default:
                        aspectPen.setStyle(Qt::DashLine);
                        aspectPen.setColor(Qt::darkRed);
                        aspectPen.setWidthF(1.0);
                        break;
                    }
                    painter->setPen(aspectPen);
                    painter->drawLine(leftPoint, rightPoint);
                }
            }
            ++planet;
        }
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

void QHoraViewItem::setYear(int year)
{
    if (mYear != year)
    {
        mYear = year;
        emit yearChanged();
    }
}

void QHoraViewItem::setMonth(int month)
{
    if (mMonth != month)
    {
        mMonth = month;
        emit monthChanged();
    }
}

void QHoraViewItem::setDay(int day)
{
    if (mDay != day)
    {
        mDay = day;
        emit dayChanged();
    }
}

void QHoraViewItem::setHour(int hour)
{
    if (mHour != hour)
    {
        mHour = hour;
        emit hourChanged();
    }
}

void QHoraViewItem::setMinute(int minute)
{
    if (mMinute != minute)
    {
        mMinute = minute;
        emit minuteChanged();
    }
}

void QHoraViewItem::setSecond(int second)
{
    if (mSecond != second)
    {
        mSecond = second;
        emit secondChanged();
    }
}

void QHoraViewItem::setGeoLont(qreal geoLont)
{
    if (!eph::arc_degree_equals(mGeoLont,geoLont))
    {
        mGeoLont = geoLont;
        emit geoLontChanged();
    }
}

void QHoraViewItem::setGeoLatt(qreal geoLatt)
{
    if (!eph::arc_degree_equals(mGeoLatt,geoLatt))
    {
        mGeoLatt = geoLatt;
        emit geoLattChanged();
    }
}

void QHoraViewItem::setTzDiff(qreal tzDiff)
{
    if (!eph::arc_degree_equals(mTzDiff,tzDiff))
    {
        mTzDiff = tzDiff;
        emit tzDiffChanged();
    }
}

void QHoraViewItem::setHousesType(const QString& housesType)
{
    if (mHousesType != housesType)
    {
        mHousesType = housesType;
        emit housesTypeChanged();
    }
}

void QHoraViewItem::setCalendarIsJulian(bool calendarIsJulian)
{
    if (mCalendarIsJulian != calendarIsJulian)
    {
        mCalendarIsJulian = calendarIsJulian;
        emit withJulianCalendarChanged();
    }
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
        connect(this, SIGNAL(yearChanged()), this, SLOT(recalc()));
        connect(this, SIGNAL(monthChanged()), this, SLOT(recalc()));
        connect(this, SIGNAL(dayChanged()), this, SLOT(recalc()));
        connect(this, SIGNAL(hourChanged()), this, SLOT(recalc()));
        connect(this, SIGNAL(minuteChanged()), this, SLOT(recalc()));
        connect(this, SIGNAL(secondChanged()), this, SLOT(recalc()));
        connect(this, SIGNAL(geoLattChanged()), this, SLOT(recalc()));
        connect(this, SIGNAL(geoLontChanged()), this, SLOT(recalc()));
        connect(this, SIGNAL(tzDiffChanged()), this, SLOT(recalc()));
        connect(this, SIGNAL(housesTypeChanged()), this, SLOT(recalc()));
        connect(this, SIGNAL(withJulianCalendarChanged()), this, SLOT(recalc()));
        connect(mHoraConfig.get(), SIGNAL(changed()), this, SLOT(recalc()));
        recalc();
    }
    else
    {
        disconnect(this, SIGNAL(yearChanged()), this, SLOT(recalc()));
        disconnect(this, SIGNAL(monthChanged()), this, SLOT(recalc()));
        disconnect(this, SIGNAL(dayChanged()), this, SLOT(recalc()));
        disconnect(this, SIGNAL(hourChanged()), this, SLOT(recalc()));
        disconnect(this, SIGNAL(minuteChanged()), this, SLOT(recalc()));
        disconnect(this, SIGNAL(secondChanged()), this, SLOT(recalc()));
        disconnect(this, SIGNAL(geoLattChanged()), this, SLOT(recalc()));
        disconnect(this, SIGNAL(geoLontChanged()), this, SLOT(recalc()));
        disconnect(this, SIGNAL(tzDiffChanged()), this, SLOT(recalc()));
        disconnect(this, SIGNAL(housesTypeChanged()), this, SLOT(recalc()));
        disconnect(this, SIGNAL(withJulianCalendarChanged()), this, SLOT(recalc()));
        disconnect(mHoraConfig.get(), SIGNAL(changed()), this, SLOT(recalc()));
    }
}

void QHoraViewItem::recalc()
{
    hor::hora_coords horaCoords;
    horaCoords._M_calendar_coords._M_year = mYear;
    horaCoords._M_calendar_coords._M_month = mMonth;
    horaCoords._M_calendar_coords._M_day = mDay;
    horaCoords._M_calendar_coords._M_hour = mHour;
    horaCoords._M_calendar_coords._M_minute = mMinute;
    horaCoords._M_calendar_coords._M_second = mSecond;
    horaCoords._M_calendar_coords._M_calendar_type = mCalendarIsJulian ? eph::calendar_type::JULIAN : eph::calendar_type::GREGORIAN;
    horaCoords._M_time_zone_diff = std::chrono::minutes(int(60.0 * mTzDiff));
    horaCoords._M_geo_latt = mGeoLatt;
    horaCoords._M_geo_lont = mGeoLont;
    mPlanetsModel->beginResetModel();
    mHousesModel->beginResetModel();
    emit startCalc();
    if (mHousesType == "koch")
    {
        mHora.calc<eph::house_system_koch>(horaCoords);
    }
    else if (mHousesType == "regiomontanus")
    {
        mHora.calc<eph::house_system_regiomontanus>(horaCoords);
    }
    else if (mHousesType == "campanus")
    {
        mHora.calc<eph::house_system_campanus>(horaCoords);
    }
    else if (mHousesType == "equal")
    {
        mHora.calc<eph::house_system_equal>(horaCoords);
    }
    else
    {
        mHora.calc<eph::house_system_placidus>(horaCoords);
    }
    mHora.calc_fixstars(horaCoords, *mFixstars.get());

    mPlanetsModel->endResetModel();
    mHousesModel->endResetModel();
    update();
    emit stopCalc();
}
