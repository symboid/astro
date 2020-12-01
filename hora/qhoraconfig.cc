
#include "astro/hora/setup.h"
#include "astro/hora/qhoraconfig.h"

QHoraConfig::QHoraConfig(QObject* parent)
    : QConfigNode(parent)
{
    QSettings settings;
    for (int c = 0; c < subConfigCount(); ++c)
    {
        subConfig(c)->loadFromSettings(&settings);
    }

    mPlanets.push_back(hor::planet(hor::planet::sun));
    mPlanets.push_back(hor::planet(hor::planet::moon));
    mPlanets.push_back(hor::planet(hor::planet::mercury));
    mPlanets.push_back(hor::planet(hor::planet::venus));
    mPlanets.push_back(hor::planet(hor::planet::mars));
    mPlanets.push_back(hor::planet(hor::planet::jupiter));
    mPlanets.push_back(hor::planet(hor::planet::saturn));

    mPlanets.push_back(hor::planet(hor::planet::uranus));
    mPlanets.push_back(hor::planet(hor::planet::neptune));
    mPlanets.push_back(hor::planet(hor::planet::pluto));

    mPlanets.push_back(hor::planet(hor::planet::dragon_head));
    mPlanets.push_back(hor::planet(hor::planet::lilith));

/*
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::sunNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::monNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::merNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::venNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::marNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::jupNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::satNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::uraNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::nepNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::pluNode));

    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::nodNode));
    mPlanets.push_back(new QPlanet(this, &QOrbisConfigNode::lilNode));
*/
}

QHoraConfig::~QHoraConfig()
{
    QSettings settings;
    for (int c = 0; c < subConfigCount(); ++c)
    {
        subConfig(c)->saveToSettings(&settings);
    }
}
