
#include "astro/hora/setup.h"
#include "astro/hora/qsinglehoraview.h"
#include <QPainter>
#include "astro/hora/qhorastellium.h"

QSingleHoraView::QSingleHoraView(QQuickItem* parent)
    : QHoraView(parent)
    , mHora(nullptr)
{
    connect(mHoraConfig->fixstars(), SIGNAL(includedChanged()), this, SLOT(onRecalcFinished()));

    qRegisterMetaType<QHora*>();
    qRegisterMetaType<QHoraCoords*>();
}

QHora* QSingleHoraView::hora() const
{
    return mHora;
}

void QSingleHoraView::setHora(QHora* hora)
{
    if (mHora)
    {
        disconnect(mHora, SIGNAL(calcTaskChanged()), this, SLOT(connectHoraSignals()));
        disconnect(mHora, SIGNAL(houseSystemTypeChanged()), this, SIGNAL(housesTypeChanged()));
    }
    if (mHora != hora)
    {
        mHora = hora;
        if (mHora)
        {
            connect(mHora, SIGNAL(calcTaskChanged()), this, SLOT(connectHoraSignals()));
            connect(mHora, SIGNAL(houseSystemTypeChanged()), this, SIGNAL(housesTypeChanged()));
        }
        emit horaChanged();
    }
}

eph::ecl_lont QSingleHoraView::mandalaLeft() const
{
    const QHouseCusp* firstHouse = mHora ? *mHora->housesBegin() : nullptr;
    return firstHouse ? firstHouse->eclPos()._M_lont : eph::ecl_lont(0.0);
}

void QSingleHoraView::onRecalcStarted()
{
    if (mHora)
    {
        mHora->setHouseSystemType(mHouseSystemType);
    }
}

void QSingleHoraView::onRecalcFinished()
{
    if (mHora)
    {
        update();
    }
}

void QSingleHoraView::connectHoraSignals()
{
    if (QCalcTask* horaCalcTask = (mHora? mHora->calcTask() : nullptr))
    {
        connect(horaCalcTask, SIGNAL(started()), this, SLOT(onRecalcStarted()));
        connect(horaCalcTask, SIGNAL(finished()), this, SLOT(onRecalcFinished()));
        connect(horaCalcTask, SIGNAL(aborted()), this, SLOT(onRecalcFinished()));
    }
}

void QSingleHoraView::paintFixstars(QPainter* painter)
{
    if (mHora)
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
}

void QSingleHoraView::paint(QPainter* painter)
{
    if (painter && mHora) {

        painter->setRenderHints(QPainter::Antialiasing, true);

        // fixstars
        if (mHoraConfig->fixstars()->included())
        {
            paintFixstars(painter);
        }

        // houses
        paintHouses(painter, mHora);

        paintPlanets(painter, mHora);

        paintMandala(painter);

        paintAspectConnections(painter, mHora);

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
