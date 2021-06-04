
#include "astro/hora/setup.h"
#include "astro/hora/qdoublehoraview.h"
#include <QPainter>

QDoubleHoraView::QDoubleHoraView(QQuickItem* parent)
    : QHoraView(parent)
    , mMainHora(nullptr)
    , mAuxHora(nullptr)
{
    connect(mHoraConfig->fixstars(), SIGNAL(includedChanged()), this, SLOT(onRecalcFinished()));

    qRegisterMetaType<QHora*>();
    qRegisterMetaType<QHoraCoords*>();
}

QHora* QDoubleHoraView::mainHora() const
{
    return mMainHora;
}

void QDoubleHoraView::setMainHora(QHora* hora)
{
    if (mMainHora)
    {
        disconnect(mMainHora, SIGNAL(calcTaskChanged()), this, SLOT(connectHoraSignals()));
//        disconnect(mMainHora, SIGNAL(houseSystemTypeChanged()), this, SIGNAL(housesTypeChanged()));
    }
    if (mMainHora != hora)
    {
        mMainHora = hora;
        if (mMainHora)
        {
            connect(mMainHora, SIGNAL(calcTaskChanged()), this, SLOT(connectHoraSignals()));
//            connect(mMainHora, SIGNAL(houseSystemTypeChanged()), this, SIGNAL(housesTypeChanged()));
        }
        emit mainHoraChanged();
    }
}

QHora* QDoubleHoraView::auxHora() const
{
    return mAuxHora;
}

void QDoubleHoraView::setAuxHora(QHora* hora)
{
    if (mAuxHora)
    {
        disconnect(mAuxHora, SIGNAL(calcTaskChanged()), this, SLOT(connectHoraSignals()));
//        disconnect(mAuxHora, SIGNAL(houseSystemTypeChanged()), this, SIGNAL(housesTypeChanged()));
    }
    if (mAuxHora != hora)
    {
        mAuxHora = hora;
        if (mAuxHora)
        {
            connect(mAuxHora, SIGNAL(calcTaskChanged()), this, SLOT(connectHoraSignals()));
//            connect(mAuxHora, SIGNAL(houseSystemTypeChanged()), this, SIGNAL(housesTypeChanged()));
        }
        emit auxHoraChanged();
    }
}

qreal QDoubleHoraView::eclipticRatio() const
{
    return 0.8 * QHoraView::eclipticRatio();
}

eph::ecl_lont QDoubleHoraView::mandalaLeft() const
{
    const QHouseCusp* firstHouse = mMainHora ? *mMainHora->housesBegin() : nullptr;
    return firstHouse ? firstHouse->eclPos()._M_lont : eph::ecl_lont(0.0);
}

void QDoubleHoraView::onRecalcStarted()
{
    if (mMainHora && mAuxHora)
    {
        mMainHora->setHouseSystemType(mHouseSystemType);
        mAuxHora->setHouseSystemType(mHouseSystemType);
    }
}

void QDoubleHoraView::onRecalcFinished()
{
    if (mMainHora && mAuxHora)
    {
        update();
    }
}

void QDoubleHoraView::connectHoraSignals()
{
    if (QCalcTask* horaCalcTask = (mMainHora? mMainHora->calcTask() : nullptr))
    {
        connect(horaCalcTask, SIGNAL(started()), this, SLOT(onRecalcStarted()));
        connect(horaCalcTask, SIGNAL(finished()), this, SLOT(onRecalcFinished()));
        connect(horaCalcTask, SIGNAL(aborted()), this, SLOT(onRecalcFinished()));
    }
    if (QCalcTask* horaCalcTask = (mAuxHora? mAuxHora->calcTask() : nullptr))
    {
        connect(horaCalcTask, SIGNAL(started()), this, SLOT(onRecalcStarted()));
        connect(horaCalcTask, SIGNAL(finished()), this, SLOT(onRecalcFinished()));
        connect(horaCalcTask, SIGNAL(aborted()), this, SLOT(onRecalcFinished()));
    }
}

void QDoubleHoraView::paint(QPainter* painter)
{
    if (painter && mMainHora && mAuxHora) {

        painter->setRenderHints(QPainter::Antialiasing, true);

        // houses
        paintHouses(painter, mMainHora);
        paintHouses(painter, mAuxHora, true);

        paintPlanets(painter, mMainHora);
        paintPlanets(painter, mAuxHora, true);

        paintMandala(painter);

        paintAspectConnections(painter, mMainHora);

        if (mHoraConfig->basic_aspects())
        {
            // basic planet aspects
            painter->setFont(*mAstroFont);
            for (QMagItem* aspectItem : *mMainHora->regularAspectItems())
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
