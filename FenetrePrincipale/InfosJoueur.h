#ifndef INFOSJOUEUR_H
#define INFOSJOUEUR_H

#include <QLabel>
#include <QGridLayout>
#include "../Partie/joueur.h"

/*
Classe infosJoueurs : permet d'afficher les infos sur un joueur (blanc ou noir) telles que
son nom, son niveau, le nombre de pierres qu'il a capturées, dans un GridLayout.
*/

class infosJoueurs : public QGridLayout
{
    QLabel* titre;
    QLabel* nom;
    QLabel* niveau;
    QLabel* pierresCapturees;
    boost::shared_ptr<Joueur> j;

public :
    infosJoueurs();
    void setTitre(QString const& t) {titre->setText(t);}
    void setTitre(QLabel* t) {titre=t;}
    void setNom(QString const& n) {nom->setText(n);}
    void setNom(QLabel* n) {nom=n;}
    void setNiveau(QString const& n) {niveau->setText(n);}
    void setNiveau(QLabel* n) {niveau=n;}
    void setCapt(QString const& c) {pierresCapturees->setText(c);}
    void setCapt(QLabel* c) {pierresCapturees=c;}
    void setJoueur(boost::shared_ptr<Joueur> J);
};


#endif // INFOSJOUEUR_H
