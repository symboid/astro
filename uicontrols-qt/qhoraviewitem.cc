
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/qhoraviewitem.h"
#include <QPainter>
#include "astro/uicontrols-qt/qhorastellium.h"

QHoraPlanetsModel::QHoraPlanetsModel(const hor::hora* hora, QObject* parent)
    : QAbstractTableModel(parent)
    , mHora(hora)
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
{
}

int QHoraPlanetsModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return mHora ? int(mHora->planet_count()) : 0;
}

int QHoraPlanetsModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return LastRole - FirstRole + 1;
}

QVariant QHoraPlanetsModel::data(const QModelIndex& index, int role) const
{
    QVariant planetData;
    if (mHora != Q_NULLPTR)
    {
        if (role == Qt::DisplayRole)
        {
            role = FirstRole + index.column();
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

QHash<int, QByteArray> QHoraPlanetsModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[SymbolRole] = "symbol";
    roles[EclLontRole] = "ecl_lont";
    roles[EclLattRole] = "ecl_latt";
    roles[EclSpeedRole] = "ecl_speed";
    return roles;
}

QHoraViewItem::QHoraViewItem(QQuickItem* parent)
    : QQuickPaintedItem(parent)
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
    , mPlanetsModel(new QHoraPlanetsModel(& mHora, this))
{
    setFlag(QQuickItem::ItemHasContents);
    setRenderTarget(QQuickPaintedItem::Image);
    connect(this, SIGNAL(widthChanged()), this, SLOT(calcMandalaGeometry()));
    connect(this, SIGNAL(heightChanged()), this, SLOT(calcMandalaGeometry()));

    mHora.add_planet(hor::planet(hor::planet::sun, 4.0));
    mHora.add_planet(hor::planet(hor::planet::moon, 4.0));
    mHora.add_planet(hor::planet(hor::planet::mercury, 3.0));
    mHora.add_planet(hor::planet(hor::planet::venus, 3.0));
    mHora.add_planet(hor::planet(hor::planet::mars, 3.0));
    mHora.add_planet(hor::planet(hor::planet::jupiter, 2.5));
    mHora.add_planet(hor::planet(hor::planet::saturn, 2.5));

    mHora.add_planet(hor::planet(hor::planet::uranus, 2.0));
    mHora.add_planet(hor::planet(hor::planet::neptune, 2.0));
    mHora.add_planet(hor::planet(hor::planet::pluto, 1.5));

    mHora.add_planet(hor::planet(hor::planet::dragon_head, 0.5));
    mHora.add_planet(hor::planet(hor::planet::lilith, 1.5));

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
}

void QHoraViewItem::calcMandalaGeometry()
{
    // basic attributes of horoscope mandala
    QRectF mandalaRect = boundingRect();
    mMandalaRadius = qMin(mandalaRect.width(), mandalaRect.height()) / 2.0;
    mMandalaCenter = mandalaRect.center();
}

qreal QHoraViewItem::eclipticRadius() const
{
    return mMandalaRadius * 0.8;
}

qreal QHoraViewItem::oneDegree() const
{
    return eclipticRadius() * PLANET_DIST * 2.0 * PI / 360;
}

eph::ecl_lont QHoraViewItem::mandalaLeft() const
{
    return mHora.houses()[1].pos()._M_lont;
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
    static constexpr int PLANET_COUNT = 7;
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
            Rank::PERG, Rank::PERG, Rank::HOME, Rank::PERG, Rank::PERG, Rank::HOME,
            Rank::PERG, Rank::PERG, Rank::EXIL, Rank::PERG, Rank::PERG, Rank::EXIL,
        },
        {
            Rank::EXIL, Rank::HOME, Rank::PERG, Rank::PERG, Rank::PERG, Rank::FALL,
            Rank::HOME, Rank::EXIL, Rank::PERG, Rank::PERG, Rank::PERG, Rank::ELEV,
        },
        {
            Rank::HOME, Rank::EXIL, Rank::PERG, Rank::EXIL, Rank::PERG, Rank::PERG,
            Rank::EXIL, Rank::HOME, Rank::PERG, Rank::ELEV, Rank::PERG, Rank::PERG,
        },
        {
            Rank::PERG, Rank::PERG, Rank::EXIL, Rank::ELEV, Rank::PERG, Rank::PERG,
            Rank::PERG, Rank::PERG, Rank::HOME, Rank::FALL, Rank::PERG, Rank::PERG,
        },
        {
            Rank::FALL, Rank::PERG, Rank::PERG, Rank::EXIL, Rank::EXIL, Rank::PERG,
            Rank::ELEV, Rank::PERG, Rank::PERG, Rank::HOME, Rank::HOME, Rank::PERG,
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
    switch (rank)
    {
    case Rank::ELEV: planetPen.setColor(Qt::red); break;
    case Rank::FALL: planetPen.setColor(Qt::darkYellow); break;
    case Rank::EXIL: planetPen.setColor(Qt::lightGray); break;
    default: planetPen.setColor(Qt::black); break;
    }
    painter->setPen(planetPen);

    // bounding circle
    if (rank == Rank::HOME)
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

        // zodiac sign domains
        mAstroFont->setPointSize(mFontPointSize);
        painter->setFont(*mAstroFont);
        QFontMetrics fontMetrics(*mAstroFont);
        for (std::size_t z = 1; z <= eph::house_system_mundan::house_count; ++z)
        {
            const eph::zod_sign_cusp zodSignCusp(z, eph::house_system_mundan::house_names[z]);
            eph::ecl_lont zodSignLont = zodSignCusp.pos()._M_lont;
            painter->drawLine(horaPoint(zodSignLont, 1.0), horaPoint(zodSignLont, 1.2));

            QPointF zodSignPoint(horaPoint(zodSignLont + 15.0, 1.1));
            QSize textSize = fontMetrics.size(0, mAstroFont->zodLetter(eph::zod(z)));
            QRectF zodSignRect(zodSignPoint - QPointF(textSize.width() / 2, textSize.height() / 2),
                               zodSignPoint + QPointF(textSize.width() / 2, textSize.height() / 2));
            painter->drawText(zodSignRect, Qt::AlignHCenter | Qt::AlignVCenter | Qt::TextDontClip,
                              mAstroFont->zodLetter(eph::zod(z)));
        }

        // houses
        const std::vector<eph::house_cusp>& houses = mHora.houses();
        for (std::size_t h = 1; h < houses.size(); ++h)
        {
            const eph::house_cusp& houseCusp = houses[h];
            bool isAxis = h % 3 == 1;
            QPen housePen;
            housePen.setWidthF(isAxis ? 2.5 : 1.0);
            housePen.setColor(isAxis ? QColor(0x80,0x0,0x00) : QColor(0x0,0x0,0x80));
            painter->setPen(housePen);
            painter->drawLine(horaPoint(houseCusp.pos()._M_lont, isAxis ? 1.2 : 1.0), horaPoint(houseCusp.pos()._M_lont, EARTH_DIST));
        }

        QFont planetFont = *mAstroFont;
        planetFont.setPixelSize(oneDegree() * 5);
        painter->setFont(planetFont);
        QFontMetrics planetFontMetrics(planetFont);


        // stelliums
        QList<QHoraStellium> stelliums;
        hor::hora::planet_it_const planet = mHora.planetsBegin(), pEnd = mHora.planetsEnd();
        while (planet != pEnd)
        {
            QHoraStellium newStellium(8.0, *planet);

            QList<QHoraStellium>::iterator stellium = stelliums.begin();
            while (stellium != stelliums.end())
            {
                if (newStellium.isMergeable(*stellium))
                {
                    newStellium.mergeIn(*stellium);
                    stelliums.erase(stellium);
                    stellium = stelliums.begin();
                }
                else
                {
                    ++stellium;
                }
            }
            stelliums.push_back(newStellium);
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
        planet = mHora.planetsBegin(), pEnd = mHora.planetsEnd();
        while (planet != pEnd)
        {
            const eph::ecl_pos planetPos = planet->pos();

            QPen aspectPen;
            hor::hora::planet_it_const planet2 = planet;
            while (++planet2 != pEnd)
            {
                hor::aspect_type aspect_conn = planet->aspect_conn(*planet2);
                if (aspect_conn != hor::none_aspect)
                {
                    QPointF leftPoint(horaPoint(planetPos._M_lont, ASPECT_DIST));
                    QPointF rightPoint(horaPoint(planet2->pos()._M_lont, ASPECT_DIST));
                    switch (aspect_conn)
                    {
                    case hor::conjunction:
                        aspectPen.setColor(Qt::black);
                        aspectPen.setWidthF(oneDegree());
                        break;
                    case hor::opposition:
                    case hor::quadrat:
                        aspectPen.setColor(Qt::red);
                        aspectPen.setWidthF(1.5);
                        break;
                    case hor::quintil:
                        aspectPen.setColor(Qt::blue);
                        aspectPen.setWidthF(1.0);
                        break;
                    default:
                        aspectPen.setColor(Qt::green);
                        aspectPen.setWidthF(1.5);
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
    mPlanetsModel->endResetModel();
    update();
}

void QHoraViewItem::setFontPointSize(int fontPointSize)
{
    if (mFontPointSize != fontPointSize)
    {
        mFontPointSize = fontPointSize;
        emit fontPointSizeChanged();
    }
}
