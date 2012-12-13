#include "InfosJoueur.h"

infosJoueurs::infosJoueurs() : QGridLayout()
{
    QLabel* rank =new QLabel("Niveau : ");
    QLabel* name =new QLabel("Nom : ");
    QLabel* capt =new QLabel("Capturées : ");
    nom = new QLabel(" ");
    niveau = new QLabel(" ");
    pierresCapturees= new QLabel(" ");
    titre= new QLabel(" ");

    addWidget(titre,0,0,1,2, Qt::AlignCenter);
    addWidget(name,1,0);
    addWidget(nom,1,1);
    addWidget(rank,2,0);
    addWidget(niveau,2,1);
    addWidget(capt,3,0);
    addWidget(pierresCapturees,3,1);
}

void infosJoueurs::setJoueur(boost::shared_ptr<Joueur> J)
{
    j=J;
    nom->setText(j->getNom());
    niveau->setText(j->getRank());
    pierresCapturees->setText("0");
}

