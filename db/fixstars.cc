
#include "astro/db/setup.h"
#include "astro/db/fixstars.h"
#include <QFile>
#include <QDir>
#include "sdk/arch/log.h"

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
    return mNames.size() > 0 ? mNames.front().toStdString() : "";//_M_nomenclature;
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

eph::basic_time_point<eph_proxy> QConsltn::sReferenceTime = eph::basic_calendar<eph_proxy>::time({ 1950,1,1, 0,0,0 });

void QConsltn::addFixstar(QSharedPointer<QFixstar> fixstar)
{
    fixstar->mConsltn = this;
    mFixstars.push_back(fixstar);
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
            consltnName = QByteArray(begin,int(letter-begin));
        }
    }
    return isName;
}

bool Fixstars::isLoadableFixstar(const QString& nomenclature)
{
    bool isClassic = false;
    int nomLength = nomenclature.length();
    if (nomenclature == "SgrA*" || nomenclature == "VC")
    {
        isClassic = true;
    }
    else if (nomenclature[0] == 'M')
    {
        isClassic = nomenclature.mid(1).toInt() > 0;
    }
    else if (nomLength > 3)
    {
        isClassic = nomenclature[nomLength - 3].isUpper() &&
                nomenclature[nomLength - 2].isLetter() && nomenclature[nomLength - 1].isLetter();
    }
    return isClassic;
}

bool Fixstars::load(const QString& fixstarsFilePath)
{
    mSkippedEntryCount = 0;
    mLoadedFixstars.clear();
    mEffectiveFixstars.clear();

    QFile fixstarsFile(fixstarsFilePath);
    if (fixstarsFile.open(QIODevice::ReadOnly))
    {
        QString consltnName;

        int lineNumber = 0;
        qint64 lineLength = 0;
        char lineBuffer[512];
        while ((lineLength = fixstarsFile.readLine(lineBuffer, 512)) != -1)
        {
            while (lineLength > 0 && (lineBuffer[lineLength - 1] == '\n' || lineBuffer[lineLength - 1] == '\r'))
            {
                lineBuffer[lineLength - 1] = 0;
                lineLength--;
            }
            if (lineBuffer[0] != '#')
            {
                QStringList line = QString(lineBuffer).split(',');
                int f = 0;
                QString name, nomenclature;
                double magnitude = 0.0;
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
            }
            else
            {
//                std::fprintf(stdout, "---%s\n", lineBuffer);
            }

            ++lineNumber;
        }

        log_info << "Loaded " << mEffectiveFixstars.size() << "/" << mLoadedFixstars.size() << " fixstars in "
                 <<  mConsltns.size() << " constellations.";
        log_info << "Skipped " << mSkippedEntryCount << " fixstar entries.";
    }
    return mLoadedFixstars.size();
}

void Fixstars::addFixstar(const QString& name, const QString& nomenclature,
        double magnitude, const QString& consltnName)
{
    Container::reverse_iterator existing = std::find_if(mLoadedFixstars.rbegin(), mLoadedFixstars.rend(),
            [nomenclature](QSharedPointer<QFixstar> fixstar){ return nomenclature == fixstar->_M_nomenclature.c_str(); });

    if (consltnName == "")
    {
        mSkippedEntryCount++;
        log_warning << "Fixstar without constellation found: " << nomenclature.toStdString();
    }
    else if (!isLoadableFixstar(nomenclature))
    {
        mSkippedEntryCount++;
        log_warning << "Irregular fixstar found: " << nomenclature.toStdString();
    }
    else if (existing != mLoadedFixstars.rend())
    {
        (*existing)->addName(name);
        if ((*existing)->mConsltn == nullptr && consltnName != "")
        {
            const QString consltnAbr = nomenclature.right(3);
            QSharedPointer<QConsltn>& constellation = mConsltns[consltnName];
            if (!constellation.get())
            {
                constellation.reset(new QConsltn(consltnAbr, consltnName));
            }
            constellation->addFixstar(*existing);
        }
    }
    else
    {
        const QString consltnAbr = nomenclature.right(3);

        QSharedPointer<QFixstar> newFixstar(new QFixstar(nomenclature, magnitude));
        newFixstar->addName(name);

        QSharedPointer<QConsltn>& constellation = mConsltns[consltnName];
        if (!constellation.get())
        {
            constellation.reset(new QConsltn(consltnAbr, consltnName));
        }
        constellation->addFixstar(newFixstar);

        mLoadedFixstars.push_back(newFixstar);

        bool isEcliptic = QConsltn::isEcliptic(consltnAbr);
        QFixstar::Magnitude magnitudeFilter = isEcliptic ? 4.5 : 2.5;
        if (magnitude < magnitudeFilter)
        {
            mEffectiveFixstars.push_back(newFixstar);
        }
    }
}

void Fixstars::addConsltn(const QString& consltnAbr, const QString& consltnName)
{
    if (consltnName != "")
    {
        QSharedPointer<QConsltn>& constellation = mConsltns[consltnName];
        if (!constellation.get())
        {
            constellation.reset(new QConsltn(consltnAbr, consltnName));
        }
    }
}

Fixstars::Container::iterator Fixstars::begin()
{
    return mEffectiveFixstars.begin();
}

Fixstars::Container::iterator Fixstars::end()
{
    return mEffectiveFixstars.end();
}

bool Fixstars::filter_match(const QSharedPointer<QFixstar> fixstar) const
{
    return fixstar != nullptr && fixstar->_M_magnitude < (fixstar->is_ecliptic() ? 4.5 : 2.5);
}
