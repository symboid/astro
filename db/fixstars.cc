
#include "astro/db/setup.h"
#include "astro/db/fixstars.h"
#include <QFile>

QFixstars::QFixstars(QObject* parent)
    : QObject(parent)
{
}

bool QFixstars::load()
{
    bool loadResult(false);
    QFile fixstarsFile("sefstars.txt");
    if (fixstarsFile.open(QIODevice::ReadOnly))
    {
    }
    return loadResult;
}
