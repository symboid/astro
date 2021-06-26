
#include "astro/controls/setup.h"
#include "astro/controls/qastrofont.h"

#include <QResource>
#include <QFontDatabase>

QAstroFont::QAstroFont(const QString& familyName)
    : QFont(familyName)
{
}

QAstroFont::~QAstroFont()
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

QString QSymboidFont::zodLetter(eph::zod zodSign) const
{
    return QString(QChar('a' + char(zodSign) - char(eph::zod::ARI)));
}

QString QSymboidFont::planetLetter(eph_proxy::object::index planetIndex) const
{
    return planetIndex == eph_proxy::object::lilith ? QChar('A') : QChar('m' + int(planetIndex));
}

QString QSymboidFont::aspectLetter(int aspectDist) const
{
    switch (aspectDist)
    {
    case 0: return QChar('[');
    case 180: return QChar('\\');
    case 120: return QChar(']');
    case 90: return QChar('^');
    case 72: return QChar('\63');
    case 60: return QChar('`');

    case 30: return QChar('{');
    case 150: return QChar('}');
    case 45: return QChar('z');
    case 135: return QChar('y');
    default: return QChar(' ');
    }
}

QString QSymboidFont::chironLetter() const
{
    return QChar('K');
}

QString QSymboidFont::lilithLetter() const
{
    return QChar('A');
}

QString QSymboidFont::dragonHeadLetter() const
{
    return QChar('w');
}

QString QSymboidFont::dragonTailLetter() const
{
    return QChar('x');
}

QString QSymboidFont::retrogradLetter() const
{
    return QString('~');
}
