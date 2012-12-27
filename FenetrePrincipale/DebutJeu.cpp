#include "DebutJeu.h"
#include "../Lanceur/Lanceur.h"
#include <qformlayout.h>
#include <QPushButton>

DebutJeu::DebutJeu(Lanceur *l) : QWidget(), m_lanceur(l)
{
    //fenêtre qui apparait lorsque l'utilisateur clique sur "Jouer contre l'ordinateur"
    //Doit demander si l'utilisateur veut blanc ou noir (ou au hasard), quel komi, quel handicap

    QFormLayout * form = new QFormLayout();
    couleur = new QButtonGroup();

    handicap = new QSpinBox();
    handicap->setMaximum(9);
    handicap->setMinimum(0);
    handicap->setValue(0);
    handicap->setSingleStep(1);
    komi = new QDoubleSpinBox();
    komi->setMaximum(7.5);
    komi->setMinimum(0.5);
    komi->setValue(0.5);
    komi->setSingleStep(1);

    //form->addRow(QString("Choisissez votre couleur"),couleur);
    form->addRow("Choisissez le komi",komi);
    form->addRow("Choisissez le handicap donné à Noir",handicap);

    QPushButton* valider = new QPushButton("Valider");
    connect(valider,SIGNAL(clicked()),this,SLOT(valider()));

    form->addRow(valider);

    this->setLayout(form);
}


void DebutJeu::valider()
{
    boost::shared_ptr<FenetreJeu> fjeu (new FenetreJeu());
    m_lanceur->setFenetreJeu(fjeu);
    fjeu->show();
    this->close();
}

void DebutJeu::choixNoir()
{

}

void DebutJeu::choixBlanc()
{

}
