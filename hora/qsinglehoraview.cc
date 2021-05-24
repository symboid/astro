
#include "astro/hora/setup.h"
#include "astro/hora/qsinglehoraview.h"
#include <QPainter>
#include "astro/hora/qhorastellium.h"

QSingleHoraView::QSingleHoraView(QQuickItem* parent)
    : QHoraView(parent)
    , mHora(new QHora(this))
    , mPlanetsModel(new QHoraPlanetsModel(mHora.get(), this))
    , mHousesModel(new QHoraHousesModel(mHora.get(), this))
{
    connect(mHora.get(), SIGNAL(calcTaskChanged()), this, SLOT(connectHoraSignals()));
    connect(mHora.get(), SIGNAL(coordsChanged()), this, SIGNAL(coordsChanged()));
    connect(mHora.get(), SIGNAL(houseSystemTypeChanged()), this, SIGNAL(housesTypeChanged()));

    connect(mHoraConfig->fixstars(), SIGNAL(includedChanged()), this, SLOT(onRecalcFinished()));

    qRegisterMetaType<QHora*>();
    qRegisterMetaType<QHoraCoords*>();
}

QHoraCoords* QSingleHoraView::coords() const
{
    return mHora->coords();
}

void QSingleHoraView::setCoords(QHoraCoords* coords)
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

eph::ecl_lont QSingleHoraView::mandalaLeft() const
{
    const QHouseCusp* firstHouse = *mHora->housesBegin();
    return firstHouse ? firstHouse->eclPos()._M_lont : eph::ecl_lont(0.0);
}

void QSingleHoraView::onRecalcStarted()
{
    mHora->setHouseSystemType(mHouseSystemType);
    mPlanetsModel->beginResetModel();
    mHousesModel->beginResetModel();
}

void QSingleHoraView::onRecalcFinished()
{
    mPlanetsModel->endResetModel();
    mHousesModel->endResetModel();
    update();
}

void QSingleHoraView::connectHoraSignals()
{
    if (QCalcTask* horaCalcTask = mHora->calcTask())
    {
        connect(horaCalcTask, SIGNAL(started()), this, SLOT(onRecalcStarted()));
        connect(horaCalcTask, SIGNAL(finished()), this, SLOT(onRecalcFinished()));
        connect(horaCalcTask, SIGNAL(aborted()), this, SLOT(onRecalcFinished()));
    }
}

void QSingleHoraView::paint(QPainter* painter)
{
    if (painter) {

        paintMandala(painter);

        // fixstars
        if (mHoraConfig->fixstars()->included())
        {
            Stellium<eph::basic_fixstar<eph_proxy>>::List fixstarStelliums(4.0);

            for (QHora::ConjunctingFixstars::ConstIterator fs = mHora->fixstarsBegin(),
                 fsEnd = mHora->fixstarsEnd(); fs != fsEnd; fs++)
            {
                fixstarStelliums.add(*fs);
            }
            QFont textFont;// = painter->font();
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

        // houses
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
            QSize textSize = mAstroFontMetrics.size(0, houseSign);
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
            for (QMagItem* aspectItem : *mHora->regularAspectItems())
            {
                eph::ecl_lont aspectLont = aspectItem->eclPos()._M_lont;
                painter->setPen(aspectItem->drawColor());
                painter->drawLine(horaPoint(aspectLont, 1.15),
                                  horaPoint(aspectLont, 1.20));

                drawRadialText(painter, aspectItem->symbol(mAstroFont.get()), aspectLont, 1.20);
            }
        }
    }
}
