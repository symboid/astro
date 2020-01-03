
#include "astro/uicontrols-qt/setup.h"
#include "astro/uicontrols-qt/qastrofont.h"

#include <QResource>
#include <QFontDatabase>

QAstroFont::QAstroFont(const QString& familyName)
    : QFont(familyName)
{
}

QAstroFontRepo::QAstroFontRepo()
{
    installFont<QSymboidFont>("Symboid", ":font/symboid.ttf");
}

template <class AstroFont>
void QAstroFontRepo::installFont(const QString& fontName, const QString& fontPath)
{
    int fontID = QFontDatabase::addApplicationFont(fontPath);
    if (fontID != -1)
    {
        QString familyName = QFontDatabase::applicationFontFamilies(fontID).at(0);
        mFonts[fontName] = QSharedPointer<QAstroFont>(new AstroFont(familyName));
    }
}

QSharedPointer<QAstroFont> QAstroFontRepo::font(const QString& fontName) const
{
    Fonts::const_iterator fIt = mFonts.find(fontName);
    return fIt != mFonts.end() ? fIt.value() : QSharedPointer<QAstroFont>(new QNonAstroFont());
}

QSharedPointer<QAstroFont> QAstroFontRepo::defaultFont() const
{
    return font("Symboid");
}

QSymboidFont::QSymboidFont(const QString& familyName)
    : QAstroFont(familyName)
{
}

QString QSymboidFont::zodLetter(eph::zod zodSign)
{
    return QString('a' + char(zodSign) - char(eph::zod::ARI));
}

QString QSymboidFont::retrogradLetter()
{
    return QString('~');
}
