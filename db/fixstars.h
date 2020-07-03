
#ifndef __SYMBOID_ASTRO_DB_FIXSTARS_H__
#define __SYMBOID_ASTRO_DB_FIXSTARS_H__

#include "astro/db/defs.h"
#include "sdk/arch/mainobject.h"
#include "astro/eph/proxy.h"
#include "astro/eph/fixstar.h"
#include <list>
#include <map>
#include <QString>
#include <QVector>

class ASTRO_DB_API QConsltn;

class ASTRO_DB_API QFixstar : public eph::fixstar_data<eph_proxy>
{
public:
    typedef eph::fixstar_data<eph_proxy>::magnitude Magnitude;

public:
    QFixstar(const QString& nomenclature, const Magnitude& magnitude);

public:
    void addName(const QString& name);
private:
    QVector<QString> mNames;

public:
    QConsltn* mConsltn = nullptr;

public:
    qreal orbis() const { return  1.5; }

public:
    std::string name() const override;
    std::string consltn() const override;
    bool is_ecliptic() const override;

public:
    eph::calc_result calc_pos(const eph::basic_time_point<eph_proxy>& timePoint,
            eph::ecl_pos& eclPos, eph::ecl_speed& eclSpeed) override;
private:
    char _M_name_buffer[eph_proxy::fixstar::NAME_BUFFER_LENGTH + 1];
};

class ASTRO_DB_API QConsltn
{
public:
    QConsltn(const QString& dbAbr, const QString& dbName);

public:
    const QString mDbAbr;
    const QString mDbName;
    const bool mIsEcliptic;

private:
    static bool isEcliptic(const QString& dbAbr);
};

class ASTRO_DB_API Fixstars
{
    MAIN_OBJECT(Fixstars,Fixstars)

public:
    Fixstars();

public:
    bool load();
private:
    bool parseConsltnName(const char* lineBuffer, QString& consltnName);
    bool loadFromDisk();
    void addFixstar(const QString& name, const QString& nomenclature,
            double magnitude, const QString& consltnName);

private:
    typedef std::map<QString, std::shared_ptr<QConsltn>> Constellations;
    Constellations mConsltns;

public:
    typedef std::list<QFixstar*> Container;
private:
    Container mFixstars;
public:
    Container::iterator begin();
    Container::iterator end();
    bool filter_match(const QFixstar* fixstarData) const;
};

#endif // __SYMBOID_ASTRO_DB_FIXSTARS_H__
