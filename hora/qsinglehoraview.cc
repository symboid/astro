
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

void QSingleHoraView::paintFixstars(QPainter* painter)
{
    Stellium<eph::basic_fixstar<eph_proxy>>::List fixstarStelliums(4.0);

    for (QHora::ConjunctingFixstars::ConstIterator fs = mHora->fixstarsBegin(),
         fsEnd = mHora->fixstarsEnd(); fs != fsEnd; fs++)
    {
        fixstarStelliums.add(*fs);
    }
    QFont textFont;
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

void QSingleHoraView::paint(QPainter* painter)
{
    if (painter) {

        painter->setRenderHints(QPainter::Antialiasing, true);

        // fixstars
        if (mHoraConfig->fixstars()->included())
        {
            paintFixstars(painter);
        }

        // houses
        paintHouses(painter, mHora.get());

        paintPlanets(painter, mHora.get());

        paintMandala(painter);

        paintAspectConnections(painter, mHora.get());

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
