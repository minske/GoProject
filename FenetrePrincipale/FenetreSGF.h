#ifndef FENETRESGF_H
#define FENETRESGF_H

#include "FenetrePrincipale.h"
#include "../Partie/PartieIA.h"

class GobanIA;

class FenetreSGF : public FenetrePrincipale
{
    Q_OBJECT

public :
    FenetreSGF();
    boost::shared_ptr<FenetreSGF> sharedFromThis() {return boost::dynamic_pointer_cast<FenetreSGF>(shared_from_this());}

public slots :
    void ouvrirFichier();
    void enregistrerFichier();
    void nouveauFichier();
    void nextMove();
    void prevMove();
    void next5Moves();
    void prev5Moves();
    void debutPartie();
    void finPartie();
    void fermerFichier();
    void bouton_goban(int a, int o);


};

#endif // FENETRESGF_H
