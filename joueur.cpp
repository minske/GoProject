#include "joueur.h"
#include <iostream>
#include <sstream>

Noir* Noir::instanceUnique = 0;
Blanc* Blanc::instanceUnique = 0;

Joueur::~Joueur()
{
    delete nom; delete niveau; delete pierresCapturees;
}

Joueur::Joueur(QString const& Nom, QString const& rank)
{
    nom = new QLabel(Nom);
    niveau = new QLabel(rank);
    pierresCapturees = new QLabel("0");
    territoire=0;
}

Noir* Noir::donneInstance(QString const& Nom, QString const& rank, unsigned int h)
{
    if (instanceUnique == 0)
    {
        instanceUnique = new Noir(Nom,rank,h);
    }
    return instanceUnique;
}

Blanc* Blanc::donneInstance(QString const& Nom, QString const& rank, double k)
{
    if (instanceUnique == 0)
    {
        instanceUnique = new Blanc(Nom,rank,k);
    }
    return instanceUnique;
}

Noir::Noir(QString const& Nom, QString const& rank, unsigned int h)
    : Joueur(Nom,rank), handicap(h)
{
    if (handicap>9) handicap=9;


    infos = new QWidget;
    QGridLayout* grid = new QGridLayout;
    QLabel* titre = new QLabel("<b>Noir</b>");
    grid->addWidget(titre,0,0,1,2, Qt::AlignCenter);
    QLabel* name = new QLabel("Nom : ");
    grid->addWidget(name,1,0);

    grid->addWidget(nom,1,1);

    QLabel* rak = new QLabel("Niveau : ");
    grid->addWidget(rak,2,0);

    grid->addWidget(niveau,2,1);

    QLabel* capt = new QLabel("Capturées : ");
    grid->addWidget(capt,3,0);
    grid->addWidget(pierresCapturees,3,1);
    infos->setLayout(grid);

}

Blanc::Blanc(QString const& Nom, QString const& rank, double k)
    : Joueur(Nom,rank), komi(k)
{
    infos = new QWidget;
    QGridLayout* grid = new QGridLayout;
    QLabel* titre = new QLabel("<b>Blanc</b>");
    grid->addWidget(titre,0,0,1,2, Qt::AlignCenter);
    QLabel* name = new QLabel("Nom : ");
    grid->addWidget(name,1,0);

    grid->addWidget(nom,1,1);

    QLabel* rak = new QLabel("Niveau : ");
    grid->addWidget(rak,2,0);

    grid->addWidget(niveau,2,1);

    QLabel* capt = new QLabel("Capturées : ");
    grid->addWidget(capt,3,0);
    grid->addWidget(pierresCapturees,3,1);
    infos->setLayout(grid);
}

void Joueur::addCapt(unsigned int i)
{
    unsigned int res = getCapt()+i;
    std::ostringstream os;
    os << res;
    pierresCapturees->setText(QString::fromStdString(os.str()));
}


QWidget* Joueur::getInfos() const
{
    return infos;
}

void Blanc::libereInstance()
{
    instanceUnique = 0;
}

void Noir::libereInstance()
{
    instanceUnique = 0;
}

Blanc::~Blanc()
{
    Blanc::libereInstance();
}

Noir::~Noir()
{
    Noir::libereInstance();
}
