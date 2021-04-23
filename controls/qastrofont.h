
#ifndef __SYMBOID_ASTRO_CONTROLS_QASTROFONT_H__
#define __SYMBOID_ASTRO_CONTROLS_QASTROFONT_H__

#include "astro/controls/defs.h"
#include <QFont>
#include "astro/eph/ecliptic.h"
#include "sdk/arch/mainobject.h"
#include "astro/eph/proxy.h"
#include <QMap>

class ASTRO_CONTROLS_API QAstroFont : public QFont
{
protected:
    QAstroFont(const QString& familyName);
    virtual ~QAstroFont();// = default;

public:
    virtual QString zodLetter(eph::zod zodSign) const = 0;
    virtual QString planetLetter(eph_proxy::object::index planetIndex) const = 0;
    virtual QString aspectLetter(int aspectDist) const = 0;
    virtual QString chironLetter() const = 0;
    virtual QString lilithLetter() const = 0;
    virtual QString dragonHeadLetter() const = 0;
    virtual QString dragonTailLetter() const = 0;
    virtual QString retrogradLetter() const = 0;
};

class ASTRO_CONTROLS_API QAstroFontRepo
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

class ASTRO_CONTROLS_API QNonAstroFont : public QAstroFont
{
public:
    QNonAstroFont() : QAstroFont("") {}
    QString zodLetter(eph::zod) const override { return "z"; }
    QString planetLetter(eph_proxy::object::index) const override { return "p"; }
    QString aspectLetter(int) const override { return "a"; }
    QString chironLetter() const override { return "c"; }
    QString lilithLetter() const override { return "l"; }
    QString dragonHeadLetter() const override { return "h"; }
    QString dragonTailLetter() const override { return "t"; }
    QString retrogradLetter() const override { return "r"; }
};

class ASTRO_CONTROLS_API QSymboidFont : public QAstroFont
{
public:
    QSymboidFont(const QString& familyName);

public:
    QString zodLetter(eph::zod zodSign) const override;
    QString planetLetter(eph_proxy::object::index planetIndex) const override;
    QString aspectLetter(int aspectDist) const override;
    QString chironLetter() const override;
    QString lilithLetter() const override;
    QString dragonHeadLetter() const override;
    QString dragonTailLetter() const override;
    QString retrogradLetter() const override;
};

#endif // __SYMBOID_ASTRO_CONTROLS_QASTROFONT_H__
