#include "joueur.h"

#include <iostream>
#include <sstream>

boost::shared_ptr<Noir> Noir::instanceUnique;
boost::shared_ptr<Blanc> Blanc::instanceUnique;

Joueur::~Joueur()
{
}

Joueur::Joueur(QString const& Nom, QString const& rank)
    : nom(Nom), niveau(rank), pierresCapturees(0), territoire(0)
{}

boost::shared_ptr<Noir> Noir::donneInstance(QString const& Nom, QString const& rank, unsigned int h)
{
    if (instanceUnique == 0)
    {
        instanceUnique = boost::shared_ptr<Noir>(new Noir(Nom,rank,h));
    }
    return instanceUnique;
}

boost::shared_ptr<Blanc> Blanc::donneInstance(QString const& Nom, QString const& rank, double k)
{
    if (instanceUnique == 0)
    {
        instanceUnique = boost::shared_ptr<Blanc>(new Blanc(Nom,rank,k));
    }
    return instanceUnique;
}

Noir::Noir(QString const& Nom, QString const& rank, unsigned int h)
    : Joueur(Nom,rank), handicap(h)
{
    if (handicap>9) handicap=9;
}

Blanc::Blanc(QString const& Nom, QString const& rank, double k)
    : Joueur(Nom,rank), komi(k)
{}

void Joueur::addCapt(unsigned int i)
{
    pierresCapturees+=i;
}

void Blanc::libereInstance()
{
    instanceUnique.reset();
}

void Noir::libereInstance()
{
    instanceUnique.reset();
}

Blanc::~Blanc()
{
    Blanc::libereInstance();
}

Noir::~Noir()
{
    Noir::libereInstance();
}
