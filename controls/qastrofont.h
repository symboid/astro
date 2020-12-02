
#ifndef __SYMBOID_ASTRO_CONTROLS_QASTROFONT_H__
#define __SYMBOID_ASTRO_CONTROLS_QASTROFONT_H__

#include "astro/controls/defs.h"
#include <QFont>
#include "astro/eph/ecliptic.h"
#include "sdk/arch/mainobject.h"
#include "astro/hora/qplanet.h"
#include <QMap>

class ASTRO_CONTROLS_API QAstroFont : public QFont
{
protected:
    QAstroFont(const QString& familyName);
    virtual ~QAstroFont() = default;

public:
    virtual QString zodLetter(eph::zod zodSign) = 0;
    virtual QString objectLetter(QPlanet::Index planetIndex) = 0;
//    virtual QString aspectLetter(Aspect::Type aspectType);
    virtual QString retrogradLetter() = 0;
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
    QString zodLetter(eph::zod) override { return "z"; }
    QString objectLetter(QPlanet::Index) override { return "p"; }
    QString retrogradLetter() override { return "r"; }
};

class ASTRO_CONTROLS_API QSymboidFont : public QAstroFont
{
public:
    QSymboidFont(const QString& familyName);

public:
    QString zodLetter(eph::zod zodSign) override;
    QString objectLetter(QPlanet::Index planetIndex) override;
//    virtual QString objectLetter(ObjectId objectId);
//    virtual QString aspectLetter(Aspect::Type aspectType);
    QString retrogradLetter() override;
};

#endif // __SYMBOID_ASTRO_CONTROLS_QASTROFONT_H__
