
#ifndef __SYMBOL_ASTRO_HORA_QMAGITEM_H__
#define __SYMBOL_ASTRO_HORA_QMAGITEM_H__

#include "astro/hora/defs.h"
#include "astro/hora/qmagobject.h"
#include "astro/hora/qaspectobject.h"

class QMagItem : public QMagObject
{
    Q_OBJECT

public:
    QMagItem(QObject* parent, const QAspectObject* origin, const QAstroFont* font);

public:
    QEclPos eclPos() const override;
    QEclSpeed eclSpeed() const override;
private:
    const QEclPos mEclPos;
    const QEclSpeed mEclSpeed;

public:
    QString abbrName() const override;
    QString symbol(const QAstroFont* font) const override;
    QColor drawColor() const override;
private:
    const QString mAbbrName;
    const QString mSymbol;
    const QColor mDrawColor;

public:
    QOrbisValue orbis() const override;
private:
    const QOrbisValue mOrbis;

public:
    qreal aspectDist() const;
    QString horaObjectSymbol() const;
private:
    const qreal mAspectDist;
    const QString mHoraObjectSymbol;
};

class QMagItemList : public QList<QMagItem*>
{
public:
    void clear();
    void insert(QMagItem* magItem);

    struct Siblings
    {
        QMagItem* mPrec = nullptr;
        QMagItem* mSucc = nullptr;
    };
    Siblings find(const QEclPos& eclPos) const;
};

#endif // __SYMBOL_ASTRO_HORA_QMAGITEM_H__
