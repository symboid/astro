
#include "astro/db/setup.h"
#include "astro/db/fixstars.h"
#include <QFile>
#include <QDir>
#include <QDebug>

QFixstar::QFixstar(const QString& nomenclature, const Magnitude& magnitude)
    : eph::fixstar_data<eph_proxy>(nomenclature.toStdString(), magnitude)
{
    _M_name_buffer[0] = ',';
    std::strncpy(_M_name_buffer + 1, _M_nomenclature.c_str(), eph_proxy::fixstar::NAME_BUFFER_LENGTH - 1);
}

void QFixstar::addName(const QString& name)
{
    if (name != "")
    {
        mNames.push_back(name);
    }
}

std::string QFixstar::name() const
{
    return mNames.size() > 0 ? mNames.front().toStdString() : _M_nomenclature;
}

std::string QFixstar::consltn() const
{
    return mConsltn != nullptr ? mConsltn->mDbName.toStdString() : "";
}

bool QFixstar::is_ecliptic() const
{
    return mConsltn != nullptr && mConsltn->mIsEcliptic;
}

eph::calc_result QFixstar::calc_pos(const eph::basic_time_point<eph_proxy>& timePoint,
        eph::ecl_pos& eclPos, eph::ecl_speed& eclSpeed)
{
    return eph_proxy::fixstar::calc_pos(_M_name_buffer, timePoint, eclPos, eclSpeed);
}

QConsltn::QConsltn(const QString& dbAbr, const QString& dbName)
    : mDbAbr(dbAbr)
    , mDbName(dbName)
    , mIsEcliptic(isEcliptic(mDbAbr))
{
}

bool QConsltn::isEcliptic(const QString& dbAbr)
{
    static const QString eclConsltn[12] =
            { "Ari", "Tau", "Gem", "Cnc", "Leo", "Vir", "Lib", "Sco", "Sgr", "Cap", "Aqr", "Psc" };
    return std::find(eclConsltn,eclConsltn + 12, dbAbr) != (eclConsltn + 12);
}

Fixstars::Fixstars()
{
}

bool Fixstars::parseConsltnName(const char* lineBuffer, QString& consltnName)
{
    bool isName = lineBuffer[0] == '#';
    const char* letter = lineBuffer + 1;
    while (*letter == ' ')
    {
        ++letter;
    }
    if ((isName = ('A' <= *letter && *letter <= 'Z')))
    {
        int spaceCount = 0;
        const char* begin = letter;
        while (isName && *++letter != '\0')
        {
            isName = ('A' <= *letter && *letter <= 'Z') || ('a' <= *letter && *letter <= 'z') ||
                    (*letter == ' ' && ++spaceCount <= 2 && *(letter+1) != '\0');
        }
        if (isName)
        {
            consltnName = QByteArray(begin,letter-begin);
        }
    }
    return isName;
}

bool Fixstars::isClassicFixstar(const QString& nomenclature)
{
    bool isClassic = false;
    int nomLength = nomenclature.length();
    if (nomLength > 3)
    {
        isClassic = nomenclature[nomLength - 3].isUpper() &&
                nomenclature[nomLength - 2].isLetter() && nomenclature[nomLength - 1].isLetter();
    }
    return isClassic;
}

bool Fixstars::load()
{
    mLoadedFixstarCount = 0;
    mSkippedEntryCount = 0;
    mFixstars.clear();

    QFile fixstarsFile(QString(eph_proxy::get_eph_dir_path().c_str()) + QDir::separator() + "sefstars.txt");
    if (fixstarsFile.open(QIODevice::ReadOnly))
    {
        QString consltnName;

        int lineNumber = 0;
        int consltnCount = 0;
        int lineLength = 0;
        char lineBuffer[512];
        while ((lineLength = fixstarsFile.readLine(lineBuffer, 512)) != -1)
        {
            lineBuffer[lineLength - 1] = 0;
            if (lineBuffer[0] != '#' && consltnName != "")
            {
                QStringList line = QString(lineBuffer).split(',');
                int f = 0;
                QString name, nomenclature;
                double magnitude;
                while (!line.isEmpty())
                {
                    QString field = line.takeFirst();
                    switch (++f)
                    {
                    case 1: name = field.left(field.indexOf(' ')); break;
                    case 2: nomenclature = field; break;
                    case 14: magnitude = field.toDouble();
                    }
                }
                addFixstar(name, nomenclature, magnitude, consltnName);
            }
            else if (parseConsltnName(lineBuffer, consltnName))
            {
                ++consltnCount;
                qDebug().noquote() << ">" << consltnName << "<";
            }
            else
            {
//                std::fprintf(stdout, "---%s\n", lineBuffer);
            }

            ++lineNumber;
        }
        qDebug("Loaded %i fixstars in %i/%lu constellations.\n", mLoadedFixstarCount, consltnCount, mConsltns.size());
        qDebug("Skipped %i fixstar entries.\n", mSkippedEntryCount);
    }
    return mLoadedFixstarCount > 0 || mSkippedEntryCount > 0;
}

void Fixstars::addFixstar(const QString& name, const QString& nomenclature,
        double magnitude, const QString& consltnName)
{
    Container::reverse_iterator existing = std::find_if(mFixstars.rbegin(), mFixstars.rend(),
            [nomenclature](QSharedPointer<QFixstar> fixstar){ return nomenclature == fixstar->_M_nomenclature.c_str(); });

    const QString consltnAbr = nomenclature.right(3);
    bool isEcliptic = QConsltn::isEcliptic(consltnAbr);
    QFixstar::Magnitude magnitudeFilter = isEcliptic ? 4.5 : 2.5;
    if (!isClassicFixstar(nomenclature))
    {
        mSkippedEntryCount++;
        qDebug().noquote() << "Irregular fixstar found: " << nomenclature;
    }
    else if (magnitude > magnitudeFilter || consltnName == "")
    {
        mSkippedEntryCount++;
    }
    else if (existing != mFixstars.rend())
    {
        (*existing)->addName(name);
    }
    else
    {
        QSharedPointer<QFixstar> newFixstar(new QFixstar(nomenclature, magnitude));
        newFixstar->addName(name);

        QSharedPointer<QConsltn>& constellation = mConsltns[consltnAbr];
        if (!constellation.get())
        {
            constellation.reset(new QConsltn(consltnAbr, consltnName));
        }
        newFixstar->mConsltn = constellation.get();

        mFixstars.push_back(newFixstar);
        mLoadedFixstarCount++;
    }
}

Fixstars::Container::iterator Fixstars::begin()
{
    return mFixstars.begin();
}

Fixstars::Container::iterator Fixstars::end()
{
    return mFixstars.end();
}

bool Fixstars::filter_match(const QSharedPointer<QFixstar> fixstar) const
{
    return fixstar != nullptr && fixstar->_M_magnitude < (fixstar->is_ecliptic() ? 4.5 : 2.5);
}
