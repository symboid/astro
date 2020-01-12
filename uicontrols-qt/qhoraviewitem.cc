
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/qhoraviewitem.h"
#include <QPainter>

static const qreal PI = 3.14159265;

QHoraViewItem::QHoraViewItem(QQuickItem* parent)
    : QQuickPaintedItem(parent)
    , mAstroFont(QAstroFontRepo::mo()->defaultFont())
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

void QHoraViewItem::paint(QPainter* painter)
{
    if (painter) {
        painter->setRenderHints(QPainter::Antialiasing, true);

        // eclipitic radius
        painter->setPen(QPen(QColor(0,0,0), 1.5));
        painter->drawEllipse(mMandalaCenter, eclipticRadius(), eclipticRadius());

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
            painter->drawText(zodSignRect, Qt::AlignHCenter | Qt::AlignVCenter, mAstroFont->zodLetter(eph::zod(z)));
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

        const qreal PLANET_DIST = 0.875;
        const qreal ONE_DEGREE = eclipticRadius() * PLANET_DIST * 2.0 * PI / 360;
        const qreal ASPECT_DIST = 1.0 - 2.0 * (1.0 - PLANET_DIST);

        hor::hora::planet_it_const planet = mHora.planetsBegin(), pEnd = mHora.planetsEnd();
        while (planet != pEnd)
        {
            const eph::ecl_pos planetPos = planet->pos();
            qreal planetSize = planet->main_orbis() * ONE_DEGREE;
            painter->setBrush(planetBrush(planet->get_index(), 0.5));
            QPen planetPen;
            planetPen.setWidthF(1.0);
            planetPen.setColor(QColor(0x80,0x80,0x80));
            painter->setPen(planetPen);
            painter->drawEllipse(horaPoint(planetPos._M_lont, PLANET_DIST), planetSize, planetSize);

            painter->setPen(QPen(QBrush(QColor(0xC0,0xC0,0xC0)), 1.0));
            painter->drawLine(horaPoint(planetPos._M_lont, ASPECT_DIST + 0.01), horaPoint(planetPos._M_lont, ASPECT_DIST + 0.03));
            painter->drawLine(horaPoint(planetPos._M_lont, 0.97), horaPoint(planetPos._M_lont, 0.99));

            QPen aspectPen;
            aspectPen.setWidthF(1.0);
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
                        aspectPen.setColor(Qt::black);break;
                    case hor::opposition:
                    case hor::quadrat:
                        aspectPen.setColor(Qt::red);break;
                    case hor::quintil:
                        aspectPen.setColor(Qt::blue);break;
                    default:
                        aspectPen.setColor(Qt::green);break;
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
