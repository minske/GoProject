#ifndef LANCEUR_H
#define LANCEUR_H

#include <QFrame>
#include <QPushButton>

#include "../FenetrePrincipale/FenetreJeu.h"
#include "../FenetrePrincipale/FenetreSGF.h"


class Lanceur : public QFrame
{
    Q_OBJECT

public :
    Lanceur();

public slots :
    void LancerSGF();
    void LancerJeu();
    void Quitter();

private :
    QPushButton* boutonSGF;
    QPushButton* boutonJeu;
    QPushButton* boutonQuitter;

    boost::shared_ptr<FenetreJeu> m_fenetreJeu;
    boost::shared_ptr<FenetreSGF> m_fenetreSGF;
};

#endif // LANCEUR_H
