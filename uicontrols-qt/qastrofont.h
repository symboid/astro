
#ifndef __SYMBOID_ASTRO_UICONTROLS_QT_QASTROFONT_H__
#define __SYMBOID_ASTRO_UICONTROLS_QT_QASTROFONT_H__

#include "astro/uicontrols-qt/defs.h"
#include <QFont>
#include "astro/eph/ecliptic.h"
#include "sdk/arch/mainobject.h"
#include "astro/calculo/planet.h"
#include <QMap>

class ASTRO_UICONTROLS_QT_API QAstroFont : public QFont
{
protected:
    QAstroFont(const QString& familyName);
    virtual ~QAstroFont() = default;

public:
    virtual QString zodLetter(eph::zod zodSign) = 0;
    virtual QString objectLetter(hor::planet::index planetIndex) = 0;
//    virtual QString aspectLetter(Aspect::Type aspectType);
    virtual QString retrogradLetter() = 0;
};

class ASTRO_UICONTROLS_QT_API QAstroFontRepo
{
    MAIN_OBJECT(QAstroFontRepo, AstroFontRepo)

public:
    QAstroFontRepo();
private:
    template <class AstroFont>
    void installFont(const QString& fontName, const QString& fontPath);

private:
    const QString mDefaultFontFamily;
    typedef QMap<QString, QSharedPointer<QAstroFont>> Fonts;
    Fonts mFonts;

public:
    QSharedPointer<QAstroFont> font(const QString& familyName) const;
    QSharedPointer<QAstroFont> defaultFont() const;
};

class ASTRO_UICONTROLS_QT_API QNonAstroFont : public QAstroFont
{
public:
    QNonAstroFont() : QAstroFont("") {}
    QString zodLetter(eph::zod) override { return "z"; }
    QString objectLetter(hor::planet::index) override { return "p"; }
    QString retrogradLetter() override { return "r"; }
};

class ASTRO_UICONTROLS_QT_API QSymboidFont : public QAstroFont
{
public:
    QSymboidFont(const QString& familyName);

public:
    QString zodLetter(eph::zod zodSign) override;
    QString objectLetter(hor::planet::index planetIndex) override;
//    virtual QString objectLetter(ObjectId objectId);
//    virtual QString aspectLetter(Aspect::Type aspectType);
    QString retrogradLetter() override;
};

#endif // __SYMBOID_ASTRO_UICONTROLS_QT_QASTROFONT_H__
